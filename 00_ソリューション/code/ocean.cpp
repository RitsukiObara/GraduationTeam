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
#include "albatross.h"
#include "BG_Ice.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int OCEAN_ROT_CHANGE_TIME_DEFAULT = 10;	// �f�t�H���g�̊C�������ύX����
	const int OCEAN_ROT_CHANGE_TIME_DEGREE = 10;	// �C�������ύX���ԂԂꕝ
	const float FLOW_LEVEL_MULTIPLY = 0.008f;		// �C���̑��x�̔{��
	const int MAX_ALBATROSS = 2;					// �A�z�E�h���ő吔
	const int MAX_RANGE_LEFT = -2800;				// �����_�������W���ő吔
	const int MIN_RANGE_LEFT = -2000;				// �����_�������W���Œᐔ
	const int MAX_RANGE_RIGHT = 2000;				// �����_�������W�E�ő吔
	const int MIN_RANGE_RIGHT = 1200;				// �����_�������W�E�Œᐔ
	const float Z_UP = 1500.0f;						// Z���������
	const float Z_DOWN = -1500.0f;					// Z����������
	const int BGICE_CREATE_CNT_L = 250;				// �w�i�X�����������b(��)
	const int BGICE_CREATE_CNT_R = 300;				// �w�i�X�����������b(�E)

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
COcean::COcean(int nPriority) : CMeshField(nPriority)
{
	m_fSpeed = 0.0f;
	m_nRandKeep = 0;
	m_nRandNextKeep = 0;
	m_bRandState = false;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSetRotTime = 0;
	m_nExecRotChangeTime = 0;
	m_nBgiceCnt_L = 0;
	m_nBgiceCnt_R = 0;
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
	m_nBgiceCnt_L = 0;
	m_nBgiceCnt_R = 0;

	CMeshField::Init();

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
	{ // ���X�}�l�[�W���[�� NULL ����Ȃ��ꍇ

		// �ۑ��p�ϐ��Ɍ��݂̊C����ݒ肷��
		m_nRandKeep = pIceManager->GetDirStream();
	}

	SetNextOceanRot();	// �ŏ��Ɏ��̌����ݒ�

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
	float OceanFlowLevel = 0.0f;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if(pIceMgr != nullptr)
		OceanFlowLevel = pIceMgr->GetOceanLevel();	//	�C�����x���̎擾

	CMeshField::Update();

	m_fSpeed += FLOW_LEVEL_MULTIPLY * OceanFlowLevel;

	BgIceRotState();
	//OceanCycleTimer();
	OceanChangeCheck();

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
// ���̊C���̌����ݒ菈��
//====================================================
void COcean::SetNextOceanRot(void)
{
	CGame* pGame = CGame::GetInstance();
	if (pGame == nullptr)
		return;

	// �����ύX
	do
	{
		m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
	} while (m_nRandKeep == m_nRandNextKeep);	// ���̌������ς��܂ŗ�������

	// �ύX���Ԑݒ�
	m_nExecRotChangeTime = OCEAN_ROT_CHANGE_TIME_DEFAULT + universal::RandRange(OCEAN_ROT_CHANGE_TIME_DEGREE, 0);
	m_nSetRotTime = pGame->GetTimeSecond();	 // ���݂̃^�C�����擾
}

//====================================================
// �C���̌����ύX���Ԃ��m�F����
//====================================================
void COcean::OceanChangeCheck(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();
	CGame* pGame = CGame::GetInstance();

	if (pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	COcean::E_Stream OceanFlow = pIceManager->GetDirStreamNext();

	int nNowTime = pGame->GetTimeSecond();	 // ���݂̃^�C�����擾

	if (m_nSetRotTime - nNowTime >= m_nExecRotChangeTime)
	{// �ύX���ԂɂȂ���
		pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// �C���̑��x��������
		pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// �C���̌����������_���ɂ���
		m_nRandKeep = m_nRandNextKeep;	// ���݂̌����ɐݒ�
		SetNextOceanRot();	// ���̌����ݒ�
		m_bRandState = false;	// �����_���̏�Ԃ� false �ɂ���
	}

	if (m_nSetRotTime - nNowTime + 3 == m_nExecRotChangeTime)
	{
		if (m_bRandState == false)
		{
			for (int nCnt = 0; nCnt < MAX_ALBATROSS; nCnt++)
			{
				// �A�z�E�h������
				CAlbatross::Create((COcean::E_Stream)(m_nRandNextKeep));
			}

			m_bRandState = true;
		}
	}
}

//====================================================
// �C���̌����ƃ��b�V���̌�����A�������鏈��
//====================================================
void COcean::BgIceRotState(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int OceanFlowKeep = pIceManager->GetDirStream();
	m_fRot = CMeshField::GetRotation();

	//	��󂪊C���̌����ɗ���鏈��
	if (OceanFlowKeep == COcean::STREAM_UP)
	{
		//������w�i�X���o������
		BgIceSetPosDown();
	}

	if (OceanFlowKeep == COcean::STREAM_DOWN)
	{
		//�ォ��w�i�X���o������
		BgIceSetPosUp();
	}
}

//====================================================
// �C�����������ԂŊǗ����鏈��
//====================================================
void COcean::OceanCycleTimer(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();

	CGame *pGame = CGame::GetInstance();

	if (pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	int OceanCycleTimer = pGame->GetTimeSecond();	 // ���݂̃^�C�����擾

	// 10�̔{���̎��ɓ���
	if (OceanCycleTimer % 10 == 0)
	{
		if (m_bRandState == false)
		{
			m_bRandState = true;

			m_nRandKeep = m_nRandNextKeep;
		}

		// ����Ǝ��̌����������Ƃ����l�����Z�b�g
		if (m_nRandKeep == m_nRandNextKeep)
		{
			m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
		}

		// ����Ǝ��̌�������������Ȃ��Ƃ�
		if (m_nRandKeep != m_nRandNextKeep)
		{
			pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// �C���̑��x��������
			pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// �C���̌����������_���ɂ���
		}
	}
}

//====================================================
// ���������w�i�X���o�Ă��鏈��
//====================================================
void COcean::BgIceSetPosUp(void)
{
	float posX_L = (float)universal::RandRange(MAX_RANGE_LEFT, MIN_RANGE_LEFT);
	CBgIce::TYPE type = (CBgIce::TYPE)universal::RandRange(CBgIce::TYPE_MAX, CBgIce::TYPE_BIG);

	m_nBgiceCnt_L++;
	m_nBgiceCnt_R++;

	if (m_nBgiceCnt_L >= BGICE_CREATE_CNT_L)
	{
		// �w�i�X�̃��[�h
		CBgIce::Create(D3DXVECTOR3(posX_L, 0.0f, Z_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt_L = 0;
	}

	float posX_R = (float)universal::RandRange(MAX_RANGE_RIGHT, MIN_RANGE_RIGHT);
	type = (CBgIce::TYPE)universal::RandRange(CBgIce::TYPE_MAX, CBgIce::TYPE_BIG);

	if (m_nBgiceCnt_R >= BGICE_CREATE_CNT_R)
	{
		// �w�i�X�̃��[�h
		CBgIce::Create(D3DXVECTOR3(posX_R, 0.0f, Z_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt_R = 0;
	}
}

//====================================================
// ����������w�i�X���o�Ă��鏈��
//====================================================
void COcean::BgIceSetPosDown(void)
{
	float posX_L = (float)universal::RandRange(MAX_RANGE_LEFT, MIN_RANGE_LEFT);
	CBgIce::TYPE type = (CBgIce::TYPE)universal::RandRange(CBgIce::TYPE_MAX, CBgIce::TYPE_BIG);

	m_nBgiceCnt_L++;
	m_nBgiceCnt_R++;

	if (m_nBgiceCnt_L >= BGICE_CREATE_CNT_L)
	{
		// �w�i�X�̃��[�h
		CBgIce::Create(D3DXVECTOR3(posX_L, 0.0f, Z_DOWN), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt_L = 0;
	}

	float posX_R = (float)universal::RandRange(MAX_RANGE_RIGHT, MIN_RANGE_RIGHT);
	type = (CBgIce::TYPE)universal::RandRange(CBgIce::TYPE_MAX, CBgIce::TYPE_BIG);

	if (m_nBgiceCnt_R >= BGICE_CREATE_CNT_R)
	{
		// �w�i�X�̃��[�h
		CBgIce::Create(D3DXVECTOR3(posX_R, 0.0f, Z_DOWN), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt_R = 0;
	}
}
