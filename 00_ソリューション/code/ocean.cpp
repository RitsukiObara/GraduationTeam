//*****************************************************
//
// �I�[�V��������[ocean.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"
#include "ocean.h"
#include "iceManager.h"
#include "timer.h"
#include "universal.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\BG\\field00.jpg"				// �e�N�X�`���t�@�C����
#define MOVE_SPEED					(1.0f)										// �ړ����x
#define MESH_LENGTH					(800.0f)									// ���b�V���̈�ӂ̒���
#define MESH_U						(254)											// ���̃u���b�N��
#define MESH_V						(254)											// �c�̃u���b�N��
#define SPLIT_TEX					(10)										// �e�N�X�`��������
#define CHENGE_LENGTH	(10000)	// ����ł��钸�_�܂ł̋���
#define ANGLE_SLIP	(0.7f)	// �������p�x
#define CMP_LENGTH	(1000.0f)	// ���肷�锼�a

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
COcean::COcean()
{
	m_fSpeed = 0.0f;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
COcean::~COcean()
{

}

//=====================================================
// ��������
//=====================================================
COcean* COcean::Create(void)
{
	if (m_pOcean == nullptr)
	{
		m_pOcean = new COcean;

		if (m_pOcean != nullptr)
		{
			m_pOcean->Init();
		}
	}

	return m_pOcean;
}

//=====================================================
// ����������
//=====================================================
HRESULT COcean::Init(void)
{
	CMeshField::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void COcean::Uninit(void)
{
	m_pOcean = nullptr;

	CMeshField::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void COcean::Update(void)
{
	float OceanFlowLevel = CIceManager::GetInstance()->GetOceanLevel();	//	�C�����x���̎擾

	CMeshField::Update();
	//OceanCycleTimer();

	m_fSpeed += 0.007f * OceanFlowLevel;

	//OceanRotState();

	universal::LimitRot(&m_fSpeed);

	CMeshField::Wave(m_fSpeed);

}

//=====================================================
// �`�揈��
//=====================================================
void COcean::Draw(void)
{
	CMeshField::Draw();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//====================================================
// �C���̌����ƃ��b�V���̌�����A�������鏈��
//====================================================
void COcean::OceanRotState(void)
{
	int OceanFlowKeep = CIceManager::GetInstance()->GetDirStream();
	m_fRot = CMeshField::GetRotation();

	//	��󂪊C���̌����ɗ���鏈��
	if (OceanFlowKeep == CIceManager::STREAM_UP)
	{
		m_fRot.y = 0.0f;
	}

	if (OceanFlowKeep == CIceManager::STREAM_RIGHT)
	{
		m_fRot.y = D3DX_PI * 0.5f;
	}

	if (OceanFlowKeep == CIceManager::STREAM_DOWN)
	{
		m_fRot.y = D3DX_PI;
	}

	if (OceanFlowKeep == CIceManager::STREAM_LEFT)
	{
		m_fRot.y = -D3DX_PI * 0.5f;
	}

	CMeshField::SetRotation(m_fRot);
}

//====================================================
// �C�����������ԂŊǗ����鏈��
//====================================================
void COcean::OceanCycleTimer(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();
	float OceanCycleTimer = CGame::GetInstance()->GetTimeSecond();	 // ���݂̃^�C�����擾

	if (OceanCycleTimer <= 111.0f &&
		OceanCycleTimer >= 110.0f)
	{
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		pIceManager->SetDirStreamNext(pIceManager->E_Stream::STREAM_DOWN);	// �C���̌��������ɂ���
		//universal::RandRange();
	}

	if (OceanCycleTimer <= 101.0f &&
		OceanCycleTimer >= 100.0f)
	{
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		pIceManager->SetDirStreamNext(pIceManager->E_Stream::STREAM_LEFT);	// �C���̌��������ɂ���
	}

}