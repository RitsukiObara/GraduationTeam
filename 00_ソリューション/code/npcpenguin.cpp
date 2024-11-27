//*****************************************************
//
// �v���C���[����Ȃ��y���M��[npcpenguin.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "npcpenguin.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"
#include "shadow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
	const float SCALE_BODY = 1.8f;	// �̂̃X�P�[��
	
	const float FACT_DECREASE_MOVE = 0.9f;	// �ړ��ʂ̌����W��
	const float ADD_MOVE = 4.0f;	// �ړ��̒ǉ���
	
	const float RADIUS_COLLISION = 200.0f;	// ���̔���̔��a
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CNPCPenguin::CNPCPenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
	nCntMove = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CNPCPenguin::~CNPCPenguin()
{

}

//=====================================================
// ��������
//=====================================================
CNPCPenguin* CNPCPenguin::Create(INPCPenguinState* pState)
{
	CNPCPenguin *pNPCPenguin = nullptr;

	pNPCPenguin = new CNPCPenguin;

	if (pNPCPenguin != nullptr)
	{
		pNPCPenguin->Init();
		if (pState != nullptr)
		{
			pNPCPenguin->SetState(pState);
		}
	}

	return pNPCPenguin;
}

//=====================================================
// �X�e�[�g�ݒ菈��
//=====================================================
void CNPCPenguin::SetState(INPCPenguinState* pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit();
		m_pState = nullptr;
	}
	
	m_pState = pState;
	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// ����������
//=====================================================
HRESULT CNPCPenguin::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// �X�P�[���̐ݒ�
	SetScale(SCALE_BODY);

	// ���̔��萶��
	m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->SetRadius(RADIUS_COLLISION);
		m_pCollisionSphere->SetPosition(GetPosition());
	}

	// �e�̐���
	m_pShadow = CShadow::Create(3);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNPCPenguin::Uninit(void)
{
	// �e�폜
	Object::DeleteObject((CObject**)&m_pShadow);

	if (m_pState != nullptr)
	{
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CNPCPenguin::Update(void)
{
	// ���[�V�����X�V
	CMotion::Update();

	D3DXVECTOR3 pos = GetPosition();

	if (m_pCollisionSphere != nullptr)
	{// ���̔���̒Ǐ]
		m_pCollisionSphere->SetPosition(pos);

		// �u���b�N����
		m_pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// �L�����̈ʒu���f
		SetPosition(pos);
	}

	// �e�̒Ǐ]
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(pos);

	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//=====================================================
// ���[�V�������
//=====================================================
void CNPCPenguin::MotionState(void)
{
	nCntMove++;

	if (nCntMove > 70)
	{
		int nRandNextMotion = universal::RandRange(CNPCPenguin::MOTION::MOTION_WALK, CNPCPenguin::MOTION::MOTION_WALK);


		nCntMove = 0;
	}

	//else
	//{
	//	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);
	//}
}

//=====================================================
// �`�揈��
//=====================================================
void CNPCPenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}
