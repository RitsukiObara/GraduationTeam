//*****************************************************
//
// �X�e�[�W�I���y���M��[slectStagePenguin.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStagePenguin.h"
#include "inputManager.h"
#include "collision.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
const float SCALE_BODY = 2.5f;	// �̂̃X�P�[��

const float FACT_DECREASE_MOVE = 0.9f;	// �ړ��ʂ̌����W��
const float ADD_MOVE = 4.0f;	// �ړ��̒ǉ���

const float RADIUS_COLLISION = 200.0f;	// ���̔���̔��a
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pInputMgr(nullptr), m_pClsnSphere(nullptr)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSelectStagePenguin::~CSelectStagePenguin()
{

}

//=====================================================
// ��������
//=====================================================
CSelectStagePenguin *CSelectStagePenguin::Create(void)
{
	CSelectStagePenguin *pSelectStagePenguin = nullptr;

	pSelectStagePenguin = new CSelectStagePenguin;

	if (pSelectStagePenguin != nullptr)
	{
		pSelectStagePenguin->Init();
	}

	return pSelectStagePenguin;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectStagePenguin::Init(void)
{
	// ���̓}�l�[�W���[����
	m_pInputMgr = CInputManager::Create();

	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// �X�P�[���̐ݒ�
	SetScale(SCALE_BODY);

	// ���̔��萶��
	m_pClsnSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pClsnSphere != nullptr)
	{
		m_pClsnSphere->SetRadius(RADIUS_COLLISION);
		m_pClsnSphere->SetPosition(GetPosition());
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectStagePenguin::Uninit(void)
{
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectStagePenguin::Update(void)
{
	// ���͏���
	Input();

	// �ړ��ʕ��ړ�
	AddPosition(m_move);

	// �ړ��ʂ̌���
	m_move *= FACT_DECREASE_MOVE;

	if (m_pClsnSphere != nullptr)
	{// ���̔���̒Ǐ]
		D3DXVECTOR3 pos = GetPosition();

		m_pClsnSphere->SetPosition(pos);

		// �u���b�N����
		m_pClsnSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// �L�����̈ʒu���f
		SetPosition(pos);
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ����
//=====================================================
void CSelectStagePenguin::Input(void)
{
	// �ړ�����
	// �ڕW�����̐ݒ�
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	D3DXVec3Normalize(&axisMove, &axisMove);
	m_move += axisMove * ADD_MOVE;
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CSelectStagePenguin::Debug(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}