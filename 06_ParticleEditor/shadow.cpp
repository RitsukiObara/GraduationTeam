//*****************************************************
//
// 影の処理[shadow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shadow.h"
#include "manager.h"
#include "texture.h"

//=====================================================
// コンストラクタ
//=====================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CShadow::~CShadow()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShadow::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShadow::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CShadow::Draw(void)
{
	// 継承クラスの描画
	CObject3D::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float width, float height)
{
	CShadow *pShadow = NULL;

	if (pShadow == NULL)
	{
		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			pShadow->SetPosition(D3DXVECTOR3(pos));
			pShadow->SetSize(width, height);

			// 初期化
			pShadow->Init();

			// テクスチャの読込
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pShadow->SetIdxTexture(nIdx);

			pShadow->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		}
	}

	return pShadow;
}