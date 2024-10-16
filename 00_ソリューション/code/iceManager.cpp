//*****************************************************
//
// スカイボックスの処理[IceManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "debugproc.h"
#include "particle.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// 六角形の割合X
const float RATE_HEX_Z = -0.1f;	// 六角形の割合Z
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority)
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
	CreateIce(3, 3);
	CreateIce(4, 3);
	CreateIce(5, 3);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(4, 6);
	CreateIce(6, 6);

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
			pos = { j * (Grid::SIZE - Grid::SIZE * RATE_HEX_X) - Grid::SIZE * m_nNumGridHorizontal * 0.5f,10.0f,i * (Grid::SIZE - Grid::SIZE * RATE_HEX_Z) * 0.67f - Grid::SIZE * m_nNumGridVirtical * 0.5f };

			// 縦で偶数列だったらずらす
			if (i % 2 == 0)
			{
				pos.x += (Grid::SIZE - Grid::SIZE * RATE_HEX_X) * 0.5f;
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

	// 氷の停止
	StopIce(pIce);

	return pIce;
}

//=====================================================
// 氷の停止
//=====================================================
void CIceManager::StopIce(CIce *pIce)
{
	if (pIce == nullptr)
		return;

	pIce->SetState(CIce::E_State::STATE_STOP);

	// グリッドに属性を割り振る
}

//=====================================================
// 氷をつつく
//=====================================================
void CIceManager::PeckIce(int nNumV, int nNumH, E_Direction direction)
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

	switch (direction)
	{
	/*case CIceManager::DIRECTION_UP:
		nNumV++;
		break;*/
	case CIceManager::DIRECTION_RIGHT:
		nNumBreakH++;
		break;
	/*case CIceManager::DIRECTION_DOWN:
		nNumV--;
		break;*/
	case CIceManager::DIRECTION_LEFT:
		nNumBreakH--;
		break;
	default:
		break;
	}

	// 氷を突っついた判定にする
	if (m_aGrid[nNumBreakV][nNumBreakH].pIce)
	{
		m_aGrid[nNumBreakV][nNumBreakH].pIce->EnablePeck(true);
	}

	// 氷探索の再帰関数
	FindIce(nNumBreakV, nNumBreakH, 0, pIceStand, apIce,false);

	// 探索フラグの無効化
	DisableFind();

	// プレイヤーから壊さないブロックの流れを出す
	DisableFromPlayer(nNumV, nNumH, m_aGrid[nNumBreakV][nNumBreakH].pIce, apIce);

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
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 再帰関数の深さをインクリメント
	nIdx++;

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	
	CalcAroundGrids(nNumV, nNumH, aV, aH);

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
// 周辺グリッドの計算
//=====================================================
void CIceManager::CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH)
{
	// 右上・右下
	aV[DIRECTION_RIGHTUP] = nNumV + 1;
	aV[DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0) {  // 偶数の時
		aH[DIRECTION_RIGHTUP] = nNumH + 1;
		aH[DIRECTION_RIGHTDOWN] = nNumH + 1;
		aH[DIRECTION_LEFTUP] = nNumH;
		aH[DIRECTION_LEFTDOWN] = nNumH;
	}
	else {  // 奇数の時
		aH[DIRECTION_RIGHTUP] = nNumH;
		aH[DIRECTION_RIGHTDOWN] = nNumH;
		aH[DIRECTION_LEFTUP] = nNumH - 1;
		aH[DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// 左側・左右
	aV[DIRECTION_LEFTUP] = nNumV + 1;
	aV[DIRECTION_LEFTDOWN] = nNumV - 1;
	aV[DIRECTION_RIGHT] = nNumV;
	aV[DIRECTION_LEFT] = nNumV;
	aH[DIRECTION_RIGHT] = nNumH + 1;
	aH[DIRECTION_LEFT] = nNumH - 1;
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
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 周辺グリッドの計算
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	CalcAroundGrids(nNumV, nNumH, aV, aH);

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
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// 周辺グリッドの計算
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	CalcAroundGrids(nNumV, nNumH, aV, aH);

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

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
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

			BreakPeck(i, j);

			m_aGrid[i][j].pIce->Uninit();
		}
	}

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsDeath())
				m_aGrid[i][j].pIce = nullptr;
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

	CEffect3D::Create(m_aGrid[nNumV][nNumH].pos, 300.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

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
		CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
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
			apIce[i]->Uninit();
		}
	}

	if (nNumIce == nNumPeck)
		m_aGrid[nNumV][nNumH].pIce->Uninit();
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

			CEffect3D::Create(m_aGrid[i][j].pos, 50.0f, 5, col);
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n氷の総数[%d]", CIce::GetNumAll());
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
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}