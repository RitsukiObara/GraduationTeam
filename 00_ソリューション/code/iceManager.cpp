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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// 六角形の割合X
const float RATE_HEX_Z = 0.13f;	// 六角形の割合Z

const float WIDTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_X;	// グリッドの幅
const float DEPTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_Z;	// グリッドの奥行き
const float OCEAN_FLOW_MIN = 1.00f;		// 海流の速度最小
const float OCEAN_FLOW_MAX = 5.00f;	// 海流の速度最大

const float RANGE_SELECT_ICE = D3DX_PI / 6;	// 氷を選択するときの角度の範囲
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority), m_nNumGridVirtical(0), m_nNumGridHorizontal(0), m_dirStream(E_Stream::STREAM_UP)
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

	// 仮マップ生成
	CreateIce(3, 6,CIce::E_Type::TYPE_HARD);
	CreateIce(3, 5);
	CreateIce(3, 4);
	/*CreateIce(3, 3);
	CreateIce(4, 3);
	CreateIce(5, 3);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(4, 6);
	CreateIce(6, 6);
	CreateIce(6, 7);
	CreateIce(6, 8);
	CreateIce(6, 9);*/

	// 海流を初期化
	m_dirStream = E_Stream::STREAM_LEFT;
	m_dirStreamNext = E_Stream::STREAM_LEFT;
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

#ifdef _DEBUG
			//CPolygon3D *pPolygon = CPolygon3D::Create(pos);
			
			//pPolygon->SetMode(CPolygon3D::MODE::MODE_BILLBOARD);
			//pPolygon->SetSize(WIDTH_GRID * 0.2f, WIDTH_GRID * 0.2f);
			//pPolygon->SetVtx();
#endif
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
#ifdef _DEBUG
	Debug();
#endif

	// 氷の状態管理
	ManageStateIce();
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
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// つっついた氷に挟まれてるかの判定
			if (JudgeBetweenPeck(i, j))
				continue;

			m_aGrid[i][j].pIce->EnableBreak(false);
			m_aGrid[i][j].pIce->EnableCanFind(true);
			m_aGrid[i][j].pIce->EnableAliveStandIce(false);
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
	{
		pos.x += WIDTH_GRID * 0.5f;
	}

	// 氷のトランスフォーム設定
	pIce->SetPosition(pos);
	pIce->SetTransform(Grid::SIZE);

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
// 氷をつつく
//=====================================================
void CIceManager::PeckIce(int nNumV, int nNumH, float fRot,D3DXVECTOR3 pos)
{
	if (m_aGrid[nNumV][nNumH].pIce != nullptr)
	{
		// 今いる氷を見つけられないようにする
		m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	}

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	CIce* pIcePeck = nullptr;

	// 向きに合わせて氷を選択
	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		// 氷とスティック角度の比較
		D3DXVECTOR3 posIce = it->GetPosition();
		bool bSelect = universal::IsInFanTargetYFlat(pos, posIce, fRot, RANGE_SELECT_ICE);

		if (bSelect)
		{// 氷が選べたらfor文を終了
			pIcePeck = it;
			break;
		}
	}

	// 番号を取得
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// 氷がつっつける状態かのチェック
	if (pIcePeck == nullptr)
		return;

	if (!pIcePeck->IsCanPeck())
		return;	// 突っつけないブロックなら後の処理を通らない

	if (pIcePeck->IsPeck())
		return;	// 既に突っついていたら通らない

	// 氷を突っついた判定にする
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
	}

	// 氷探索の再帰関数
	FindIce(nNumBreakV, nNumBreakH, 0, pIceStand, apIce,false);

	// 探索フラグの無効化
	DisableFind();

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
			DisableFromHardIce(i, j, apIce);
		}
	}

	// 探索フラグの無効化
	DisableFind();

	// プレイヤーから壊さないブロックの流れを出す
	DisableFromPlayer(nNumV, nNumH, pIcePeck, apIce);

	// 探索フラグの無効化
	DisableFind();

	// 壊れるブロックをまとまりにする
	SummarizeIce(nNumBreakV, nNumBreakH);

	// 氷が壊れるフラグが立っていたら氷を壊す
	BreakIce();
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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
void CIceManager::Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = m_aGrid[nIdxV][nIdxH].pos;

	universal::LimitDistCylinderInSide(WIDTH_GRID * 0.7f, pPos, posGrid);
}

//=====================================================
// 外に出さないようにする判定
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos, CIce *pIce)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIce->GetPosition();

	universal::LimitDistCylinderInSide(WIDTH_GRID * 0.7f, pPos, posGrid);
}

//=====================================================
// 氷内に収める処理
//=====================================================
void CIceManager::LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH)
{
	if (pPos == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	D3DXVECTOR3 posIce = m_aGrid[nNumV][nNumH].pIce->GetPosition();

	pPos->y = posIce.y;

	//if (pPos->x >= posIce.x + WIDTH_GRID * 0.5f)
	//	pPos->x = posIce.x + WIDTH_GRID * 0.5f;	// 右側の補正
	//else if (pPos->x <= posIce.x - WIDTH_GRID * 0.5f)
	//	pPos->x = posIce.x - WIDTH_GRID * 0.5f;	// 左側の補正

	//if (pPos->z >= posIce.z + DEPTH_GRID * 0.5f)
	//	pPos->z = posIce.z + DEPTH_GRID * 0.5f;	// 奥側の補正
	//else if (pPos->z <= posIce.z - DEPTH_GRID * 0.5f)
	//	pPos->z = posIce.z - DEPTH_GRID * 0.5f;	// 手前側の補正
}

//=====================================================
// 氷の探索
//=====================================================
bool CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand, vector<CIce*> apIceLast, bool bBreakLast)
{
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsCanFind() == false)
			continue;

		FindIce(aV[i], aH[i], nIdx, pIceStand, apIce, true);
	}

	return false;
}

//=====================================================
// 立っているブロックの確認
//=====================================================
bool CIceManager::CheckStandBlock(vector<CIce*> apIce, CIce *pIceStand, int nIdx)
{
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if ((apIce[i] == pIceStand || apIce[i]->IsAliveStandIce()) && nIdx != 1)
		{
			return true;
		}
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
		pIce->Uninit();
		return;
	}

	bool bOk = SetIceInGrid(nIdxV, nIdxH, pIce);

	if (!bOk)
	{
		DeleteIce(pIce);
		pIce->Uninit();
	}
}

//=====================================================
// 硬い氷から信号を出して、破壊信号を解除
//=====================================================
void CIceManager::DisableFromHardIce(int nNumV, int nNumH, vector<CIce*> apIce)
{
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

	// 氷のポインタの保存
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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
// プレイヤーから信号発射で破壊信号解除
//=====================================================
void CIceManager::DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck, vector<CIce*> apIce)
{
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
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
void CIceManager::BreakIce(void)
{
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
		}
	}
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
// 共通氷の確認
//=====================================================
bool CIceManager::CheckCommon(vector<CIce*> apIce, vector<CIce*> apIceLast, CIce* pIceStand, int nNumV, int nNumH,bool bBreakLast)
{
	bool bBreak = true;

	for (int i = 0; i < (int)apIceLast.size(); i++)
	{// 前の氷と共通の氷を見ているかのチェックを行う
		if (apIceLast[i] == nullptr)
			continue;

		for (int j = 0; j < (int)apIce.size(); j++)
		{
			if (apIce[j] == nullptr)
				continue;

			if (apIce[j] != apIceLast[i])
				continue;	// 同じポインタかどうか

			if (apIce[j] == pIceStand)
			{// 立っているブロックに当たったら強制的に信号を途絶
				m_aGrid[nNumV][nNumH].pIce->EnableAliveStandIce(true);

				return false;
			}

			// 一つでも壊れないブロックがあったら破壊しない判定
			if (!apIceLast[i]->IsPeck() && !apIceLast[i]->IsBreak())
				bBreak = false;
		}
	}

#ifdef _DEBUG
	if (!bBreak)
	{
		//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
#endif

	return bBreak;
}

//=====================================================
// 突っついた氷を沈める処理
//=====================================================
void CIceManager::BreakPeck(int nNumV, int nNumH)
{
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
	// グリッドの場所をデバッグ表示
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			D3DXCOLOR col = { (float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

			if (m_aGrid[i][j].pIce != nullptr)
			{
				if (m_aGrid[i][j].pIce->IsBreak())
				{
					col = { 0.0f,0.0f,1.0f,1.0f };
				}

				if (m_aGrid[i][j].pIce->IsPeck())
				{
					col = { 0.0f,1.0f,0.0f,1.0f };
				}
			}
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n氷の総数[%d]", CIce::GetNumAll());

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	// 海流の向きを変更
	if (pKeyboard->GetTrigger(DIK_LEFT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		m_dirStreamNext = (E_Stream)((m_dirStreamNext + 1) % E_Stream::STREAM_MAX);	// 次の海流の向きにする
	}

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		m_dirStreamNext = (E_Stream)((m_dirStreamNext + E_Stream::STREAM_MAX - 1) % E_Stream::STREAM_MAX);	// 次の海流の向きにする
	}

	pDebugProc->Print("\n現在の海流の向き[%d]", m_dirStreamNext);
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

	pos.y = 10.0f;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// 距離の計算
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

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
// 右下の氷を取得
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
				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}