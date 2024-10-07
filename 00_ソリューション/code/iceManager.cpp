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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_GRID = 200.0f;	// グリッドのサイズ
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
	CIceManager *pIceManager = nullptr;

	if (pIceManager == nullptr)
	{
		pIceManager = new CIceManager;

		if (pIceManager != nullptr)
		{
			pIceManager->m_nNumGridVirtical = nNumV;
			pIceManager->m_nNumGridHorizontal = nNumH;
			pIceManager->Init();
		}
	}

	return pIceManager;
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
}

//=====================================================
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}