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
#include "iceManager.h"

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
	//const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// �N���A���S�̃p�X
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
	m_pArrow = CObjectX::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//	��󃂃f���̏�����
	if (m_pArrow != nullptr)
	{
		m_pArrow->Init();

		m_pArrow->BindModel(CModel::Load("data\\MODEL\\other\\Arrow001.x"));
	}

	CIceManager::GetInstance()->GetDirStream();

	m_state = STATE_IN;

	return S_OK;
}

//====================================================
// �I������
//====================================================
void COceanFlowUI::Uninit(void)
{
	// ���j���[���ڂ̔j��
	if (m_pArrow != nullptr)
	{
		m_pArrow->Uninit();

		m_pArrow = nullptr;
	}

	m_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void COceanFlowUI::Update(void)
{
	OceanFlowKeep = CIceManager::GetInstance()->GetDirStream();
	D3DXVECTOR3 Rot = m_pArrow->GetRotation();

	if (OceanFlowKeep == CIceManager::STREAM_UP)
	{
		universal::FactingRot(&Rot.y, D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_RIGHT)
	{
		universal::FactingRot(&Rot.y, D3DX_PI, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_DOWN)
	{
		universal::FactingRot(&Rot.y, -D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_LEFT)
	{
		universal::FactingRot(&Rot.y, 0.0f, 0.02f);
	}

	m_pArrow->SetRotation(Rot);

	// ��ԊǗ�
	OceanState();
}

//====================================================
// ��ԊǗ�
//====================================================
void COceanFlowUI::OceanState(void)
{

}

//====================================================
// �`�揈��
//====================================================
void COceanFlowUI::Draw(void)
{

}