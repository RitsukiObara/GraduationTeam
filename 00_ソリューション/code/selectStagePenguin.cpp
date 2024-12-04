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
#include "debugproc.h"
#include "ice.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
const float SCALE_BODY = 2.5f;		// �̂̃X�P�[��
const float RADIUS_ICE = 400.0f;	// �X�̔��a
const D3DXVECTOR3 POS_INIT = { 0.0f,50.0f,0.0f };	// �����ʒu

const float FACT_DECREASE_MOVE = 0.9f;	// �ړ��ʂ̌����W��
const float ADD_MOVE = 4.0f;	// �ړ��̒ǉ���

const float RADIUS_COLLISION = 200.0f;	// ���̔���̔��a

const float FACT_ROT = 0.2f;	// ��]�W��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pInputMgr(nullptr), m_pClsnSphere(nullptr), m_bInput(false), m_pIce(nullptr)
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

	// �X�P�[���̐ݒ�
	SetScale(SCALE_BODY);

	// �����ʒu�ݒ�
	SetPosition(POS_INIT);

	// ���̔��萶��
	m_pClsnSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pClsnSphere != nullptr)
	{
		m_pClsnSphere->SetRadius(RADIUS_COLLISION);
		m_pClsnSphere->SetPosition(GetPosition());
	}

	// ���̓t���O�𗧂ĂĂ���
	m_bInput = true;

	// ����̕X�̐���
	m_pIce = CIce::Create();
	if (m_pIce != nullptr)
		m_pIce->SetTransform(RADIUS_ICE);

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
	if(m_bInput)
		Input();	// ���͏���

	// �ړ��ʕ��ړ�
	Translate(m_move);

	// �ړ��ʂ̌���
	m_move *= FACT_DECREASE_MOVE;

	if (m_pClsnSphere != nullptr)
	{// ���̔���̒Ǐ]
		D3DXVECTOR3 pos = GetPosition();

		m_pClsnSphere->SetPosition(pos);

		// �u���b�N����
		m_pClsnSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		pos.y = POS_INIT.y;

		// �L�����̈ʒu���f
		SetPosition(pos);
	}

	// �ڕW�̌����ɕ␳����
	float fRotDest = atan2f(-m_move.x, -m_move.z);

	D3DXVECTOR3 rot = GetRotation();
	universal::FactingRot(&rot.y, fRotDest, FACT_ROT);
	SetRotation(rot);

	if (m_pIce != nullptr)
	{// �X�̒Ǐ]
		D3DXVECTOR3 posIce = m_pIce->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		m_pIce->SetPosition(D3DXVECTOR3(pos.x + m_move.x, posIce.y, pos.z + m_move.z));
		m_pIce->SetRotation(rot);

		// ������X�Ɠ����ɂ���
		SetPosition(D3DXVECTOR3(pos.x, posIce.y, pos.z));
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
// �G���^�[���͂̌��o
//=====================================================
bool CSelectStagePenguin::IsEnter(void)
{
	if (m_pInputMgr == nullptr)
		return false;

	return m_pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER);
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CSelectStagePenguin::Debug(void)
{
	CDebugProc::GetInstance()->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}