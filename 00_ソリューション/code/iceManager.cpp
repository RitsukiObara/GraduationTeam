//*****************************************************
//
// スカイボックスの処理[IceManager.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

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

	CreateIce(3, 6);
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
	CreateIce(5, 3);
	CreateIce(4, 6);
	CreateIce(6, 6);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CIceManager::Uninit(void)
{
	s_pIceManager = nullptr;

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
}

//=====================================================
// 氷の生成
//=====================================================
CIce *CIceManager::CreateIce(int nGridV, int nGridH)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create();

	if (pIce == nullptr)
		return nullptr;

	D3DXVECTOR3 pos;
	pos = { nGridH * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f ,10.0f,nGridV * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };
	pIce->SetPosition(pos);
	pIce->SetSize(Grid::SIZE * 0.5f, Grid::SIZE * 0.5f);

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

	// 今いるグリッドの計算
	D3DXVECTOR3 pos = pIce->GetPosition();

	AddIce(pIce, pos);

	// 今いるグリッドとその周辺の状態を設定

	// 真ん中

	// 端っこ
}

//=====================================================
// 氷をつつく
//=====================================================
void CIceManager::PeckIce(D3DXVECTOR3 pos, E_Direction direction)
{
	// 場所からグリッドを計算
	int nH = (int)(((pos.x + Grid::SIZE * m_nNumGridHorizontal * 0.5f) / Grid::SIZE * m_nNumGridHorizontal) * 0.1f);
	int nV = (int)(((pos.z + Grid::SIZE * m_nNumGridVirtical * 0.5f) / Grid::SIZE * m_nNumGridVirtical) * 0.1f);

	D3DXVECTOR3 posEffect;
	posEffect = { nH * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f,0.0f,nV * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };

	CEffect3D::Create(posEffect, 100.0f, 100, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// 今いる氷を見つけられないようにする
	m_aGrid[nV][nH].pIce->EnableCanFind(false);

	switch (direction)
	{
	case CIceManager::DIRECTION_UP:
		nV++;
		break;
	case CIceManager::DIRECTION_RIGHT:
		nH++;
		break;
	case CIceManager::DIRECTION_DOWN:
		nV--;
		break;
	case CIceManager::DIRECTION_LEFT:
		nH--;
		break;
	default:
		break;
	}

	FindIce(nV, nH);
}

//=====================================================
// 氷の探索
//=====================================================
void CIceManager::FindIce(int nNumV, int nNumH)
{
	// 探索済みフラグを立てる
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	vector<CIce*> apIce(DIRECTION_MAX);

	apIce[DIRECTION_UP] = m_aGrid[nNumV + 1][nNumH].pIce;
	apIce[DIRECTION_DOWN] = m_aGrid[nNumV - 1][nNumH].pIce;
	apIce[DIRECTION_RIGHT] = m_aGrid[nNumV][nNumH + 1].pIce;
	apIce[DIRECTION_LEFT] = m_aGrid[nNumV][nNumH - 1].pIce;
	
	// 四方向氷がないか探索できない状態なら終了
	bool bNothing = true;

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsCanFind() == false)
			continue;
		
		switch (i)
		{
		case CIceManager::DIRECTION_UP:
			FindIce(nNumV + 1, nNumH);
			break;
		case CIceManager::DIRECTION_RIGHT:
			FindIce(nNumV, nNumH + 1);
			break;
		case CIceManager::DIRECTION_DOWN:
			FindIce(nNumV - 1, nNumH);
			break;
		case CIceManager::DIRECTION_LEFT:
			FindIce(nNumV, nNumH - 1);
			break;
		default:
			break;
		}

		bNothing = false;
	}

	if (bNothing)
	{
		return;
	}
}

//=====================================================
// 氷の追加
//=====================================================
void CIceManager::AddIce(CIce *pIce, D3DXVECTOR3 pos)
{
	if (pIce == nullptr)
		return;

	// 場所からグリッドを計算
	int nH = (int)((pos.x + Grid::SIZE * m_nNumGridHorizontal * 0.5f) / Grid::SIZE * m_nNumGridHorizontal * 0.1f);
	int nV = (int)((pos.z + Grid::SIZE * m_nNumGridVirtical * 0.5f) / Grid::SIZE * m_nNumGridVirtical * 0.1f);

	m_aGrid[nV][nH].pIce = pIce;
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
			// 今のグリッド
			m_aGrid[i][j];

			D3DXVECTOR3 pos;
			pos = { j * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f,0.0f,i * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };
			D3DXCOLOR col = { (float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

			if (m_aGrid[i][j].pIce != nullptr)
			{
				if (m_aGrid[i][j].pIce->IsCanFind() == false)
				{
					col = { 1.0f,0.0f,0.0f,1.0f };
				}
			}

			CEffect3D::Create(pos, 50.0f, 5, col);
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n氷の総数[%d]", CIce::GetNumAll());
}

//=====================================================
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}