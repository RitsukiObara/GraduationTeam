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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* SEALS_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// クリアロゴのパス
	const char* BEARS_LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";	// 失敗ロゴのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CFishShadow* CFishShadow::m_pFishShadow = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
CFishShadow::CFishShadow()
{
	m_FishShadow = FISHSHADOW_SEALS;
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
CFishShadow* CFishShadow::Create(void)
{
	if (m_pFishShadow == nullptr)
	{
		m_pFishShadow = new CFishShadow;

		if (m_pFishShadow != nullptr)
		{
			m_pFishShadow->Init();
		}
	}

	return m_pFishShadow;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CFishShadow::Init(void)
{
	//クリアの時
	if (m_FishShadow == FISHSHADOW_SEALS)
	{
		// クリアロゴの生成
		m_apFishShadow[FISHSHADOW_SEALS] = CObject3D::Create();

		if (m_apFishShadow[FISHSHADOW_SEALS] != nullptr)
		{
			m_apFishShadow[FISHSHADOW_SEALS]->SetScale(D3DXVECTOR3(300.0f, 100.0f, 100.0f));
			m_apFishShadow[FISHSHADOW_SEALS]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(SEALS_LOGO_PATH);
		}
	}

	//失敗の時
	else if (m_FishShadow == FISHSHADOW_BEARS)
	{
		// 失敗ロゴの生成
		m_apFishShadow[FISHSHADOW_BEARS] = CObject3D::Create();

		if (m_apFishShadow[FISHSHADOW_BEARS] != nullptr)
		{
			m_apFishShadow[FISHSHADOW_BEARS]->SetScale(D3DXVECTOR3(300.0f, 100.0f, 100.0f));
			m_apFishShadow[FISHSHADOW_BEARS]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(BEARS_LOGO_PATH);
		}
	}

	//m_aPosDest[0].x = RESULT_WIDTH;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CFishShadow::Uninit(void)
{
	for (int nCnt = 0; nCnt < FISHSHADOW_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apFishShadow[nCnt] != nullptr)
		{
			m_apFishShadow[nCnt]->Uninit();

			m_apFishShadow[nCnt] = nullptr;
		}
	}

	m_pFishShadow = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void CFishShadow::Update(void)
{
	// 状態管理
	ResultState();
}

//====================================================
// 魚影状態管理
//====================================================
void CFishShadow::ResultState(void)
{
	// アザラシの魚影処理
	FishShadowSeals();

	// しろくまの魚影処理
	FishShadowBears();
}

//====================================================
// アザラシの魚影処理
//====================================================
void CFishShadow::FishShadowSeals(void)
{
	// クリアの時
	if (m_FishShadow == FISHSHADOW_SEALS)
	{
		if (m_apFishShadow[FISHSHADOW_SEALS] != nullptr)
		{

		}
	}
}

//====================================================
// しろくまの魚影処理
//====================================================
void CFishShadow::FishShadowBears(void)
{
	// 失敗の時
	if (m_FishShadow == FISHSHADOW_BEARS)
	{
		if (m_apFishShadow[FISHSHADOW_BEARS] != nullptr)
		{

		}
	}
}

//====================================================
// 描画処理
//====================================================
void CFishShadow::Draw(void)
{

}