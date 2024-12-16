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
#include "inputManager.h"
#include "sound.h"
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
	const float SELECTPARAM_SCALING_RANGE = 0.20f;				// �g�k���i�I�����ځj

	// �ݒ��ނ̃e�L�X�g
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Bibe.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.139f, 0.263f, 0.0f);	// BGM�̃e�L�X�g�̈ʒu
	const float PARAM_TEXT_POS_INTERVAL = 0.25f;							// ���̃e�L�X�g�̊Ԋu�iY�l�̂݁j
	const D3DXVECTOR2 PARAM_TEXT_SCALE = D3DXVECTOR2(0.086f, 0.075f);


	// BGM�ESE
	namespace soundUI
	{
		// �o�[
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.522f;	// �o�[�̈ʒu�iX�̂݁EY = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * �z�u���j
		const D3DXVECTOR2 BAR_SCALE = D3DXVECTOR2(0.234f, 0.064f);
		
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
		const string ICON_TEX_PATH[COption::BIBESWITCH::BIBESWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Bibe_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Bibe_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.278f;	// �A�C�R���̊Ԋu�i�U���e�L�X�g�n�_�j
		const D3DXVECTOR2 ICON_SCALE = D3DXVECTOR2(0.071f, 0.125f);
		const D3DXCOLOR NOSELECT_COLOR = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.8f);
	}

	// �߂�{�^��UI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.12f, 0.92f, 0.0f);
	const D3DXVECTOR2 BACK_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);
}

//---------------------------------------------------------
// �֐��|�C���^
//---------------------------------------------------------
COption::SettingFunc COption::s_settingFunc[PARAM_MAX] =
{
	&COption::SettingBGM,
	&COption::SettingSE,
	&COption::SettingBibe
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
	m_pBibeText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_apBibePenguin) / sizeof(CUI*); cnt++) { m_apBibePenguin[cnt] = nullptr; }
	m_pButtonUI = nullptr;

	m_optionParam = PARAM_BGM;
	m_fOptionTextScale = 0.0f;
	m_fBGMVolume = 0.0f;
	m_fSEVolume = 0.0f;
	m_fParamScale = 0.0f;
	m_Bibe = BIBE_ON;
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

	// �w�i�C����
	COcean::Create();

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
	MoveBibe();
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
	CreateBibeUIObj();

	// �߂�{�^��UI
	CreateSingleUI(&m_pButtonUI, BACK_BUTTON_UI_TEX_PATH, BACK_BUTTON_UI_POS, BACK_BUTTON_UI_SCALE.x, BACK_BUTTON_UI_SCALE.y);

	// UI�ݒ�
	SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, 1.0f);
	SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, 1.0f);
	ColChangeBibe();
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
	}
}

//=====================================================
// �ݒ荀�ړ�����
//=====================================================
void COption::MoveSelect(void)
{
	CUI* apUI[PARAM_MAX] = { m_aSoundUIObj[0].text,m_aSoundUIObj[1].text,m_pBibeText };
	for (int cnt = 0; cnt < PARAM_MAX; cnt++)
	{
		if (cnt == m_optionParam)
		{// �I��
			// �T�C�Y�ݒ�
			D3DXVECTOR2 scale = PARAM_TEXT_SCALE * ScaleChange(&m_fOptionTextScale, OPTION_TEXT_SCALING_SPEED, OPTION_TEXT_SCALING_RANGE);

			apUI[cnt]->SetSize(scale.x, scale.y);
			apUI[cnt]->SetVtx();
		}
		else
		{// ��I��
			apUI[cnt]->SetSize(PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);
			apUI[cnt]->SetVtx();
		}
	}
}

//=====================================================
// �U��UI����
//=====================================================
void COption::CreateBibeUIObj(void)
{
	D3DXVECTOR3 pos;

	// �e�L�X�g
	pos = D3DXVECTOR3(PARAM_TEXT_POS.x, PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_BIBE), 0.0f);
	CreateSingleUI(&m_pBibeText, PARAM_TEXT_TEX_PATH[PARAM_BIBE], pos, PARAM_TEXT_SCALE.x, PARAM_TEXT_SCALE.y);

	// ON�EOFF�y���M���A�C�R��
	for (int cnt = 0; cnt < sizeof(m_apBibePenguin) / sizeof(CUI*); cnt++)
	{
		pos = D3DXVECTOR3(PARAM_TEXT_POS.x + (bibeUI::ICON_POS_INTERVAL * (cnt + 1)),
			PARAM_TEXT_POS.y + (PARAM_TEXT_POS_INTERVAL * PARAM_BIBE), 0.0f);
		CreateSingleUI(&m_apBibePenguin[cnt], bibeUI::ICON_TEX_PATH[cnt], pos, bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
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
	{// ��ړ�
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// ���ړ�
		SettingSound(m_aSoundUIObj[PARAM_BGM].point, &m_fBGMVolume, m_fBGMVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_BGM, m_fBGMVolume);
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
	{// ��ړ�
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume - 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// ���ړ�
		SettingSound(m_aSoundUIObj[PARAM_SE].point, &m_fSEVolume, m_fSEVolume + 0.1f);
		pSound->SetVolume(CSound::SOUNDTYPE::TYPE_SE, m_fSEVolume);
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
	float pointPosLeft = soundUI::BAR_SCALE.x * *volumeBase * 2.0f;	// �o�[�̍�����̋���
	float pointPos = soundUI::BAR_POS_X - (soundUI::BAR_SCALE.x) + pointPosLeft;	// ���ۂ̈ʒu

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
void COption::SettingBibe(void)
{
	CInputManager* pInputMgr = CInputManager::GetInstance();
	if (pInputMgr == nullptr)
		return;

	// �ړ�
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{// ��ړ�
		m_Bibe = (BIBESWITCH)(((int)m_Bibe - 1 + BIBESWITCH_MAX) % BIBESWITCH_MAX);
	}
	else if (pInputMgr->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{// ���ړ�
		m_Bibe = (BIBESWITCH)(((int)m_Bibe + 1) % BIBESWITCH_MAX);
	}

	// �F�ύX
	ColChangeBibe();
}

//=====================================================
// �U����UI������
//=====================================================
void COption::MoveBibe(void)
{
	for (int cnt = 0; cnt < BIBESWITCH_MAX; cnt++)
	{
		if (m_bSetting && m_optionParam == PARAM_BIBE && cnt == m_Bibe)
		{// �I��
			// �T�C�Y�ݒ�
			D3DXVECTOR2 scale = bibeUI::ICON_SCALE * ScaleChange(&m_fParamScale, SELECTPARAM_SCALING_SPEED, SELECTPARAM_SCALING_RANGE);
			m_apBibePenguin[cnt]->SetSize(scale.x, scale.y);
		}
		else
		{// ��I��
			m_apBibePenguin[cnt]->SetSize(bibeUI::ICON_SCALE.x, bibeUI::ICON_SCALE.y);
		}
		m_apBibePenguin[cnt]->SetVtx();
	}
}

//=====================================================
// �U���̐F�ύX
//=====================================================
void COption::ColChangeBibe(void)
{
	for (int cnt = 0; cnt < BIBESWITCH_MAX; cnt++)
	{
		if (cnt == m_Bibe)
		{// �I��
			m_apBibePenguin[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// ��I��
			m_apBibePenguin[cnt]->SetCol(bibeUI::NOSELECT_COLOR);
		}
		m_apBibePenguin[cnt]->SetVtx();
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
	if (pInputMgr->GetTrigger(CInputManager::BUTTON_BACK))
	{// �߂�
		m_bSetting = false;
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