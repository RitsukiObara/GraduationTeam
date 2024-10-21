//*****************************************************
//
// �X�e�[�W���U���gUI�̏���[stageResultUI.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ocean_flow_UI.h"
#include "manager.h"
#include "objectX.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "UI.h"
#include "inputManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// ���ڂ̕�
#define RESULT_HEIGHT	(0.05f)	// ���ڂ̍���
#define MOVE_FACT	(0.15f)	// �ړ����x
#define LINE_ARRIVAL	(0.05f)	// ���������Ƃ���邵�����l
#define LINE_UNINIT	(3.0f)	// �I������܂ł̂������l

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// �N���A���S�̃p�X
	const char* FAIL_LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";	// ���s���S�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
COceanFlowUI* COceanFlowUI::m_pOceanFlowUI = nullptr;	// ���g�̃|�C���^

//====================================================
// �R���X�g���N�^
//====================================================
COceanFlowUI::COceanFlowUI()
{
	m_state = STATE_NONE;
	m_bSound = false;
	nCountMove = 0;
}

//====================================================
// �f�X�g���N�^
//====================================================
COceanFlowUI::~COceanFlowUI()
{

}

//====================================================
// ��������
//====================================================
COceanFlowUI* COceanFlowUI::Create(void)
{
	if (m_pOceanFlowUI == nullptr)
	{
		m_pOceanFlowUI = new COceanFlowUI;

		if (m_pOceanFlowUI != nullptr)
		{
			m_pOceanFlowUI->Init();
		}
	}

	return m_pOceanFlowUI;
}

//====================================================
// ����������
//====================================================
HRESULT COceanFlowUI::Init(void)
{
	//	��󃂃f���̏�����
	if (m_apArrow != nullptr)
	{
		m_apArrow->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
		m_apArrow->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_apArrow->BindModel(CModel::Load("data\\MODEL\\other\\Arrow001.x"));
	}

	m_state = STATE_IN;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void COceanFlowUI::Uninit(void)
{
	// ���j���[���ڂ̔j��
	if (m_apArrow != nullptr)
	{
		Uninit();

		m_apArrow = nullptr;
	}

	m_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void COceanFlowUI::Update(void)
{
	// ��ԊǗ�
	ResultState();
}

//====================================================
// ��ԊǗ�
//====================================================
void COceanFlowUI::ResultState(void)
{

}

//====================================================
// �`�揈��
//====================================================
void COceanFlowUI::Draw(void)
{

}