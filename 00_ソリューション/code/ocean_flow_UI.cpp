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
#include "gameManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// ���ڂ̕�
#define RESULT_HEIGHT	(0.05f)	// ���ڂ̍���
#define MOVE_FACT	(0.15f)	// �ړ����x
#define LINE_ARRIVAL	(0.05f)	// ���������Ƃ���邵�����l
#define LINE_UNINIT	(3.0f)	// �I������܂ł̂������l
#define MAX_OCEANLEVEL (7.00f)	// �C���ő僌�x��

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_SINGLE = { -1000.0f,200.0f,0.0f };	// �V���O���v���C���̖��̈ʒu
const D3DXVECTOR3 POS_MULTI = { -1500.0f ,400.0f,700.0f };	// �}���`�v���C���̖��̈ʒu

const string PATH_MODEL_SINGLE = "data\\MODEL\\other\\Arrow001.x";	// �V���O�����̃��f���p�X
const string PATH_MODEL_MULTI = "data\\MODEL\\other\\Arrow002.x";	// �}���`���̃��f���p�X

const string PATH_COMPASS_SINGLE = "data\\MODEL\\other\\Compass00.x";	// �V���O�����̃R���p�X���f���p�X
const string PATH_COMPASS_MULTI = "data\\MODEL\\other\\Compass01.x";	// �}���`���̃R���p�X���f���p�X

const float TIMEMAX_SHAKE = 5.0f;		// ���̗h��̍ő厞��
const float SPEED_SHAKE_INIT = 0.1f;	// ���̏����h�ꑬ�x
const float MAGNITUDE_SHAKE = 100.0f;	// �h��̋K��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
COceanFlowUI* COceanFlowUI::s_pOceanFlowUI = nullptr;	// ���g�̃|�C���^

//====================================================
// �R���X�g���N�^
//====================================================
COceanFlowUI::COceanFlowUI()
{
	m_state = STATE_NONE;
	m_pDir = nullptr;
	m_pArrow = nullptr;
	m_pCompass = nullptr;
	m_fTimerShakeArrow = 0.0f;
	m_fSpeedShakeArrow = 0.0f;
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
	if (s_pOceanFlowUI == nullptr)
	{
		s_pOceanFlowUI = new COceanFlowUI;

		if (s_pOceanFlowUI != nullptr)
		{
			s_pOceanFlowUI->Init();
		}
	}

	return s_pOceanFlowUI;
}

//====================================================
// ����������
//====================================================
HRESULT COceanFlowUI::Init(void)
{
	// �����p�̋�̃I�u�W�F�N�g����
	CreateDir();

	// ���̐���
	CreateArrow();

	// �R���p�X�̐���
	//CreateCompass();

	// �l�̏�����
	m_state = STATE_IN;
	m_fSpeedShakeArrow = SPEED_SHAKE_INIT;

	return S_OK;
}

//====================================================
// �����p�̋�̃I�u�W�F�N�g����
//====================================================
void COceanFlowUI::CreateDir(void)
{
	if (m_pDir != nullptr)
		return;

	m_pDir = CObject3D::Create();

	// �ʒu�̐ݒ�
	if (m_pDir != nullptr)
	{
		bool bMulti = gameManager::IsMulti();

		// ���[�h���̈ʒu
		if (bMulti)
			m_pDir->SetPosition(POS_MULTI);
		else
			m_pDir->SetPosition(POS_SINGLE);
	}
}

//====================================================
// ��󐶐�
//====================================================
void COceanFlowUI::CreateArrow(void)
{
	if (m_pArrow != nullptr)
		return;

	m_pArrow = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ��󃂃f���̏�����
	if (m_pArrow != nullptr)
	{
		CGame::E_GameMode gamemode = CGame::GetInstance()->GetGameMode();

		// ���[�h���̖��ʒu
		if (gamemode == CGame::MODE_SINGLE)
			m_pArrow->BindModel(CModel::Load(&PATH_MODEL_SINGLE[0]));
		else
			m_pArrow->BindModel(CModel::Load(&PATH_MODEL_MULTI[0]));
	}
}

//====================================================
// �R���p�X����
//====================================================
void COceanFlowUI::CreateCompass(void)
{
	if (m_pCompass != nullptr)
		return;

	m_pCompass = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pCompass != nullptr)
	{// ������
		bool bMulti = gameManager::IsMulti();

		// ���[�h���̐ݒ�
		if (bMulti)
		{
			m_pCompass->SetPosition(POS_MULTI);
			m_pCompass->BindModel(CModel::Load(&PATH_COMPASS_MULTI[0]));
		}
		else
		{
			m_pCompass->SetPosition(POS_SINGLE);
			m_pCompass->BindModel(CModel::Load(&PATH_COMPASS_SINGLE[0]));
		}
	}
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

	s_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void COceanFlowUI::Update(void)
{
	// �C���̌����␳����
	OceanRotState();

	// �C�����x����ԊǗ�
	OceanLevelState();
}

//====================================================
// �C���̌����␳����
//====================================================
void COceanFlowUI::OceanRotState(void)
{
	if (m_pDir == nullptr)
		return;

	int OceanFlowKeep = CIceManager::GetInstance()->GetDirStreamNext();
	D3DXVECTOR3 Rot = m_pDir->GetRotation();

	//	��󂪊C���̌����ɗ���鏈��
	if (OceanFlowKeep == COcean::STREAM_UP)
	{
		universal::FactingRot(&Rot.y, D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_RIGHT)
	{
		universal::FactingRot(&Rot.y, D3DX_PI, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_DOWN)
	{
		universal::FactingRot(&Rot.y, -D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_LEFT)
	{
		universal::FactingRot(&Rot.y, 0.0f, 0.02f);
	}

	m_pDir->SetRotation(Rot);
}

//====================================================
// �C�����x����UI����
//====================================================
void COceanFlowUI::OceanLevelState(void)
{
	if (m_pArrow == nullptr || m_pDir == nullptr)
		return;

	if (CIceManager::GetInstance() == nullptr)
		return;

	float OceanFlowLevel = CIceManager::GetInstance()->GetOceanLevel();	//	�C�����x���̎擾
	D3DXCOLOR fEmissiveCol = m_pArrow->GetDeffuseeCol(0);	//	�C��UI�̐F�擾

	float Colorrate = OceanFlowLevel / MAX_OCEANLEVEL;	// �C���ő僌�x���Ƃ̊���

	// �ڕW�̐F�ɑJ��
	fEmissiveCol = D3DXCOLOR(0.8f + (0.2f * Colorrate), 0.8f - (0.6f * Colorrate), 0.2f, 1.0f);

	m_pArrow->SetDeffuseCol(fEmissiveCol,0);

	// ���̑O��ړ���ǉ�
	ShakeArrow(Colorrate);

	// ���̐e�q�t��
	D3DXMATRIX mtx = m_pDir->GetMatrix();
	m_pArrow->SetMatrixParent(mtx);
}

//====================================================
// ���̗h��
//====================================================
void COceanFlowUI::ShakeArrow(float fRate)
{
	if (m_pArrow == nullptr)
		return;

	// �T�C���J�[�u�v�Z
	float fSin = universal::CalcSinWave(m_fTimerShakeArrow, TIMEMAX_SHAKE);

	float fShake = MAGNITUDE_SHAKE * fSin;

	m_pArrow->SetPosition(D3DXVECTOR3(fShake, 0.0f, 0.0f));

	m_fTimerShakeArrow += m_fSpeedShakeArrow * fRate;

	// �^�C�}�[��͈͓��Ɏ��߂�
	m_fTimerShakeArrow = std::fmod(m_fTimerShakeArrow, TIMEMAX_SHAKE);
}

//====================================================
// �`�揈��
//====================================================
void COceanFlowUI::Draw(void)
{

}