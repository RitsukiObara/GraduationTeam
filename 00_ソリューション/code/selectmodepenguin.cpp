//*****************************************************
//
// ���[�h�I���y���M��[selectmodepenguin.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectmodepenguin.h"
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
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectModePenguin::CSelectModePenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSelectModePenguin::~CSelectModePenguin()
{

}

//=====================================================
// ��������
//=====================================================
CSelectModePenguin* CSelectModePenguin::Create(ISelectModePenguinState* pState)
{
	CSelectModePenguin *pSelectModePenguin = nullptr;

	pSelectModePenguin = new CSelectModePenguin;

	if (pSelectModePenguin != nullptr)
	{
		pSelectModePenguin->Init();
		if (pState != nullptr)
		{
			pSelectModePenguin->SetState(pState);
		}
	}

	return pSelectModePenguin;
}

//=====================================================
// �X�e�[�g�ݒ菈��
//=====================================================
void CSelectModePenguin::SetState(ISelectModePenguinState* pState)
{
	m_pState = pState;
	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguin::Init(void)
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
void CSelectModePenguin::Uninit(void)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguin::Update(void)
{
	// �ړ��ʕ��ړ�
	Translate(m_move);

	// �ړ��ʂ̌���
	m_move *= FACT_DECREASE_MOVE;

	// ���[�V�����̊Ǘ�
	ManageMotion();

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
// �`�揈��
//=====================================================
void CSelectModePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}

//=====================================================
// �W�����v�̏I��
//=====================================================
void CSelectModePenguin::EndJump(void)
{
	// �W�����v���[�V�����t���O��܂�
	m_fragMotion.bJump = false;

	// ���͂�L����
	EnableInput(true);
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CSelectModePenguin::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bJump)
	{// �W�����v��
		if (nMotion == MOTION::MOTION_LANDING)
		{// ���n���[�V�����I���ŃW�����v�I��
			if (bFinish)
				EndJump();
		}
		else if (nMotion == MOTION::MOTION_STARTJUMP)
		{// �W�����v�J�n���[�V�����I������̑J��
			if (bFinish)
			{
				SetMotion(MOTION::MOTION_STAYJUMP);
			}
		}
		else if (nMotion != MOTION::MOTION_STARTJUMP)	// �W�����v�J�n���[�V�����̊J�n
			SetMotion(MOTION::MOTION_STARTJUMP);
	}
	else if ((nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK) && !bFinish)
	{

	}
	else if (m_fragMotion.bWalk)
	{// �������[�V�����t���O�L��
		if (nMotion != MOTION::MOTION_WALK)
			SetMotion(MOTION::MOTION_WALK);
	}
	else
	{// �����t���O�������Ă��Ȃ����
		if (nMotion != MOTION::MOTION_NEUTRAL)
			SetMotion(MOTION::MOTION_NEUTRAL);
	}
}

//*****************************************************
// �����Ă邾���X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Stand::Init(CSelectModePenguin* pPenguin)
{
	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Stand::Update(CSelectModePenguin* pPenguin)
{
}
