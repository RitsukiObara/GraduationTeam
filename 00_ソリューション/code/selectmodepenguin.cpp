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
	namespace Stand
	{
		const int MOTION_COUNT = 60 * 3;	// ���[�V���������鎞��
		const vector<CSelectModePenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CSelectModePenguin::MOTION_WALK,		// �ړ�
			CSelectModePenguin::MOTION_WALK,		// �ړ�
			CSelectModePenguin::MOTION_WALK,		// �ړ�
			CSelectModePenguin::MOTION_NECKSHAKE,	// ��U��
			CSelectModePenguin::MOTION_STOMACH,		// ���x�^�@
			CSelectModePenguin::MOTION_UNYO,		// �񂤂ɂ��
			CSelectModePenguin::MOTION_WINGPTPT		// �H�p�^
		};
	}
	namespace Move
	{
		const int MOTION_COUNT = 60 * 6;	// ���[�V���������鎞��
		const int FALL_CHANCE = 15;			// �]�Ԋm���i�l�F���j
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
CSelectModePenguin::CSelectModePenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
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
		m_pState->Uninit();
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
// �I������
//=====================================================
void CSelectModePenguinState_Stand::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Stand::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stand::MOTION_COUNT)
	{// �X�e�[�g���ԏI��
		// �ǂ̃X�e�[�g�ɑJ�ڂ��邩���I
		CSelectModePenguin::MOTION rnd = Stand::NEXT_MOTION_PATTERN[(rand() % (int)Stand::NEXT_MOTION_PATTERN.size())];
		switch (rnd)
		{
		case CSelectModePenguin::MOTION_WALK:
			pPenguin->SetState(new CSelectModePenguinState_Move);
			break;
		case CSelectModePenguin::MOTION_NECKSHAKE:
			pPenguin->SetState(new CSelectModePenguinState_ShakeHead);
			break;
		case CSelectModePenguin::MOTION_STOMACH:
			pPenguin->SetState(new CSelectModePenguinState_Stomach);
			break;
		case CSelectModePenguin::MOTION_UNYO:
			pPenguin->SetState(new CSelectModePenguinState_UNYO);
			break;
		case CSelectModePenguin::MOTION_WINGPTPT:
			pPenguin->SetState(new CSelectModePenguinState_WingPTPT);
			break;
		default:
			m_nCounter = 0;
			break;
		}
	}
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
// �I������
//=====================================================
void CSelectModePenguinState_Move::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Move::Update(CSelectModePenguin* pPenguin)
{
	// �ړ�
	D3DXVECTOR3 rot = pPenguin->GetRotation();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	move.x = -sinf(rot.y) * Move::MOVE_SPEED;
	move.z = -cosf(rot.y) * Move::MOVE_SPEED;

	pPenguin->Translate(move);

	// ���
	rot.y += 0.005f * D3DX_PI;
	pPenguin->SetRotation(rot);

	// �X�e�[�g�I���J�E���g
	m_nCounter++;
	if (m_nCounter >= Move::MOTION_COUNT)
	{// �X�e�[�g���ԏI��
		// �R�P�`�����X
		int rnd = rand() % 100;
		if (rnd < Move::FALL_CHANCE)
		{// �R�P
			pPenguin->SetState(new CSelectModePenguinState_Fall);
		}
		else
		{// ���ʂɗ���
			pPenguin->SetState(new CSelectModePenguinState_Stand);
		}
	}
}

//*****************************************************
// �]�уX�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Fall::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_FALL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectModePenguinState_Fall::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Fall::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// �R�P�I�����
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// ��U��X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_ShakeHead::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NECKSHAKE);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectModePenguinState_ShakeHead::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_ShakeHead::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= ShakeHead::MOTION_COUNT)
	{// �X�e�[�g���ԏI��
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// ���x�^�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_Stomach::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_STOMACH);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectModePenguinState_Stomach::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_Stomach::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stomach::MOTION_COUNT)
	{// �X�e�[�g���ԏI��
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// �񂤂˃X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_UNYO::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_UNYO);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectModePenguinState_UNYO::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_UNYO::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// �񂤂ɂ��I�����
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// �H�p�^�p�^�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CSelectModePenguinState_WingPTPT::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WINGPTPT);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectModePenguinState_WingPTPT::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CSelectModePenguinState_WingPTPT::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// �H�p�^�I�����
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}
