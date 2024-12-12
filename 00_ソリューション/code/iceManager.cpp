//*****************************************************
//
// 氷マネージャー[IceManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "particle.h"
#include "flowIce.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "meshfield.h"
#include "ocean.h"
#include "polygon3D.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "manager.h"
#include "gameManager.h"
#include "player.h"
#include "inputmouse.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// 六角形の割合X
const float RATE_HEX_Z = 0.18f;	// 六角形の割合Z

const float WIDTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_X;	// グリッドの幅
const float DEPTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_Z;	// グリッドの奥行き
const float OCEAN_FLOW_MIN = 1.00f;								// 海流の速度最小
const float OCEAN_FLOW_MAX = 5.00f;								// 海流の速度最大

const float RANGE_SELECT_ICE = D3DX_PI;	// 氷を選択するときの角度の範囲

const D3DXCOLOR COL_ICE[CIceManager::E_Pecker::PECKER_MAX] =	// 突っつく人による色
{
	D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
};
const D3DXCOLOR COL_ICE_DEFAULT = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// デフォルト色

const float POW_CAMERAQUAKE_DEFAULT = 0.1f;		// カメラの揺れのデフォルト値
const int FRAME_CAMERAQUAKE_DEFAULT = 30;		// カメラの揺れのデフォルトフレーム数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority), m_nNumGridVirtical(0), m_nNumGridHorizontal(0), m_dirStream(COcean::STREAM_UP)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceManager::~CIceManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CIceManager *CIceManager::Create(int nNumV, int nNumH)
{
	if (s_pIceManager == nullptr)
	{
		s_pIceManager = new CIceManager;

		if (s_pIceManager != nullptr)
		{
			s_pIceManager->m_nNumGridVirtical = nNumV;
			s_pIceManager->m_nNumGridHorizontal = nNumH;
			s_pIceManager->Init();
		}
	}

	return s_pIceManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceManager::Init(void)
{
	// 指定された数分だけグリッド配列をリサイズ
	// 縦のグリッド用意
	m_aGrid.resize(m_nNumGridVirtical);

	for (int i = 0; i < m_nNumGridVirtical; i++)
		m_aGrid[i].resize(m_nNumGridHorizontal);

	// グリッドの位置設定
	SetGridPos();

	// 海流を初期化
	m_dirStream = COcean::STREAM_DOWN;
	m_dirStreamNext = COcean::STREAM_DOWN;
	m_fOceanLevel = OCEAN_FLOW_MAX;

	return S_OK;
}

//=====================================================
// グリッドの位置を設定
//=====================================================
void CIceManager::SetGridPos(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			D3DXVECTOR3 pos;
			pos = { j * WIDTH_GRID - WIDTH_GRID * m_nNumGridHorizontal * 0.5f,10.0f,i * DEPTH_GRID - DEPTH_GRID * m_nNumGridVirtical * 0.5f };

			// 縦で偶数列だったらずらす
			if (i % 2 == 0)
			{
				pos.x += WIDTH_GRID * 0.5f;
			}

			m_aGrid[i][j].pos = pos;
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CIceManager::Uninit(void)
{
	s_pIceManager = nullptr;

	m_aGrid.clear();

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CIceManager::Update(void)
{
	// 無効な停止氷の検出
	SearchInvailStopIce();

	// 氷の状態管理
	ManageStateIce();
	
	// さざ波の属性割り当て
	BindRippleElements();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 氷の状態管理
//=====================================================
void CIceManager::ManageStateIce(void)
{
	if (m_aGrid.empty())
		return;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
#ifdef _DEBUG
			if (i == 0 ||
				i == m_nNumGridVirtical - 1 ||
				j == 0 ||
				j == m_nNumGridHorizontal - 1)
			{
				CEffect3D::Create(m_aGrid[i][j].pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
#endif

			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// つっついた氷に挟まれてるかの判定
			if (JudgeBetweenPeck(i, j))
				continue;

			m_aGrid[i][j].pIce->EnableBreak(false);
			m_aGrid[i][j].pIce->EnableCanFind(true);
			m_aGrid[i][j].pIce->SetColor(COL_ICE_DEFAULT);
		}
	}
}

//=====================================================
// 無効な停止氷の検出
//=====================================================
void CIceManager::SearchInvailStopIce(void)
{
	vector<CIce*> apIce;	// 無効な停止氷の配列

	// 繋がってない氷の検出
	SearchNotConnectIce(apIce);

	for (auto it : apIce)	// 氷を流す設定にする
		it->ChangeState(new CIceStateFlow);
}

//=====================================================
// 繋がってない氷の検出
//=====================================================
void CIceManager::SearchNotConnectIce(vector<CIce*> &rpIce)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsCanPeck())
				continue;

			// 壊れないブロックが行う信号解除
			DisableFromHardIce(i, j,false);
		}
	}

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (!m_aGrid[i][j].pIce->IsCanFind())
				continue;

			if (m_aGrid[i][j].pIce->IsPeck())
				continue;

			// 探索済みでない氷を追加
			rpIce.push_back(m_aGrid[i][j].pIce);
		}
	}
}

//=====================================================
// つっついた氷に挟まれてるかの判定
//=====================================================
bool CIceManager::JudgeBetweenPeck(int nNumV, int nNumH)
{
	return false;
}

//=====================================================
// 氷の生成
//=====================================================
CIce *CIceManager::CreateIce(int nGridV, int nGridH, CIce::E_Type type)
{
	if (m_aGrid[nGridV][nGridH].pIce != nullptr)
		return m_aGrid[nGridV][nGridH].pIce;

	CIce *pIce = nullptr;

	pIce = CIce::Create(type);

	if (pIce == nullptr)
		return nullptr;

	// 氷のトランスフォーム設定
	pIce->SetPosition(m_aGrid[nGridV][nGridH].pos);
	pIce->SetTransform(Grid::SIZE);

	// 光る処理の初期化
	pIce->StartFlash();

	// 氷を配列にセット
	m_aGrid[nGridV][nGridH].pIce = pIce;

	return pIce;
}

//=====================================================
// 流氷の生成
//=====================================================
CIce *CIceManager::CreateFlowIce(int nGridV, int nGridH, CIce::E_Type type)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create(type);

	if (pIce == nullptr)
		return nullptr;

	D3DXVECTOR3 pos;
	pos = { nGridH * WIDTH_GRID - WIDTH_GRID * m_nNumGridHorizontal * 0.5f,10.0f,nGridV * DEPTH_GRID - DEPTH_GRID * m_nNumGridVirtical * 0.5f };

	// 偶数行だったらずらす
	if (nGridV % 2 == 0)
		pos.x += WIDTH_GRID * 0.5f;

	// 氷のトランスフォーム設定
	pIce->SetPosition(pos);
	pIce->SetTransform(Grid::SIZE);

	// 光る処理の初期化
	pIce->StartFlash();

	return pIce;
}

//=====================================================
// 氷の停止
//=====================================================
void CIceManager::StopIce(CIce *pIce)
{
	if (pIce == nullptr)
		return;

	pIce->SetState(CIce::E_State::STATE_NORMAL);

	// グリッドに属性を割り振る
}

//=====================================================
// 氷をつつけるかのチェック
//=====================================================
bool CIceManager::CheckPeck(int nNumV, int nNumH, float fRot, D3DXVECTOR3 pos, E_Direction *pDir, CIce **ppIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	CIce* pIcePeck = nullptr;

	// 向きに合わせて氷を選択
	float fDiffMin = D3DX_PI * 2;
	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		// 氷とスティック角度の比較
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();
		// 差分ベクトルの角度を取得
		D3DXVECTOR3 vecDiff = posIce - pos;
		float fRotToTarget = atan2f(vecDiff.x, vecDiff.z);

		// 差分角度が範囲内かどうか取得
		float fRotDiff = fRotToTarget - fRot;
		universal::LimitRot(&fRotDiff);

		if (RANGE_SELECT_ICE * RANGE_SELECT_ICE > fRotDiff * fRotDiff)
		{// 最低限選べる氷の判定
			if (fRotDiff * fRotDiff < fDiffMin * fDiffMin)
			{// 最小の角度なのを保存
				fDiffMin = fRotDiff;
				pIcePeck = apIce[i];

				if (pDir != nullptr)	// 番号保存
					*pDir = (E_Direction)i;

				*ppIce = pIcePeck;	// 選んでる氷保存
			}
		}
	}

	// 番号を取得
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// 突っつける氷かのチェック
	return CanPeck(pIcePeck, nNumBreakV, nNumBreakH);
}

//=====================================================
// 氷をつつく
//=====================================================
bool CIceManager::PeckIce(int nNumV, int nNumH, float fRot,D3DXVECTOR3 pos, bool *pResultBreak)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	CIce* pIcePeck = nullptr;

	// 向きに合わせて氷を選択
	float fDiffMin = D3DX_PI * 2;

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		// 氷とスティック角度の比較
		D3DXVECTOR3 posIce = it->GetPosition();

		// 差分ベクトルの角度を取得
		D3DXVECTOR3 vecDiff = posIce - pos;
		float fRotToTarget = atan2f(vecDiff.x, vecDiff.z);

		// 差分角度が範囲内かどうか取得
		float fRotDiff = fRotToTarget - fRot;
		universal::LimitRot(&fRotDiff);

		if (RANGE_SELECT_ICE * RANGE_SELECT_ICE > fRotDiff * fRotDiff)
		{// 最低限選べる氷の判定
			if (fRotDiff * fRotDiff < fDiffMin * fDiffMin)
			{
				fDiffMin = fRotDiff;
				pIcePeck = it;
			}
		}
	}

	if (pIcePeck == nullptr)
		return false;

	// 番号を取得
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// 突っつける氷かのチェック
	if (!CanPeck(pIcePeck, nNumBreakV, nNumBreakH))
		return false;

	// 氷を突っついた判定にする
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
		CSound::GetInstance()->Play(CSound::LABEL_SE_BREAK_ICE);
	}

	// 氷探索の再帰関数
	FindIce(nNumBreakV, nNumBreakH, 0, pIceStand, apIce,false);

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsCanPeck())
				continue;

			// 探索フラグの無効化
			DisableFind();

			// 壊れないブロックが行う信号解除
			DisableFromHardIce(i, j);
		}
	}

	// 探索フラグの無効化
	DisableFind();

	// 壊れるブロックをまとまりにする
	SummarizeIce(nNumBreakV, nNumBreakH);

	// 氷が壊れるフラグが立っていたら氷を壊す
	bool bResultBreak = BreakIce();

	if (pResultBreak != nullptr)
		*pResultBreak = bResultBreak;

	return true;
}

//=====================================================
// 番号でつっつく処理
//=====================================================
bool CIceManager::PeckIce(int nIdxV, int nIdxH)
{
	if (nIdxV < 0 || nIdxV >= m_nNumGridVirtical ||
		nIdxH < 0 || nIdxH >= m_nNumGridHorizontal)
		return false;

	vector<CIce*> apIce = GetAroundIce(nIdxV, nIdxH);

	int nNumBreakV = nIdxV;
	int nNumBreakH = nIdxH;

	CIce* pIcePeck = m_aGrid[nIdxV][nIdxH].pIce;

	if (pIcePeck == nullptr)
		return false;

	// 番号を取得
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// 突っつける氷かのチェック
	if (!CanPeck(pIcePeck, nNumBreakV, nNumBreakH))
		return false;

	// 氷を突っついた判定にする
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
		CSound::GetInstance()->Play(CSound::LABEL_SE_BREAK_ICE);
	}

	// 氷探索の再帰関数
	FindIce(nNumBreakV, nNumBreakH, 0, pIcePeck, apIce, false);

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsCanPeck())
				continue;

			// 探索フラグの無効化
			DisableFind();

			// 壊れないブロックが行う信号解除
			DisableFromHardIce(i, j);
		}
	}

	// 探索フラグの無効化
	DisableFind();

	// 壊れるブロックをまとまりにする
	SummarizeIce(nNumBreakV, nNumBreakH);

	// 氷が壊れるフラグが立っていたら氷を壊す
	BreakIce();

	return true;
}

//=====================================================
// 突っつける氷かのチェック
//=====================================================
bool CIceManager::CanPeck(CIce* pIce, int nNumV, int nNumH)
{
	if (pIce == nullptr)
		return false;	// ヌルだったら突けない

	if (!pIce->IsCanPeck())
		return false;	// 突っつけないブロックなら突けない

	if (pIce->IsPeck())
		return false;	// 既に突っついていたら突けない

	// なにかしら乗ってたら突けない
	if (pIce->IsOnTopAnyObject())
		return false;

	return true;
}

//=====================================================
// 探索済みフラグの無効化
//=====================================================
void CIceManager::DisableFind(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			m_aGrid[i][j].pIce->EnableCanFind(true);
		}
	}
}

//=====================================================
// 周辺の氷の取得
//=====================================================
vector<CIce*> CIceManager::GetAroundIce(int nNumV, int nNumH)
{
	vector<CIce*> apIce(DIRECTION_MAX);

	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	// 右上
	aV[DIRECTION_RIGHTUP] = nNumV + 1;
	aV[DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0)
	{// 偶数の時
		// グリッド番号を保存
		aH[DIRECTION_RIGHTUP] = nNumH + 1;
		aH[DIRECTION_RIGHTDOWN] = nNumH + 1;

		// 左ななめ
		aH[DIRECTION_LEFTUP] = nNumH;
		aH[DIRECTION_LEFTDOWN] = nNumH;
	}
	else
	{// 奇数の時

		// グリッド番号を保存
		aH[DIRECTION_RIGHTUP] = nNumH;
		aH[DIRECTION_RIGHTDOWN] = nNumH;

		// 左ななめ
		aH[DIRECTION_LEFTUP] = nNumH - 1;
		aH[DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// 左側
	aV[DIRECTION_LEFTUP] = nNumV + 1;
	aV[DIRECTION_LEFTDOWN] = nNumV - 1;

	// 左右
	aV[DIRECTION_RIGHT] = nNumV;
	aV[DIRECTION_LEFT] = nNumV;
	aH[DIRECTION_RIGHT] = nNumH + 1;
	aH[DIRECTION_LEFT] = nNumH - 1;

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
	}

	return apIce;
}

//=====================================================
// 指定した氷と同じポインタをヌルにする
//=====================================================
void CIceManager::DeleteIce(CIce *pIce)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce == pIce)
				m_aGrid[i][j].pIce = nullptr;
		}
	}
}

//=====================================================
// 外に出さないようにする判定
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH, float fRate)
{
	if (nIdxV < 0 || nIdxV >= m_nNumGridVirtical ||
		nIdxH < 0 || nIdxH >= m_nNumGridHorizontal)
		return;

	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = m_aGrid[nIdxV][nIdxH].pos;

	universal::LimitDistCylinderInSide(WIDTH_GRID * fRate, pPos, posGrid);
}

//=====================================================
// 外に出さないようにする判定
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos, CIce *pIce, float fRate)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIce->GetPosition();

	universal::LimitDistCylinderInSide(WIDTH_GRID * fRate, pPos, posGrid);
}

//=====================================================
// 氷内に収める処理
//=====================================================
void CIceManager::LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (pPos == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	D3DXVECTOR3 posIce = m_aGrid[nNumV][nNumH].pIce->GetPosition();

	pPos->y = posIce.y;
}

//=====================================================
// 最も近い氷の取得
//=====================================================
CIce *CIceManager::GetNearestIce(D3DXVECTOR3 pos, int *pNumV, int *pNumH)
{
	float fDistMin = FLT_MAX;
	CIce *pIceNearest = nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsPeck())
				continue;

			CIce *pIce = m_aGrid[i][j].pIce;

			float fDiff = 0.0f;

			if (universal::DistCmpFlat(pos, pIce->GetPosition(), fDistMin, &fDiff))
			{
				fDistMin = fDiff;

				pIceNearest = pIce;

				if (pNumV != nullptr && pNumH != nullptr)
				{
					*pNumV = i;
					*pNumH = j;
				}
			}
		}
	}

	return pIceNearest;
}

//=====================================================
// 氷の探索
//=====================================================
bool CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand, vector<CIce*> apIceLast, bool bBreakLast)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return false;
	
	// 探索済みフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	if (!m_aGrid[nNumV][nNumH].pIce->IsPeck())
	{
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);
	}

#ifdef _DEBUG
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 再帰関数の深さをインクリメント
	nIdx++;

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	// 四方向氷がないか探索できない状態なら終了
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (!apIce[i]->IsCanPeck())
			continue;

		FindIce(aV[i], aH[i], nIdx, pIceStand, apIce, true);
	}

	return false;
}

//=====================================================
// 氷の追加
//=====================================================
void CIceManager::AddIce(CIce *pIce, D3DXVECTOR3 pos)
{
	int nIdxV = -1;
	int nIdxH = -1;
	D3DXVECTOR3 posIce = pIce->GetPosition();

	GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	if (nIdxV == -1 && nIdxH == -1)
	{
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, pIce->GetPosition());
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, pIce->GetPosition());
		pIce->Uninit();
		return;
	}

	bool bOk = SetIceInGrid(nIdxV, nIdxH, pIce);

	if (!bOk)
	{// グリッドに無かったら壊す
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, pIce->GetPosition());
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, pIce->GetPosition());
		DeleteIce(pIce);
		pIce->Uninit();
	}
}

//=====================================================
// 硬い氷から信号を出して、破壊信号を解除
//=====================================================
void CIceManager::DisableFromHardIce(int nNumV, int nNumH, bool bPeck)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	// 探索済みフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

	// 周辺グリッドの計算
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	vector<CIce*> apIce(DIRECTION_MAX);

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (bPeck)
		{// つっつきブロックを含めるかどうか
			if (apIce[i]->IsPeck())
				continue;
		}

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// プレイヤーから信号発射で破壊信号解除
//=====================================================
void CIceManager::DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck, vector<CIce*> apIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce->IsPeck())
		return;

	// 探索済みフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

#ifdef _DEBUG
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 周辺グリッドの計算
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	// 四方向氷がないか探索できない状態なら終了
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i] == pIcePeck)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// 破壊信号の解除
//=====================================================
void CIceManager::DisableBreak(int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	// 探索済みフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

#ifdef _DEBUG
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	// 四方向氷がないか探索できない状態なら終了
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// 氷をまとめる
//=====================================================
void CIceManager::SummarizeIce(int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		if (!apIce[i]->IsBreak())
			continue;

		if (!apIce[i]->IsCanPeck())
			continue;

		// 流氷システムの生成
		CFlowIce *pFlowIce = CFlowIce::Create();

		// 再帰関数で連鎖して氷を流氷システムに保存
		SaveFlowIce(aV[i], aH[i], pFlowIce);
	}
}

//=====================================================
// 氷を流氷に保存するシステム
//=====================================================
void CIceManager::SaveFlowIce(int nNumV, int nNumH, CFlowIce *pFlowIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	// 周辺のグリッド番号の計算
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	// 探索済みのフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	// 自身を流氷に追加
	pFlowIce->AddIceToArray(m_aGrid[nNumV][nNumH].pIce);

#ifdef _DEBUG
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 100, 120, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;
		
		if (!apIce[i]->IsBreak())
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		pFlowIce->AddIceToArray(apIce[i]);

		// 再帰関数で連鎖して氷を流氷システムに保存
		SaveFlowIce(aV[i], aH[i], pFlowIce);
	}
}

//=====================================================
// 氷の破壊
//=====================================================
bool CIceManager::BreakIce(void)
{
	bool bBreakAny = false;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (!m_aGrid[i][j].pIce->IsBreak())
				continue;

			m_aGrid[i][j].pIce->ChangeState(new CIceStateFlow);
			m_aGrid[i][j].pIce = nullptr;

			BreakPeck(i, j);

			bBreakAny = true;
		}
	}

	if (bBreakAny)
	{// カメラを揺らす
		CCamera *pCamera = CManager::GetCamera();
		
		if (pCamera != nullptr)
			pCamera->SetQuake(POW_CAMERAQUAKE_DEFAULT, POW_CAMERAQUAKE_DEFAULT, FRAME_CAMERAQUAKE_DEFAULT);
	}

	return bBreakAny;
}

//=====================================================
// 角の確認
//=====================================================
bool CIceManager::CheckCorner(int nNumV, int nNumH)
{
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumIce = 0;
	int nNumBreakIce = 0;

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		nNumIce++;

		if (it->IsBreak() || it->IsPeck())
			nNumBreakIce++;
	}

	if (nNumIce > 0)
	{
		if (nNumBreakIce == nNumIce)
			return false;
	}

	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pos, 300.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	return true;
}

//=====================================================
// 突っついた氷を沈める処理
//=====================================================
void CIceManager::BreakPeck(int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumIce = 0;
	int nNumPeck = 0;

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		nNumIce++;

		if (apIce[i]->IsPeck())
		{
			nNumPeck++;
			DeleteIce(apIce[i]);
			apIce[i]->EnableSink(true);

			// 沈みパーティクルの発生
			D3DXVECTOR3 posIce = apIce[i]->GetPosition();
			CParticle::Create(posIce, CParticle::TYPE::TYPE_BUBBLE_SINK);
		}
	}

	if (nNumIce == nNumPeck)
	{// くっついている氷が全てつっついたものだったら壊す
		if (m_aGrid[nNumV][nNumH].pIce != nullptr)
		{
			m_aGrid[nNumV][nNumH].pIce->Uninit();
			m_aGrid[nNumV][nNumH].pIce = nullptr;
		}
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CIceManager::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n氷の情報=====================");
	pDebugProc->Print("\n氷の総数[%d]", CIce::GetNumAll());

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	// 海流の向きを変更
	if (pKeyboard->GetTrigger(DIK_LEFT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		m_dirStreamNext = (COcean::E_Stream)((m_dirStreamNext + 1) % COcean::E_Stream::STREAM_MAX);	// 次の海流の向きにする
	}

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		m_dirStreamNext = (COcean::E_Stream)((m_dirStreamNext + COcean::E_Stream::STREAM_MAX - 1) % COcean::E_Stream::STREAM_MAX);	// 次の海流の向きにする
	}

	pDebugProc->Print("\n現在の海流の向き[%d]", m_dirStreamNext);

	//-------------------------------------
	// デバッグで氷をつつく処理
	//-------------------------------------
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pMouse == nullptr)
		return;

	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	std::map<CObject3D*, int> mapIcon;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			D3DXVECTOR3 posIce = m_aGrid[i][j].pIce->GetPosition();

			bool bHit = universal::CalcRaySphere(posNear, vecDiff, posIce, Grid::SIZE * 0.5f);

			if (!bHit)
				continue;

			debug::Effect3DShort(posIce, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

			if (pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
			{// クリックしたらパーツ番号の決定
				PeckIce(i, j);
			}
		}
	}
}

//=====================================================
// グリッド位置の取得
//=====================================================
D3DXVECTOR3 CIceManager::GetGridPosition(int *pNumV, int *pNumH)
{
	if (m_aGrid.empty())
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (*pNumV > (int)m_aGrid.size() - 1)
	{// 上から飛び出てた時の補正
		*pNumV = m_aGrid.size() - 1;
	}
	else if (*pNumV < 0)
	{// 下から飛び出た時の補正
		*pNumV = 0;
	}

	if (*pNumH > (int)m_aGrid[*pNumV].size() - 1)
	{// 右から飛び出てた時の補正
		*pNumH = m_aGrid[*pNumV].size() - 1;
	}
	else if (*pNumH < 0)
	{// 左から飛び出た時の補正
		*pNumH = 0;
	}

	return m_aGrid[*pNumV][*pNumH].pos;
}

//=====================================================
// グリッドオブジェクトの取得
//=====================================================
CIce* CIceManager::GetGridIce(int* pNumV, int* pNumH)
{
	if (m_aGrid.empty())
		return nullptr;

	if (*pNumV > (int)m_aGrid.size() - 1)
	{// 上から飛び出てた時の補正
		*pNumV = m_aGrid.size() - 1;
	}
	else if (*pNumV < 0)
	{// 下から飛び出た時の補正
		*pNumV = 0;
	}

	if (*pNumH > (int)m_aGrid[*pNumV].size() - 1)
	{// 右から飛び出てた時の補正
		*pNumH = m_aGrid[*pNumV].size() - 1;
	}
	else if (*pNumH < 0)
	{// 左から飛び出た時の補正
		*pNumH = 0;
	}

	return m_aGrid[*pNumV][*pNumH].pIce;
}

//=====================================================
// 浮いてる氷の取得
//=====================================================
vector<CIce*> CIceManager::GetFlows(void)
{
	vector<CIce*> apIceFlows;
	vector<CIce*> apIce = CIce::GetInstance();

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		if (!it->IsStop())	// 停止していなければ保存
			apIceFlows.push_back(it);
	}

	return apIceFlows;
}

//=====================================================
// 位置からグリッド番号を取得する処理
//=====================================================
bool CIceManager::GetIdxGridFromPosition(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH, float fRate)
{
	if (pIdxV == nullptr || pIdxH == nullptr)
		return false;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// 距離の計算
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			pos.y = posGrid.y;

			D3DXVECTOR3 vecDiff = posGrid - pos;

			float fDist = D3DXVec3Length(&vecDiff);

			if (fDist < WIDTH_GRID * fRate)
			{// 氷のサイズ分の半径より小さかったら乗ってる判定
				*pIdxV = i;
				*pIdxH = j;

				return true;
			}
		}
	}

	return false;
}

//=====================================================
// 氷内判定
//=====================================================
bool CIceManager::IsInIce(D3DXVECTOR3 pos, CIce *pIce, float fRate)
{
	if (pIce == nullptr)
		return false;

	// 距離の計算
	D3DXVECTOR3 posIce = pIce->GetPosition();

	D3DXVECTOR3 vecDiff = posIce - pos;
	vecDiff.y = 0.0f;

	float fDist = D3DXVec3Length(&vecDiff);

	if (fDist < WIDTH_GRID * fRate)
	{// 氷のサイズ分の半径より小さかったら乗ってる判定
		return true;
	}

	return false;
}

//=====================================================
// グリッドに氷を設定
//=====================================================
bool CIceManager::SetIceInGrid(int nNumV, int nNumH, CIce *pIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	if (m_aGrid.empty())
		return false;

	if (nNumV > (int)m_aGrid.size() - 1)
	{// 上から飛び出てた時の制限
		return false;
	}
	else if (nNumV < 0)
	{// 下から飛び出た時の制限
		return false;
	}

	if (nNumH > (int)m_aGrid[nNumH].size() - 1)
	{// 右から飛び出てた時の制限
		return false;
	}
	else if (nNumH < 0)
	{// 左から飛び出た時の制限
		return false;
	}

	if (m_aGrid[nNumV][nNumH].pIce == nullptr || m_aGrid[nNumV][nNumH].pIce == pIce)
	{
		m_aGrid[nNumV][nNumH].pIce = pIce;

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================
// 氷のグリッド番号を取得
//=====================================================
void CIceManager::GetIceIndex(CIce *pIce, int *pNumV, int *pNumH)
{
	if (pIce == nullptr || pNumV == nullptr || pNumV == nullptr)
		return;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (pIce == m_aGrid[i][j].pIce)
			{// 同じ氷のポインタだった場合、番号を保存して関数を終了
				*pNumV = i;
				*pNumH = j;
				return;
			}
		}
	}

	// どのポインタにも入らなかった場合は-1を返す
	*pNumV = -1;
	*pNumH = -1;
}

//=====================================================
// 左下の氷を取得
//=====================================================
CIce* CIceManager::GetLeftDownIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// 氷があったら番号を保存
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// なにか乗ってたらキャンセル

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// つっついてたらキャンセル

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// 左上の氷を取得
//=====================================================
CIce* CIceManager::GetLeftUpIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = m_nNumGridVirtical - 1; i >= 0; i--)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// 氷があったら番号を保存
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// なにか乗ってたらキャンセル

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// つっついてたらキャンセル

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// 右下の氷を取得
//=====================================================
CIce* CIceManager::GetRightDownIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = m_nNumGridHorizontal - 1; j >= 0; j--)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// 氷があったら番号を保存
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// なにか乗ってたらキャンセル

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// つっついてたらキャンセル

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// 右上の氷を取得
//=====================================================
CIce* CIceManager::GetRightUpIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = m_nNumGridVirtical - 1; i >= 0; i--)
	{
		for (int j = m_nNumGridHorizontal - 1; j >= 0; j--)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// 氷があったら番号を保存
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// なにか乗ってたらキャンセル

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// つっついてたらキャンセル

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// グリッド内かの判定
//=====================================================
bool CIceManager::IsInGrid(D3DXVECTOR3 pos, float fRate)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// 距離の計算
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			pos.y = posGrid.y;

			D3DXVECTOR3 vecDiff = posGrid - pos;

			float fDist = D3DXVec3Length(&vecDiff);

			if (fDist < WIDTH_GRID * fRate)
			{// 氷のサイズ分の半径より小さかったら乗ってる判定
				return true;
			}
		}
	}

	return false;
}

//=====================================================
// ランダムな氷の取得
//=====================================================
CIce* CIceManager::GetRandomIce(int *pNumV, int *pNumH)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return nullptr;

	// 止まってない氷を除外
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && ice->IsPeck(); });

	// サイズからランダムで氷を指定
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

	// 番号の保存
	if (pNumV != nullptr && pNumH != nullptr)
	{
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr != nullptr)
		{
			pIceMgr->GetIceIndex(pIce, pNumV, pNumH);
		}
	}

	return pIce;
}

//=====================================================
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}

//=====================================================
// 初期配置読み込み処理
//=====================================================
void CIceManager::Load(const char* pPath)
{
	bool bMulti = gameManager::IsMulti();	// マルチフラグ取得

	bool bLoad = false;
	int nGridV = 0;
	std::ifstream ifs(pPath);

	if (ifs.is_open())
	{
		std::string strLine;
		while (std::getline(ifs, strLine))
		{
			std::istringstream iss(strLine);
			std::string key;
			iss >> key;
			
			if (strLine.length() == 0)
				continue;	// 読み込んだ文字が空なら通らない

			if (key == "SETICE")
			{
				bLoad = true;
				continue;
			}

			if (key == "END_SETICE")
			{// 終了
				break;
			}

			// 配置読み込み
			if (bLoad)
			{// SETICE読み込んだ
				int nGridH = 0;
				for (int cnt = 0; cnt < (int)strLine.size(); cnt++)
				{
					char cData = strLine[cnt];
					if (cData != ' ')
					{// 何かしら数字がいる
						if (cData == '1')
						{// 通常氷
							CreateIce(nGridV, nGridH);
						}
						else if (cData == '2')
						{// 硬い氷
							if(bMulti)	// マルチ用の硬い氷
								CreateIce(nGridV, nGridH, CIce::E_Type::TYPE_HARDMULTI);
							else	// シングル用の硬い氷
								CreateIce(nGridV, nGridH, CIce::E_Type::TYPE_HARD);
						}
						nGridH++;
					}
				}
				nGridV++;	// 行カウント増やす
			}
		}
		ifs.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}
}

//=====================================================
// さざ波の属性を割り当てる処理
//=====================================================
void CIceManager::BindRippleElements(void)
{
	// 全氷のチェック
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// 左端の検出
			if (j == 0)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_RIGHT, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_RIGHT, m_aGrid[i][j - 1].pIce == nullptr);

			// 右端の検出
			if (j == m_nNumGridHorizontal - 1)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_LEFT, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_LEFT, m_aGrid[i][j + 1].pIce == nullptr);

			// 下端の検出
			if (i == 0)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_UP, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_UP, m_aGrid[i - 1][j].pIce == nullptr);

			// 上端の検出
			if (i == m_nNumGridVirtical - 1)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_DOWN, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_DOWN, m_aGrid[i + 1][j].pIce == nullptr);
		}
	}
}