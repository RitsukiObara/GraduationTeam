//*****************************************************
//
// �I�v�V��������[option.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �萔��`
//*****************************************************
namespace
{// UI�z�u�̏ォ�珇�ɒ�`
	// �u�I�v�V�����v�̃e�L�X�g
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.1375f, 0.072f, 0.0f);
	const D3DXVECTOR2 OPTION_TEXT_SCALE = D3DXVECTOR2(0.129f, 0.057f);
	const float OPTION_TEXT_SCALING_SPEED = 0.01f * D3DX_PI;	// �g�k���x�i�e�L�X�g�j
	const float OPTION_TEXT_SCALING_RANGE = 0.25f;				// �g�k���i�e�L�X�g�j
	const float SELECTPARAM_SCALING_SPEED = 0.01f * D3DX_PI;	// �g�k���x�i�I�����ځj
	const float SELECTPARAM_SCALING_RANGE = 0.22f;				// �g�k���i�I�����ځj
	const D3DXCOLOR NOSELECT_COLOR = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.8f);	// ��I�����̐F

	// �ݒ��ނ̃e�L�X�g
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Vibration.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.139f, 0.263f, 0.0f);	// BGM�̃e�L�X�g�̈ʒu
	const float PARAM_TEXT_POS_INTERVAL = 0.25f;							// ���̃e�L�X�g�̊Ԋu�iY�l�̂݁j
	const D3DXVECTOR2 PARAM_TEXT_SCALE = D3DXVECTOR2(0.086f, 0.075f);		// �p�����[�^�e�L�X�g�̃X�P�[��

	// BGM�ESE
	namespace soundUI
	{
		// �o�[
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.522f;	// �o�[�̈ʒu�iX�̂݁EY = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * �z�u���j
		const D3DXVECTOR2 BAR_SCALE = D3DXVECTOR2(0.234f, 0.064f);
		const float BAR_LENGTH = 0.23f;
		
		// �o�[�̏�ɂ���_
		const string BARPOINT_TEX_PATH[] =
		{
			"data\\TEXTURE\\UI\\option_Bar_Select.png",		// �I��
			"data\\TEXTURE\\UI\\option_Bar_UnSelect.png"	// ��I��
		};
		const D3DXVECTOR2 BARPOINT_SCALE = D3DXVECTOR2(0.018f, 0.036f);

		// �y���M��
		const string PENGUIN_TEX_PATH[COption::OPTIONPARAM::PARAM_MAX - 1] =
		{
			"data\\TEXTURE\\UI\\option_BGM_Icon.png",
			"data\\TEXTURE\\UI\\option_SE_Icon.png"
		};
		const float PENGUIN_POS_X = 0.881f;	// �y���M���̈ʒu�iX�̂݁j
		const D3DXVECTOR2 PENGUIN_SCALE = D3DXVECTOR2(0.071f, 0.125f);
	}

	// �U��
	namespace bibeUI
	{
		const string ICON_TEX_PATH[COption::VIBRATIONSWITCH::VIBRATIONSWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Vibration_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Vibration_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.278f;	// �A�C�R���̊Ԋu�i�U���e�L�X�g�n�_�j
		const D3DXVECTOR2 ICON_SCALE = D3DXVECTOR2(0.071f, 0.125f);
		const float POW_VIB = 0.6f;				// �U���̋���
		const int TIME_VIB = 20;				// �U���̒���
	}

	// �߂�{�^��UI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.12f, 0.92f, 0.0f);
	const D3DXVECTOR2 BACK_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);

	// �I���{�^��UI
	const string SELECT_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\A_Select.png";
	const string NOWSETTING_SELECT_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\A_Enter.png";
	const D3DXVECTOR3 SELECT_BUTTON_UI_POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
	const D3DXVECTOR2 SELECT_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);

	// �ݒ�ۑ���
	const string SETTING_SAVE_PATH = "data\\SETTING\\config.bin";
}

//---------------------------------------------------------
// �ÓI�����o
//---------------------------------------------------------
float COption::m_fBGMVolume = 1.0f;								// BGM�{�����[��
float COption::m_fSEVolume = 1.0f;								// SE�{�����[��
COption::VIBRATIONSWITCH COption::m_Vibration = VIBRATION_ON;	// �o�C�u�L��

//---------------------------------------------------------
// �֐��|�C���^
//---------------------------------------------------------
COption::SettingFunc COption::s_settingFunc[PARAM_MAX] =
{
	&COption::SettingBGM,
	&COption::SettingSE,
	&COption::SettingVibration
};

//=====================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=====================================================
COption::~COption()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT COption::Init(void)
{
	CScene::Init();

	// ���̓}�l�[�W���[����
	CInputManager::Create();

	// UI�z�u
	CreateUIAll();

	// �p�����[�^�ݒ�
	CSound* pSound = CSound::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad != nullptr)
	{// �ݒ�
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume);
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));
	}

	// �w�i�C����
	COcean::Create();

	// �J�����ʒu�̐ݒ�
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
// �I������
//=====================================================
void COption::Uninit(void)
{
	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void COption::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	// ���͏���
	Input();

	// �g�k����
	MoveSelect();
	MoveBGM();
	MoveSE();
	MoveVibration();
}

//=====================================================
// �`�揈��
//=====================================================
void COption::Draw(void)
{
	CScene::Draw();
}

//=====================================================
// UI�����S��
//=====================================================
void COption::CreateUIAll(void)
{
	// �u�I�v�V�����v�̃e�L�X�g
	CreateSingleUI(&m_pOptionText, OPTION_TEXT_TEX_PATH, OPTION_TEXT_POS, OPTION_TEXT_SCALE.x, OPTION_TEXT_SCALE.y);

	// �T�E���h�nUI
	CreateSoundUIObj();

	// �U��
	CreateVibrationUIObj();

	// �{�^��UI
	CreateSingleUI(&m_pBackButtonUI, BACK_BUTTON_UI_TEX_PATH, BACK_BUTTON_UI_POS, BACK_BUTTON_UI_SCALE.x, BACK_BUTTON_UI_SCALE.y);
	CreateSingleUI(&m_pAButtonUI, SELECT_BUTTON_UI_TEX_PATH, SELECT_BUTTON_UI_POS, SELECT_BUTTON_UI_SCALE.x, SELECT_BUTTON_UI_SCALE.y);

	// UI�ݒ�
	SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume);
	SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume);
	ColChangeVibration();
}

//=====================================================
// �T�E���h�nUI����
//=====================================================
void COption::CreateSoundUIObj(void)
{
	for (int cnt = 0; cnt < sizeof(m_aSoundUIObj) / sizeof(SoundUIObject); cnt++)
	{
		D3DXVECTOR3 pos;

		// �ݒ��ނ̃e�L�X�g
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].text, PARAM_TEXT_TEX_PATH[cnt], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

		// �o�[
		pos = D3DXVECTOR3(soundUI::BAR_POS_X, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].bar, soundUI::BAR_TEX_PATH, pos, soundUI::BAR_SCALE.x, soundUI::BAR_SCALE.y);

		// �o�[�̓_
		pos = D3DXVECTOR3(0.5f, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);	// X�l��
		CreateSingleUI(&m_aSoundUIObj[cnt].point, soundUI::BARPOINT_TEX_PATH[0], pos, soundUI::BARPOINT_SCALE.x, soundUI::BARPOINT_SCALE.y);

		// �y���M��
		pos = D3DXVECTOR3(soundUI::PENGUIN_POS_X, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * cnt), 0.0f);
		CreateSingleUI(&m_aSoundUIObj[cnt].penguin, soundUI::PENGUIN_TEX_PATH[cnt], pos, soundUI::PENGUIN_SCALE.x, soundUI::PENGUIN_SCALE.y);
	}
}

//=====================================================
// ���͏����n
//=====================================================
void COption::Input(void)
{
	if (m_bSetting)
	{// ������
		(this->*s_settingFunc[m_optionParam])();
		BackSelect();
	}
	else
	{// �I��
		// ���ڑI��
		Select();

		// �߂鏈��
		ModeSelectBack();
	}
}

//=====================================================
// ���[�h�Z���N�g�ɖ߂鏈��
//=====================================================
void COption::ModeSelectBack(void)
{
	// �t�F�[�h�̊J�n
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_BACK))	// BACK�������Ƃ�
	{
		pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
	}
}

//=====================================================
// �ݒ荀�ڑI��
//=====================================================
void COption::Select(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	if (pInputMgr == nullptr)
		return;

	// �ړ�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{// ��ړ�
		m_optionParam = (OPTIONPARAM)(((int)m_optionParam - 1 + PARAM_MAX) % PARAM_MAX);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{// ���ړ�
		m_optionParam = (OPTIONPARAM)(((int)m_optionParam + 1) % PARAM_MAX);
	}

	// ����
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER))
	{// ����
		m_bSetting = true;

		// A�{�^��UI�̃e�N�X�`���ݒ�
		if (m_pAButtonUI != nullptr)
		{// �e�N�X�`������
			m_pAButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&NOWSETTING_SELECT_BUTTON_UI_TEX_PATH[0]));
		}

		// �߂�{�^��UI�𓧖���
		if (m_pBackButtonUI != nullptr)
		{
			m_pBackButtonUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
}

//=====================================================
// �ݒ荀�ړ�����
//=====================================================
void COption::MoveSelect(void)
{
	CUI* apUI[PARAM_MAX] = { m_aSoundUIObj[0].text,m_aSoundUIObj[1].text,m_pVibrationText };
	for (int cnt = 0; cnt < PARAM_MAX; cnt++)
	{
		if (cnt == m_optionParam)
		{// �I��
			// �T�C�Y�ݒ�
			D3DXVECTOR2 scale = PARAM_TEXT_SCALE * ScaleChange(&m_fOptionTextScale, OPTION_TEXT_SCALING_SPEED, OPTION_TEXT_SCALING_RANGE);
			apUI[cnt]->SetSize(scale.x, scale.y);
			apUI[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			apUI[cnt]->SetVtx();
		}
		else
		{// ��I��
			if (m_bSetting == true)
			{// ���̍��ڂ��ݒ蒆�Ȃ�D�F�ɂ���
				apUI[cnt]->SetCol(NOSELECT_COLOR);
			}
			else
			{// �����ݒ肵�Ă��Ȃ���Βʏ�̐F
				apUI[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			apUI[cnt]->SetSize(PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);	// �T�C�Y�͒ʏ�i�ݒ蒆�֌W�Ȃ��j
			apUI[cnt]->SetVtx();
		}
	}
}

//=====================================================
// �U��UI����
//=====================================================
void COption::CreateVibrationUIObj(void)
{
	D3DXVECTOR3 pos;

	// �e�L�X�g
	pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_VIBRATION), 0.0f);
	CreateSingleUI(&m_pVibrationText, PARAM_TEXT_TEX_PATH[PARAM_VIBRATION], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

	// ON�EOFF�y���M���A�C�R��
	for (int cnt = 0; cnt < sizeof(m_apVibrationPenguin) / sizeof(CUI*); cnt++)
	{
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x + (bibeUI::ICON_POS_INTERVAL * (cnt + 1)),
			PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_VIBRATION), 0.0f);
		CreateSingleUI(&m_apVibrationPenguin[cnt], bibeUI::ICON_TEX_PATH[cnt], pos, bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
	}
}

//=====================================================
// BGM�ݒ�
//=====================================================
void COption::SettingBGM(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	if (pInputMgr == nullptr || pSound == nullptr)
		return;

	// �ړ�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// ���ړ�
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::LABEL_BGM_TITLE, 1.0f);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// �E�ړ�
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::LABEL_BGM_TITLE, 1.0f);
	}
}

//=====================================================
// BGM��UI������
//=====================================================
void COption::MoveBGM(void)
{
	// �T�C�Y�v�Z
	D3DXVECTOR2 scale = soundUI::BARPOINT_SCALE;
	if (m_bSetting && m_optionParam == PARAM_BGM)
	{
		scale *= ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
	}

	// UI�ݒ�
	if (m_aSoundUIObj[PARAM_BGM].point != nullptr)
	{
		m_aSoundUIObj[PARAM_BGM].point->SetSize(scale.x, scale.y);
		m_aSoundUIObj[PARAM_BGM].point->SetVtx();
	}
}

//=====================================================
// SE�ݒ�
//=====================================================
void COption::SettingSE(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	if (pInputMgr == nullptr || pSound == nullptr)
		return;

	// �ړ�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// ���ړ�
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pSound->Play(CSound::LABEL_SE_NOTPECK);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// �E�ړ�
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pSound->Play(CSound::LABEL_SE_NOTPECK);
	}
}

//=====================================================
// SE��UI������
//=====================================================
void COption::MoveSE(void)
{
	// �T�C�Y�v�Z
	D3DXVECTOR2 scale = soundUI::BARPOINT_SCALE;
	if (m_bSetting && m_optionParam == PARAM_SE)
	{
		scale *= ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
	}

	// UI�ݒ�
	if (m_aSoundUIObj[PARAM_SE].point != nullptr)
	{
		m_aSoundUIObj[PARAM_SE].point->SetSize(scale.x, scale.y);
		m_aSoundUIObj[PARAM_SE].point->SetVtx();
	}
}

//=====================================================
// �T�E���h�n�̒l��UI�ݒ�
//=====================================================
void COption::SettingSound(CUI* pUI, float* volumeBase, float volumeAfter)
{
	// ���l�ݒ�
	*volumeBase = volumeAfter;
	universal::LimitValuefloat(volumeBase, 1.0f, 0.0f);

	// UI�ʒu�v�Z
	float pointPosLeft = soundUI::BAR_LENGTH * *volumeBase * 2.0f;	// �o�[�̍�����̋���
	float pointPos = soundUI::BAR_POS_X - (soundUI::BAR_LENGTH) + pointPosLeft;	// ���ۂ̈ʒu

	// UI�ݒ�
	if (pUI != nullptr)
	{
		D3DXVECTOR3 pos = pUI->GetPosition();
		pos.x = pointPos;
		pUI->SetPosition(pos);
		pUI->SetVtx();
	}
}

//=====================================================
// �U���ݒ�
//=====================================================
void COption::SettingVibration(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	if (pInputMgr == nullptr || pJoypad == nullptr)
		return;

	// �ړ�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// ���ړ�
		m_Vibration = (VIBRATIONSWITCH)(((int)m_Vibration - 1 + VIBRATIONSWITCH_MAX) % VIBRATIONSWITCH_MAX);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));

		if (m_Vibration == VIBRATION_ON)
			pJoypad->Vibration(0,bibeUI::POW_VIB, bibeUI::TIME_VIB);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// �E�ړ�
		m_Vibration = (VIBRATIONSWITCH)(((int)m_Vibration + 1) % VIBRATIONSWITCH_MAX);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));

		if (m_Vibration == VIBRATION_ON)
			pJoypad->Vibration(0,bibeUI::POW_VIB, bibeUI::TIME_VIB);
	}

	// �F�ύX
	ColChangeVibration();
}

//=====================================================
// �U����UI������
//=====================================================
void COption::MoveVibration(void)
{
	for (int cnt = 0; cnt < VIBRATIONSWITCH_MAX; cnt++)
	{
		if (m_bSetting && m_optionParam == PARAM_VIBRATION && cnt == m_Vibration)
		{// �I��
			// �T�C�Y�ݒ�
			D3DXVECTOR2 scale = bibeUI::ICON_SCALE * ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
			m_apVibrationPenguin[cnt]->SetSize(scale.x, scale.y);
		}
		else
		{// ��I��
			m_apVibrationPenguin[cnt]->SetSize(bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
		}
		m_apVibrationPenguin[cnt]->SetVtx();
	}
}

//=====================================================
// �U���̐F�ύX
//=====================================================
void COption::ColChangeVibration(void)
{
	for (int cnt = 0; cnt < VIBRATIONSWITCH_MAX; cnt++)
	{
		if (cnt == m_Vibration)
		{// �I��
			m_apVibrationPenguin[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// ��I��
			m_apVibrationPenguin[cnt]->SetCol(NOSELECT_COLOR);
		}
		m_apVibrationPenguin[cnt]->SetVtx();
	}
}

//=====================================================
// �I����Ԃɖ߂�
//=====================================================
void COption::BackSelect(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	if (pInputMgr == nullptr)
		return;

	// �߂�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER))
	{// �߂�
		m_bSetting = false;

		// A�{�^��UI�̃e�N�X�`���ݒ�
		if (m_pAButtonUI != nullptr)
		{// �e�N�X�`������
			m_pAButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&SELECT_BUTTON_UI_TEX_PATH[0]));
		}

		// �߂�{�^��UI��s������
		if (m_pBackButtonUI != nullptr)
		{
			m_pBackButtonUI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// �ݒ���O���t�@�C���ɕۑ�
		SaveSetting();
	}
}

//=====================================================
// UI�P�̐���
//=====================================================
void COption::CreateSingleUI(CUI** ppUI, string path, D3DXVECTOR3 pos, float width, float height)
{
	*ppUI = CUI::Create();
	if (*ppUI != nullptr)
	{// �ݒ�
		(*ppUI)->SetIdxTexture(CTexture::GetInstance()->Regist(&path[0]));	// �e�N�X�`������
		(*ppUI)->SetPosition(pos);			// �ʒu
		(*ppUI)->SetSize(width, height);	// �傫��
		(*ppUI)->SetVtx();	// ���_���f
	}
}

//=====================================================
// UI�T�C�Y�ύX�isin�g�j
//=====================================================
float COption::ScaleChange(float* angle, float speed, float range)
{
	// �p�x�ύX
	*angle += speed;
	if (*angle >= 2.0f * D3DX_PI)
		*angle -= 2.0f * D3DX_PI;

	// ���ʕԂ�
	return sinf(*angle) * range + 1.0f;
}

//=====================================================
// �ݒ�ۑ�
//=====================================================
void COption::SaveSetting(void)
{
	// �t�@�C�����J��
	std::ofstream outputFile(SETTING_SAVE_PATH, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// �f�[�^�̕ۑ�
	outputFile.write(reinterpret_cast<char*>(&m_fBGMVolume), sizeof(float));
	outputFile.write(reinterpret_cast<char*>(&m_fSEVolume), sizeof(float));
	outputFile.write(reinterpret_cast<char*>(&m_Vibration), sizeof(VIBRATIONSWITCH));

	outputFile.close();
}

//=====================================================
// �ݒ�ǂݍ���
//=====================================================
void COption::LoadSetting(void)
{
	// �t�@�C�����J��
	std::ifstream inputFile(SETTING_SAVE_PATH, std::ios::binary);

	if (inputFile.is_open())
	{// �t�@�C��������i�ǂݍ���
		inputFile.read(reinterpret_cast<char*>(&m_fBGMVolume), sizeof(float));
		inputFile.read(reinterpret_cast<char*>(&m_fSEVolume), sizeof(float));
		inputFile.read(reinterpret_cast<char*>(&m_Vibration), sizeof(VIBRATIONSWITCH));

		inputFile.close();
	}
	else
	{// �t�@�C�����Ȃ��i�����ݒ�j
		m_fBGMVolume = 1.0f;
		m_fSEVolume = 1.0f;
		m_Vibration = VIBRATION_ON;
		SaveSetting();	// �t�@�C������
	}

	// �ݒ�
	CSound* pSound = CSound::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	if (pSound != nullptr && pJoypad != nullptr)
	{// �ݒ�
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
		pJoypad->SetVibration((m_Vibration == VIBRATION_ON));
	}
}