//*****************************************************
//
// スカイボックスの処理[skybox.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "skybox.h"
#include "manager.h"
#include "renderer.h"

//=====================================================
// コンストラクタ
//=====================================================
CSkybox::CSkybox(int nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CSkybox::~CSkybox()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSkybox::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// モデルの読込
	int nIdx = CModel::Load("data\\MODEL\\skybox\\sky00.x");
	SetIdxModel(nIdx);
	BindModel(nIdx);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSkybox::Uninit(void)
{
	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSkybox::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CSkybox::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pRenderer->IsFog())
	{
		// フォグを無効化
		//pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 継承クラスの描画
	CObjectX::Draw();

	// ライティングを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (pRenderer->IsFog())
	{
		// フォグを有効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
}

//=====================================================
// 生成処理
//=====================================================
CSkybox *CSkybox::Create()
{
	CSkybox *pSkybox = nullptr;

	if (pSkybox == nullptr)
	{
		pSkybox = new CSkybox;

		if (pSkybox != nullptr)
		{
			// 初期化
			pSkybox->Init();
		}
	}

	return pSkybox;
}
