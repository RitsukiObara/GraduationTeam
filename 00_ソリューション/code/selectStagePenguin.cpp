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
#include "MyEffekseer.h"
#include "player.h"

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

const int RIPPLE_CNT = 15;		// �g��o���b��

const float LINE_MOVE = 0.9f;	// �ړ����Ă锻�f�̂������l
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pInputMgr(nullptr), m_pClsnSphere(nullptr), m_bInput(false), m_pIce(nullptr), m_nRippleCnt(0)
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

	// �g�䂪�o��܂ł̃J�E���g
	m_nRippleCnt = 0;

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

	D3DXVECTOR3 rot = GetRotation();
	float fSpeed = D3DXVec3Length(&m_move);
	if (fSpeed > LINE_MOVE)
	{
		// �ڕW�̌����ɕ␳����
		float fRotDest = atan2f(-m_move.x, -m_move.z);

		universal::FactingRot(&rot.y, fRotDest, FACT_ROT);
		SetRotation(rot);
	}

	if (m_pIce != nullptr)
	{// �X�̒Ǐ]
		D3DXVECTOR3 posIce = m_pIce->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		m_pIce->SetPosition(D3DXVECTOR3(pos.x + m_move.x, posIce.y, pos.z + m_move.z));
		m_pIce->SetRotation(rot);

		// ������X�Ɠ����ɂ���
		SetPosition(D3DXVECTOR3(pos.x, posIce.y, pos.z));
	}

	// �g��o���J�E���g�����Z
	m_nRippleCnt++;

	if (m_nRippleCnt >= RIPPLE_CNT)
	{
		// �v���C���[�ɍ��킹�Ĕg����o��
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE_RIPPLE, m_pIce->GetPosition(), m_pIce->GetRotation(), D3DXVECTOR3(100.0f, 100.0f, 100.0f));

		m_nRippleCnt = 0;
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

}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}