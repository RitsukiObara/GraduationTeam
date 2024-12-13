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
#include "UI.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{// UI配置の上から順に定義
	// 「オプション」のテキスト
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.1375f, 0.072f, 0.0f);
	const D3DXVECTOR2 OPTION_TEXT_SCALE = D3DXVECTOR2(0.129f, 0.057f);

	// 設定種類のテキスト
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Bibe.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.089f, 0.263f, 0.0f);	// BGMのテキストの位置
	const float PARAM_TEXT_POS_INTERVAL = 0.25f;							// 他のテキストの間隔（Y値のみ）
	const D3DXVECTOR2 PARAM_TEXT_SCALE = D3DXVECTOR2(0.086f, 0.075f);

	// BGM・SE
	namespace soundUI
	{
		// バー
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.422f;	// バーの位置（Xのみ・Y = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * 配置順）
		const float BAR_WIDTH = 0.375f;	// バーの長さ（バーの点で使う）
		const D3DXVECTOR2 BAR_SCALE = D3DXVECTOR2(0.188f, 0.064f);
		
		// バーの上にある点
		const string BARPOINT_TEX_PATH[] =
		{
			"data\\TEXTURE\\UI\\option_Bar_Select.png",		// 選択中
			"data\\TEXTURE\\UI\\option_Bar_UnSelect.png"	// 非選択
		};
		const D3DXVECTOR2 BARPOINT_SCALE = D3DXVECTOR2(0.018f, 0.036f);

		// ペンギン
		const string PENGUIN_TEX_PATH[COption::OPTIONPARAM::PARAM_MAX - 1] =
		{
			"data\\TEXTURE\\UI\\option_BGM_Icon.png",
			"data\\TEXTURE\\UI\\option_SE_Icon.png"
		};
		const float PENGUIN_POS_X = 0.881f;	// ペンギンの位置（Xのみ）
		const D3DXVECTOR2 PENGUIN_SCALE = D3DXVECTOR2(0.071f, 0.125f);
	}

	// 振動
	namespace bibeUI
	{
		const string ICON_TEX_PATH[COption::BIBESWITCH::BIBESWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Bibe_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Bibe_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.278f;	// アイコンの間隔（振動テキスト始点）
		const D3DXVECTOR2 ICON_SCALE = D3DXVECTOR2(0.071f, 0.125f);
	}

	// 戻るボタンUI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.12f, 0.92f, 0.0f);
	const D3DXVECTOR2 BACK_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);
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

	// UI配置
	CreateUIAll();

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

//=====================================================
// UI生成全体
//=====================================================
void COption::CreateUIAll(void)
{
	// 「オプション」のテキスト
	CreateSingleUI(&m_pOptionText, OPTION_TEXT_TEX_PATH, OPTION_TEXT_POS, OPTION_TEXT_SCALE.x, OPTION_TEXT_SCALE.y);

	// サウンド系UI
	CreateSoundUIObj();

	// 振動
	CreateBibeUIObj();

	// 戻るボタンUI
	CreateSingleUI(&m_pButtonUI, BACK_BUTTON_UI_TEX_PATH, BACK_BUTTON_UI_POS, BACK_BUTTON_UI_SCALE.x, BACK_BUTTON_UI_SCALE.y);
}

//=====================================================
// サウンド系UI生成
//=====================================================
void COption::CreateSoundUIObj(void)
{
	for (int cnt = 0; cnt < sizeof(m_aSoundUIObj) / sizeof(SoundUIObject); cnt++)
	{
		D3DXVECTOR3 pos;

		// 設定種類のテキスト
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].text, PARAM_TEXT_TEX_PATH[cnt], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

		// バー
		pos = D3DXVECTOR3(soundUI::BAR_POS_X, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].bar, soundUI::BAR_TEX_PATH, pos, soundUI::BAR_SCALE.x, soundUI::BAR_SCALE.y);

		// バーの点
		pos = D3DXVECTOR3(0.5f, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);	// X値仮
		CreateSingleUI(&m_aSoundUIObj[cnt].point, soundUI::BARPOINT_TEX_PATH[0], pos, soundUI::BARPOINT_SCALE.x, soundUI::BARPOINT_SCALE.y);

		// ペンギン
		pos = D3DXVECTOR3(soundUI::PENGUIN_POS_X, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].penguin, soundUI::PENGUIN_TEX_PATH[cnt], pos, soundUI::PENGUIN_SCALE.x, soundUI::PENGUIN_SCALE.y);
	}
}

//=====================================================
// 振動UI生成
//=====================================================
void COption::CreateBibeUIObj(void)
{
	D3DXVECTOR3 pos;

	// テキスト
	pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_BIBE), 0.0f);
	CreateSingleUI(&m_pBibeText, PARAM_TEXT_TEX_PATH[PARAM_BIBE], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

	// ON・OFFペンギンアイコン
	for (int cnt = 0; cnt < sizeof(m_apBibePenguin) / sizeof(CUI*); cnt++)
	{
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x + (bibeUI::ICON_POS_INTERVAL * (cnt + 1)), 
			PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_BIBE), 0.0f);
		CreateSingleUI(&m_apBibePenguin[cnt], bibeUI::ICON_TEX_PATH[cnt], pos, bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
	}
}

//=====================================================
// UI単体生成
//=====================================================
void COption::CreateSingleUI(CUI** ppUI, string path, D3DXVECTOR3 pos, float width, float height)
{
	*ppUI = CUI::Create();
	if (*ppUI != nullptr)
	{// 設定
		(*ppUI)->SetIdxTexture(CTexture::GetInstance()->Regist(&path[0]));	// テクスチャ割当
		(*ppUI)->SetPosition(pos);			// 位置
		(*ppUI)->SetSize(width, height);	// 大きさ
		(*ppUI)->SetVtx();	// 頂点反映
	}
}