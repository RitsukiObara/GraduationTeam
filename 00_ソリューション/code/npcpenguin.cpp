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
	namespace Stand
	{
		const int MOTION_COUNT = 60 * 3;	// ���[�V���������鎞��
		const vector<CNPCPenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CNPCPenguin::MOTION_WALK,		// �ړ�
			CNPCPenguin::MOTION_WALK,		// �ړ�
			CNPCPenguin::MOTION_WALK,		// �ړ�
			CNPCPenguin::MOTION_NECKSHAKE,	// ��U��
			CNPCPenguin::MOTION_STOMACH,		// ���x�^�@
			CNPCPenguin::MOTION_UNYO,		// �񂤂ɂ��
			CNPCPenguin::MOTION_WINGPTPT		// �H�p�^
		};
	}
	namespace Move
	{
		const int MOTION_COUNT = 60 * 5 + 20;	// ���[�V���������鎞��
		const int FALL_CHANCE = 18;			// �R�P�m���i�l�F���j
		const float MOVE_SPEED = 2.5f;		// �ړ����x
	}
	namespace ShakeHead
	{
		const int MOTION_COUNT = 60 * 2;	// ���[�V���������鎞��
	}
	namespace Stomach
	{
		const int MOTION_COUNT = 60 * 3;	// ���[�V���������鎞��
	}
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

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNPCPenguin::Uninit(void)
{
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

	if (m_pCollisionSphere != nullptr)
	{// ���̔���̒Ǐ]
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);

		// �u���b�N����
		m_pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// �L�����̈ʒu���f
		SetPosition(pos);
	}

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
