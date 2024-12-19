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
#include "manager.h"
#include "inputManager.h"
#include "inputjoypad.h"
#include "sound.h"
#include "camera.h"
#include "cameraState.h"
#include "fade.h"
#include "UI.h"
#include "texture.h"
#include "ocean.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{// UI配置の上から順に定義
	// 「オプション」のテキスト
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.1375f, 0.072f, 0.0f);
	const D3DXVECTOR2 OPTION_TEXT_SCALE = D3DXVECTOR2(0.129f, 0.057f);
	const float OPTION_TEXT_SCALING_SPEED = 0.01f * D3DX_PI;	// 拡縮速度（テキスト）
	const float OPTION_TEXT_SCALING_RANGE = 0.25f;				// 拡縮率（テキスト）
	const float SELECTPARAM_SCALING_SPEED = 0.01f * D3DX_PI;	// 拡縮速度（選択項目）
	const float SELECTPARAM_SCALING_RANGE = 0.22f;				// 拡縮率（選択項目）
	const D3DXCOLOR NOSELECT_COLOR = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.8f);	// 非選択時の色

	// 設定種類のテキスト
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Vibration.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.139f, 0.263f, 0.0f);	// BGMのテキストの位置
	const float PARAM_TEXT_POS_INTERVAL = 0.25f;							// 他のテキストの間隔（Y値のみ）
	const D3DXVECTOR2 PARAM_TEXT_SCALE = D3DXVECTOR2(0.086f, 0.075f);		// パラメータテキストのスケール

	// BGM・SE
	namespace soundUI
	{
		// バー
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.522f;	// バーの位置（Xのみ・Y = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * 配置順）
		const D3DXVECTOR2 BAR_SCALE = D3DXVECTOR2(0.234f, 0.064f);
		const float BAR_LENGTH = 0.23f;
		
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
		const string ICON_TEX_PATH[COption::VIBRATIONSWITCH::VIBRATIONSWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Vibration_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Vibration_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.278f;	// アイコンの間隔（振動テキスト始点）
		const D3DXVECTOR2 ICON_SCALE = D3DXVECTOR2(0.071f, 0.125f);
		const float POW_VIB = 0.6f;				// 振動の強さ
		const int TIME_VIB = 20;				// 振動の長さ
	}

	// 戻るボタンUI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.12f, 0.92f, 0.0f);
	const D3DXVECTOR2 BACK_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);

	// 選択ボタンUI
	const string SELECT_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\A_Select.png";
	const string NOWSETTING_SELECT_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\A_Enter.png";
	const D3DXVECTOR3 SELECT_BUTTON_UI_POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
	const D3DXVECTOR2 SELECT_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);

	// 設定保存先
	const string SETTING_SAVE_PATH = "data\\SETTING\\config.bin";
}

//---------------------------------------------------------
// 静的メンバ
//---------------------------------------------------------
float COption::m_fBGMVolume = 1.0f;								// BGMボリューム
float COption::m_fSEVolume = 1.0f;								// SEボリューム
COption::VIBRATIONSWITCH COption::m_Vibration = VIBRATION_ON;	// バイブ有無

//---------------------------------------------------------
// 関数ポインタ
//---------------------------------------------------------
COption::SettingFunc COption::s_settingFunc[PARAM_MAX] =
{
	&COption::SettingBGM,
	&COption::SettingSE,
	&COption::SettingVibration
};

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
	m_pVibrationText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_apVibrationPenguin) / sizeof(CUI*); cnt++) { m_apVibrationPenguin[cnt] = nullptr; }
	m_pBackButtonUI = nullptr;
	m_pAButtonUI = nullptr;

	m_optionParam = PARAM_BGM;
	m_fOptionTextScale = 0.0f;
	m_fParamScale = 0.0f;
	m_bSetting = false;
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

	// 入力マネージャー生成
	CInputManager::Create();

	// UI配置
	CreateUIAll();

	// パラメータ設定
	CSound* pSound = CSound::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad != nullptr)
	{// 設定
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume);
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));
	}

	// 背景海生成
	COcean::Create();

	// カメラ位置の設定
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	if (pInfoCamera != nullptr)
	{
		pInfoCamera->posV = { 45.38f,84.71f,270.10f };
		pInfoCamera->posR = { 0.0f,0.0f,0.0f };
		pInfoCamera->fLength = 1500.0f;
	}

	Camera::ChangeState(new CCameraStateOptionMode);

	if(pSound != nullptr)
		pSound->Play(CSound::LABEL_BGM_TITLE);

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

	// 入力処理
	Input();

	// 拡縮処理
	MoveSelect();
	MoveBGM();
	MoveSE();
	MoveVibration();
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
	CreateVibrationUIObj();

	// ボタンUI
	CreateSingleUI(&m_pBackButtonUI, BACK_BUTTON_UI_TEX_PATH, BACK_BUTTON_UI_POS, BACK_BUTTON_UI_SCALE.x, BACK_BUTTON_UI_SCALE.y);
	CreateSingleUI(&m_pAButtonUI, SELECT_BUTTON_UI_TEX_PATH, SELECT_BUTTON_UI_POS, SELECT_BUTTON_UI_SCALE.x, SELECT_BUTTON_UI_SCALE.y);

	// UI設定
	SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume);
	SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume);
	ColChangeVibration();
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
// 入力処理系
//=====================================================
void COption::Input(void)
{
	if (m_bSetting)
	{// 調整中
		(this->*s_settingFunc[m_optionParam])();
		BackSelect();
	}
	else
	{// 選択中
		// 項目選択
		Select();

		// 戻る処理
		ModeSelectBack();
	}
}

//=====================================================
// モードセレクトに戻る処理
//=====================================================
void COption::ModeSelectBack(void)
{
	// フェードの開始
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_BACK))	// BACK押したとき
	{
		pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
	}
}

//=====================================================
// 設定項目選択
//=====================================================
void COption::Select(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	if (pInputMgr == nullptr)
		return;

	// 移動
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{// 上移動
		m_optionParam = (OPTIONPARAM)(((int)m_optionParam - 1 + PARAM_MAX) % PARAM_MAX);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{// 下移動
		m_optionParam = (OPTIONPARAM)(((int)m_optionParam + 1) % PARAM_MAX);
	}

	// 決定
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER))
	{// 決定
		m_bSetting = true;

		// AボタンUIのテクスチャ設定
		if (m_pAButtonUI != nullptr)
		{// テクスチャ割当
			m_pAButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&NOWSETTING_SELECT_BUTTON_UI_TEX_PATH[0]));
		}

		// 戻るボタンUIを透明化
		if (m_pBackButtonUI != nullptr)
		{
			m_pBackButtonUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
}

//=====================================================
// 設定項目動かす
//=====================================================
void COption::MoveSelect(void)
{
	CUI* apUI[PARAM_MAX] = { m_aSoundUIObj[0].text,m_aSoundUIObj[1].text,m_pVibrationText };
	for (int cnt = 0; cnt < PARAM_MAX; cnt++)
	{
		if (cnt == m_optionParam)
		{// 選択中
			// サイズ設定
			D3DXVECTOR2 scale = PARAM_TEXT_SCALE * ScaleChange(&m_fOptionTextScale, OPTION_TEXT_SCALING_SPEED, OPTION_TEXT_SCALING_RANGE);
			apUI[cnt]->SetSize(scale.x, scale.y);
			apUI[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			apUI[cnt]->SetVtx();
		}
		else
		{// 非選択
			if (m_bSetting == true)
			{// 他の項目が設定中なら灰色にする
				apUI[cnt]->SetCol(NOSELECT_COLOR);
			}
			else
			{// 何も設定していなければ通常の色
				apUI[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			apUI[cnt]->SetSize(PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);	// サイズは通常（設定中関係なく）
			apUI[cnt]->SetVtx();
		}
	}
}

//=====================================================
// 振動UI生成
//=====================================================
void COption::CreateVibrationUIObj(void)
{
	D3DXVECTOR3 pos;

	// テキスト
	pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_VIBRATION), 0.0f);
	CreateSingleUI(&m_pVibrationText, PARAM_TEXT_TEX_PATH[PARAM_VIBRATION], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

	// ON・OFFペンギンアイコン
	for (int cnt = 0; cnt < sizeof(m_apVibrationPenguin) / sizeof(CUI*); cnt++)
	{
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x + (bibeUI::ICON_POS_INTERVAL * (cnt + 1)),
			PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_VIBRATION), 0.0f);
		CreateSingleUI(&m_apVibrationPenguin[cnt], bibeUI::ICON_TEX_PATH[cnt], pos, bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
	}
}

//=====================================================
// BGM設定
//=====================================================
void COption::SettingBGM(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	if (pInputMgr == nullptr || pSound == nullptr)
		return;

	// 移動
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// 左移動
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::LABEL_BGM_TITLE, 1.0f);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// 右移動
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::LABEL_BGM_TITLE, 1.0f);
	}
}

//=====================================================
// BGMのUI動かす
//=====================================================
void COption::MoveBGM(void)
{
	// サイズ計算
	D3DXVECTOR2 scale = soundUI::BARPOINT_SCALE;
	if (m_bSetting && m_optionParam == PARAM_BGM)
	{
		scale *= ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
	}

	// UI設定
	if (m_aSoundUIObj[PARAM_BGM].point != nullptr)
	{
		m_aSoundUIObj[PARAM_BGM].point->SetSize(scale.x, scale.y);
		m_aSoundUIObj[PARAM_BGM].point->SetVtx();
	}
}

//=====================================================
// SE設定
//=====================================================
void COption::SettingSE(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	if (pInputMgr == nullptr || pSound == nullptr)
		return;

	// 移動
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// 左移動
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pSound->Play(CSound::LABEL_SE_NOTPECK);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// 右移動
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pSound->Play(CSound::LABEL_SE_NOTPECK);
	}
}

//=====================================================
// SEのUI動かす
//=====================================================
void COption::MoveSE(void)
{
	// サイズ計算
	D3DXVECTOR2 scale = soundUI::BARPOINT_SCALE;
	if (m_bSetting && m_optionParam == PARAM_SE)
	{
		scale *= ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
	}

	// UI設定
	if (m_aSoundUIObj[PARAM_SE].point != nullptr)
	{
		m_aSoundUIObj[PARAM_SE].point->SetSize(scale.x, scale.y);
		m_aSoundUIObj[PARAM_SE].point->SetVtx();
	}
}

//=====================================================
// サウンド系の値とUI設定
//=====================================================
void COption::SettingSound(CUI* pUI, float* volumeBase, float volumeAfter)
{
	// 数値設定
	*volumeBase = volumeAfter;
	universal::LimitValuefloat(volumeBase, 1.0f, 0.0f);

	// UI位置計算
	float pointPosLeft = soundUI::BAR_LENGTH * *volumeBase * 2.0f;	// バーの左からの距離
	float pointPos = soundUI::BAR_POS_X - (soundUI::BAR_LENGTH) + pointPosLeft;	// 実際の位置

	// UI設定
	if (pUI != nullptr)
	{
		D3DXVECTOR3 pos = pUI->GetPosition();
		pos.x = pointPos;
		pUI->SetPosition(pos);
		pUI->SetVtx();
	}
}

//=====================================================
// 振動設定
//=====================================================
void COption::SettingVibration(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	if (pInputMgr == nullptr || pJoypad == nullptr)
		return;

	// 移動
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// 左移動
		m_Vibration = (VIBRATIONSWITCH)(((int)m_Vibration - 1 + VIBRATIONSWITCH_MAX) % VIBRATIONSWITCH_MAX);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));

		if (m_Vibration == VIBRATION_ON)
			pJoypad->Vibration(0,bibeUI::POW_VIB, bibeUI::TIME_VIB);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// 右移動
		m_Vibration = (VIBRATIONSWITCH)(((int)m_Vibration + 1) % VIBRATIONSWITCH_MAX);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));

		if (m_Vibration == VIBRATION_ON)
			pJoypad->Vibration(0,bibeUI::POW_VIB, bibeUI::TIME_VIB);
	}

	// 色変更
	ColChangeVibration();
}

//=====================================================
// 振動のUI動かす
//=====================================================
void COption::MoveVibration(void)
{
	for (int cnt = 0; cnt < VIBRATIONSWITCH_MAX; cnt++)
	{
		if (m_bSetting && m_optionParam == PARAM_VIBRATION && cnt == m_Vibration)
		{// 選択中
			// サイズ設定
			D3DXVECTOR2 scale = bibeUI::ICON_SCALE * ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
			m_apVibrationPenguin[cnt]->SetSize(scale.x, scale.y);
		}
		else
		{// 非選択
			m_apVibrationPenguin[cnt]->SetSize(bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
		}
		m_apVibrationPenguin[cnt]->SetVtx();
	}
}

//=====================================================
// 振動の色変更
//=====================================================
void COption::ColChangeVibration(void)
{
	for (int cnt = 0; cnt < VIBRATIONSWITCH_MAX; cnt++)
	{
		if (cnt == m_Vibration)
		{// 選択中
			m_apVibrationPenguin[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// 非選択
			m_apVibrationPenguin[cnt]->SetCol(NOSELECT_COLOR);
		}
		m_apVibrationPenguin[cnt]->SetVtx();
	}
}

//=====================================================
// 選択状態に戻る
//=====================================================
void COption::BackSelect(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	if (pInputMgr == nullptr)
		return;

	// 戻る
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER))
	{// 戻る
		m_bSetting = false;

		// AボタンUIのテクスチャ設定
		if (m_pAButtonUI != nullptr)
		{// テクスチャ割当
			m_pAButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&SELECT_BUTTON_UI_TEX_PATH[0]));
		}

		// 戻るボタンUIを不透明化
		if (m_pBackButtonUI != nullptr)
		{
			m_pBackButtonUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 設定を外部ファイルに保存
		SaveSetting();
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

//=====================================================
// UIサイズ変更（sin波）
//=====================================================
float COption::ScaleChange(float* angle, float speed, float range)
{
	// 角度変更
	*angle += speed;
	if (*angle >= 2.0f * D3DX_PI)
		*angle -= 2.0f * D3DX_PI;

	// 結果返す
	return sinf(*angle) * range + 1.0f;
}

//=====================================================
// 設定保存
//=====================================================
void COption::SaveSetting(void)
{
	// ファイルを開く
	std::ofstream outputFile(SETTING_SAVE_PATH, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// データの保存
	outputFile.write(reinterpret_cast<char*>(&m_fBGMVolume), sizeof(float));
	outputFile.write(reinterpret_cast<char*>(&m_fSEVolume), sizeof(float));
	outputFile.write(reinterpret_cast<char*>(&m_Vibration), sizeof(VIBRATIONSWITCH));

	outputFile.close();
}

//=====================================================
// 設定読み込み
//=====================================================
void COption::LoadSetting(void)
{
	// ファイルを開く
	std::ifstream inputFile(SETTING_SAVE_PATH, std::ios::binary);

	if (inputFile.is_open())
	{// ファイルがある（読み込み
		inputFile.read(reinterpret_cast<char*>(&m_fBGMVolume), sizeof(float));
		inputFile.read(reinterpret_cast<char*>(&m_fSEVolume), sizeof(float));
		inputFile.read(reinterpret_cast<char*>(&m_Vibration), sizeof(VIBRATIONSWITCH));

		inputFile.close();
	}
	else
	{// ファイルがない（初期設定）
		m_fBGMVolume = 1.0f;
		m_fSEVolume = 1.0f;
		m_Vibration = VIBRATION_ON;
		SaveSetting();	// ファイル生成
	}

	// 設定
	CSound* pSound = CSound::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	if (pSound != nullptr && pJoypad != nullptr)
	{// 設定
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));
	}
}