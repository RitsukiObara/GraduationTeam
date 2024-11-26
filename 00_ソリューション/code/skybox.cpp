//*****************************************************
//
// スカイボックスの処理[skybox.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "skybox.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_ROT = 0.0003f;	// 回る速度
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CSkybox *CSkybox::s_pSkybox = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CSkybox::CSkybox(int nPriority) : CObjectX(nPriority), m_bFollowPlayer(false)
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
	BindModel(nIdx);

	EnableLighting(true);
	EnableFog(false);

	m_bFollowPlayer = true;

	s_pSkybox = this;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSkybox::Uninit(void)
{
	s_pSkybox = nullptr;

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

	Rotate(D3DXVECTOR3(0.0f, SPEED_ROT, 0.0f));
}

//=====================================================
// 描画処理
//=====================================================
void CSkybox::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 継承クラスの描画
	CObjectX::Draw();

	// ライティングを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
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