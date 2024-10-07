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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_GRID = 200.0f;	// グリッドのサイズ
const float DIST_SPAWN_ICE = 200.0f;	// アイスのスポーン距離
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
CIce *CIceManager::CreateIce(int nGridV)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create();

	if (pIce == nullptr)
		return nullptr;

	D3DXVECTOR3 pos;
	pos = { -SIZE_GRID * m_nNumGridHorizontal * 0.5f - DIST_SPAWN_ICE,0.0f,nGridV * SIZE_GRID - SIZE_GRID * m_nNumGridVirtical * 0.5f };
	pIce->SetPosition(pos);

	return pIce;
}

//=====================================================
// 氷の停止
//=====================================================
void CIceManager::StopIce(CIce *pIce)
{
	pIce->SetState(CIce::E_State::STATE_STOP);

	// 今いるグリッドとその周辺の状態を設定

	// 真ん中

	// 端っこ
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
			pos = { j * SIZE_GRID - SIZE_GRID * m_nNumGridHorizontal * 0.5f,0.0f,i * SIZE_GRID - SIZE_GRID * m_nNumGridVirtical * 0.5f };
			D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

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