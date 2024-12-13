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
#include "UI.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{// UI�z�u�̏ォ�珇�ɒ�`
	// �u�I�v�V�����v�̃e�L�X�g
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.1375f, 0.072f, 0.0f);
	const D3DXVECTOR2 OPTION_TEXT_SCALE = D3DXVECTOR2(0.129f, 0.057f);

	// �ݒ��ނ̃e�L�X�g
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Bibe.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.089f, 0.263f, 0.0f);	// BGM�̃e�L�X�g�̈ʒu
	const float PARAM_TEXT_POS_INTERVAL = 0.25f;							// ���̃e�L�X�g�̊Ԋu�iY�l�̂݁j
	const D3DXVECTOR2 PARAM_TEXT_SCALE = D3DXVECTOR2(0.086f, 0.075f);

	// BGM�ESE
	namespace soundUI
	{
		// �o�[
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.422f;	// �o�[�̈ʒu�iX�̂݁EY = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * �z�u���j
		const float BAR_WIDTH = 0.375f;	// �o�[�̒����i�o�[�̓_�Ŏg���j
		const D3DXVECTOR2 BAR_SCALE = D3DXVECTOR2(0.188f, 0.064f);
		
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
	}

	// �߂�{�^��UI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.12f, 0.92f, 0.0f);
	const D3DXVECTOR2 BACK_BUTTON_UI_SCALE = D3DXVECTOR2(0.09f, 0.049f);
}

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
	m_fBGMVolume = 0.0f;
	m_fSEVolume = 0.0f;
	m_Bibe = BIBE_ON;
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

	// UI�z�u
	CreateUIAll();

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