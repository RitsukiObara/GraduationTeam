//*****************************************************
//
// 魚影の処理[fishshadow.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fishshadow.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const char* FISHSHADOW_LOGO_PATH = "data\\TEXTURE\\enemy\\Fish_shadow.png";	// クリアロゴのパス
	const float SIZE_INIT[CEnemy::TYPE::TYPE_MAX] = {70.0f , 200.0f};	// 初期のサイズ
	const D3DXCOLOR COL_INIT = { 0.0f,0.0f,0.0f,1.0f };	// 初期色
}

//====================================================
// コンストラクタ
//====================================================
CFishShadow::CFishShadow()
{

}

//====================================================
// デストラクタ
//====================================================
CFishShadow::~CFishShadow()
{

}

//====================================================
// 生成処理
//====================================================
CFishShadow* CFishShadow::Create(CEnemy::TYPE type)
{
	CFishShadow* pFishShadow = nullptr;

	pFishShadow = new CFishShadow;

	if (pFishShadow != nullptr)
	{
		pFishShadow->Init();

		// サイズ設定
		pFishShadow->SetSize(SIZE_INIT[type], SIZE_INIT[type]);
		pFishShadow->SetVtx();
	}

	return pFishShadow;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CFishShadow::Init(void)
{
	// 継承クラスの初期化
	CPolygon3D::Init();

	// テクスチャ設定
	int nIdxTexture = Texture::GetIdx(&FISHSHADOW_LOGO_PATH[0]);
	SetIdxTexture(nIdxTexture);

	// 前面に出す
	EnableZtest(true);

	// 色設定
	SetColor(COL_INIT);

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CFishShadow::Uninit(void)
{
	// 継承クラスの終了
	CPolygon3D::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CFishShadow::Update(void)
{
	// 継承クラスの更新
	CPolygon3D::Update();
}

//====================================================
// 描画処理
//====================================================
void CFishShadow::Draw(void)
{
	// 継承クラスの描画
	CPolygon3D::Draw();
}