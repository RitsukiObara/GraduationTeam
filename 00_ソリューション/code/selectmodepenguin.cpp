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
	nCntMove = 0;
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
	if (m_pState != nullptr)
	{
		delete m_pState;
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
void CSelectModePenguin::MotionState(void)
{
	nCntMove++;

	if (nCntMove > 70)
	{
		int nRandNextMotion = universal::RandRange(CSelectModePenguin::MOTION::MOTION_WALK, CSelectModePenguin::MOTION::MOTION_WALK);


		nCntMove = 0;
	}

	//else
	//{
	//	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NEUTRAL);
	//}
}

//=====================================================
// �`�揈��
//=====================================================
void CSelectModePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}

//*****************************************************
// �����Ă邾���X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Stand::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Stand::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// �ړ��X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Move::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Move::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// �]�уX�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Fall::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Fall::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// ��U��X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_ShakeHead::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_ShakeHead::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// ���x�^�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Stomach::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Stomach::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// �񂤂˃X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_UNYO::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_UNYO::Update(CSelectModePenguin* pPenguin)
{

}

//*****************************************************
// �H�p�^�p�^�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_WingPTPT::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_WingPTPT::Update(CSelectModePenguin* pPenguin)
{

}
