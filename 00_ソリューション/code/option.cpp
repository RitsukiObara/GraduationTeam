//*****************************************************
//
// オプション処理[option.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "option.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{// UI配置の上から順に定義
	// 「オプション」のテキスト
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 設定種類のテキスト
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Bibe.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// BGMのテキストの位置
	const float PARAM_TEXT_POS_INTERVAL = 0.0f;							// 他のテキストの間隔（Y値のみ）

	// BGM・SE
	namespace soundUI
	{
		// バー
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.0f;	// バーの位置（Xのみ・Y = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * 配置順）
		const float BAR_WIDTH = 0.0f;	// バーの長さ（バーの点で使う）
		
		// バーの上にある点
		const string BARPOINT_TEX_PATH[] =
		{
			"data\\TEXTURE\\UI\\option_Bar_Select.png",		// 選択中
			"data\\TEXTURE\\UI\\option_Bar_UnSelect.png"	// 非選択
		};

		// ペンギン
		const string PENGUIN_TEX_PATH[COption::OPTIONPARAM::PARAM_MAX - 1] =
		{
			"data\\TEXTURE\\UI\\option_BGM_Icon.png",
			"data\\TEXTURE\\UI\\option_SE_Icon.png"
		};
		const float PENGUIN_POS_X = 0.0f;	// ペンギンの位置（Xのみ）
	}

	// 振動
	namespace bibeUI
	{
		const string ICON_TEX_PATH[COption::BIBESWITCH::BIBESWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Bibe_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Bibe_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.0f;	// アイコンの間隔（振動テキスト始点）
	}

	// 戻るボタンUI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// コンストラクタ
//=====================================================
COption::COption()
{
	m_pOptionText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_aSoundUIObj) / sizeof(SoundUIObject); cnt++)
	{
		m_aSoundUIObj[cnt].text = nullptr;
		m_aSoundUIObj[cnt].bar = nullptr;
		m_aSoundUIObj[cnt].point = nullptr;
		m_aSoundUIObj[cnt].penguin = nullptr;
	}
	m_pBibeText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_apBibePenguin) / sizeof(CUI*); cnt++) { m_apBibePenguin[cnt] = nullptr; }
	m_pButtonUI = nullptr;

	m_optionParam = PARAM_BGM;
	m_fBGMVolume = 0.0f;
	m_fSEVolume = 0.0f;
	m_Bibe = BIBE_ON;
}

//=====================================================
// デストラクタ
//=====================================================
COption::~COption()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT COption::Init(void)
{
	CScene::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void COption::Uninit(void)
{
	// オブジェクト全破棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void COption::Update(void)
{
	// シーンの更新
	CScene::Update();
}

//=====================================================
// 描画処理
//=====================================================
void COption::Draw(void)
{
	CScene::Draw();
}
