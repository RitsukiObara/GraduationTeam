//*****************************************************
//
// ���U���g�̃A�U���V[resultseal.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultseal.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"
#include "shadow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// �{�f�B�̃p�X
	const float SCALE_BODY = 1.8f;	// �̂̃X�P�[��
	
	const float FACT_DECREASE_MOVE = 0.9f;	// �ړ��ʂ̌����W��
	const float ADD_MOVE = 4.0f;	// �ړ��̒ǉ���
	
	const float RADIUS_COLLISION = 200.0f;	// ���̔���̔��a

	const int MOVE_COUNT = 50;	// �O�C�b�Ɠ����܂ł̎���
	const float ROTATE_SPEED = 0.008f * D3DX_PI;	// ��]�ʁi����Ɋ�����������j
	const float ROTATE_COEF = 0.9f;				// ��]�̊���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultSeal::CResultSeal(D3DXVECTOR3 pos, int nPriority) : CMotion(nPriority),c_centorPos(pos)
{
	m_pCollisionSphere = nullptr;
	m_nCounterMove = 0;
	m_bMove = false;
	m_fRotateSpeed = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResultSeal::~CResultSeal()
{

}

//=====================================================
// ��������
//=====================================================
CResultSeal* CResultSeal::Create(D3DXVECTOR3 pos)
{
	CResultSeal *pNPCPenguin = nullptr;

	pNPCPenguin = new CResultSeal(pos);

	if (pNPCPenguin != nullptr)
	{
		pNPCPenguin->Init();
	}

	return pNPCPenguin;
}

//=====================================================
// ����������
//=====================================================
HRESULT CResultSeal::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// ���[�V���������ݒ�
	SetMotion(MOTION_WALK);

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
	m_pShadow = CShadow::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CResultSeal::Uninit(void)
{
	// �e�폜
	Object::DeleteObject((CObject**)&m_pShadow);

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CResultSeal::Update(void)
{
	// ���[�V�����X�V
	CMotion::Update();

	// �ړ�����
	Move();

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
}

//=====================================================
// �ړ�����
//=====================================================
void CResultSeal::Move(void)
{
	m_nCounterMove++;
	if (m_nCounterMove >= MOVE_COUNT)
	{
		m_nCounterMove = 0;
		m_fRotateSpeed = ROTATE_SPEED;
	}

	float length = D3DXVec3Length(&(c_centorPos - GetPosition()));
	D3DXVECTOR3 rot = GetRotation();

	// �ړ�������ς���
	rot.y += m_fRotateSpeed;
	rot.y = fmodf(rot.y + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI;

	// �ʒu�v�Z
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, GetPosition().y, 0.0f);
	pos.x = -sinf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.x;
	pos.z = -cosf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.z;

	// �ʒu�����ݒ�
	SetPosition(pos);
	SetRotation(rot);

	// ��]�Ɋ���
	m_fRotateSpeed = m_fRotateSpeed * ROTATE_COEF;
}

//=====================================================
// �`�揈��
//=====================================================
void CResultSeal::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}
