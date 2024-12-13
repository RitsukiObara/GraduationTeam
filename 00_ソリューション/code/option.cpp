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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{// UI�z�u�̏ォ�珇�ɒ�`
	// �u�I�v�V�����v�̃e�L�X�g
	const string OPTION_TEXT_TEX_PATH = "data\\TEXTURE\\UI\\option_Text.png";
	const D3DXVECTOR3 OPTION_TEXT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ݒ��ނ̃e�L�X�g
	const string PARAM_TEXT_TEX_PATH[] =
	{
		"data\\TEXTURE\\UI\\option_BGM.png",
		"data\\TEXTURE\\UI\\option_SE.png",
		"data\\TEXTURE\\UI\\option_Bibe.png"
	};
	const D3DXVECTOR3 PARAM_TEXT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// BGM�̃e�L�X�g�̈ʒu
	const float PARAM_TEXT_POS_INTERVAL = 0.0f;							// ���̃e�L�X�g�̊Ԋu�iY�l�̂݁j

	// BGM�ESE
	namespace soundUI
	{
		// �o�[
		const string BAR_TEX_PATH = "data\\TEXTURE\\UI\\option_Bar.png";
		const float BAR_POS_X = 0.0f;	// �o�[�̈ʒu�iX�̂݁EY = PARAM_TEXT_POS.y + PARAM_TEXT_POS_INTERVAL * �z�u���j
		const float BAR_WIDTH = 0.0f;	// �o�[�̒����i�o�[�̓_�Ŏg���j
		
		// �o�[�̏�ɂ���_
		const string BARPOINT_TEX_PATH[] =
		{
			"data\\TEXTURE\\UI\\option_Bar_Select.png",		// �I��
			"data\\TEXTURE\\UI\\option_Bar_UnSelect.png"	// ��I��
		};

		// �y���M��
		const string PENGUIN_TEX_PATH[COption::OPTIONPARAM::PARAM_MAX - 1] =
		{
			"data\\TEXTURE\\UI\\option_BGM_Icon.png",
			"data\\TEXTURE\\UI\\option_SE_Icon.png"
		};
		const float PENGUIN_POS_X = 0.0f;	// �y���M���̈ʒu�iX�̂݁j
	}

	// �U��
	namespace bibeUI
	{
		const string ICON_TEX_PATH[COption::BIBESWITCH::BIBESWITCH_MAX] =
		{
			"data\\TEXTURE\\UI\\option_Bibe_On_Icon.png",
			"data\\TEXTURE\\UI\\option_Bibe_Off_Icon.png"
		};
		const float ICON_POS_INTERVAL = 0.0f;	// �A�C�R���̊Ԋu�i�U���e�L�X�g�n�_�j
	}

	// �߂�{�^��UI
	const string BACK_BUTTON_UI_TEX_PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const D3DXVECTOR3 BACK_BUTTON_UI_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
