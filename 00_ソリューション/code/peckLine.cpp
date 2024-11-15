//*****************************************************
//
// �������̏���[peckLine.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "peckLine.h"
#include "player.h"
#include "object3D.h"
#include "orbit.h"
#include "manager.h"
#include "effect3D.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
//--------------------------
// �O�Ղ̒萔
//--------------------------
namespace orbit
{
const D3DXVECTOR3 OFFSET_LEFT	= { 10.0f,0.0f,0.0f };				// �����̃I�t�Z�b�g
const D3DXVECTOR3 OFFSET_RIGHT	= { -10.0f,0.0f,0.0f };				// �E���̃I�t�Z�b�g
const int NUM_EDGE				= 40;								// �ӂ̐�
const float MOVE_TIME			= 0.2f;								// �ړ��ɂ����鎞��
const float WAIT_TIME			= 0.7f;								// �ҋ@����
const float FACT_PARABOLA		= 10.0f;							// �������̌W��
const D3DXCOLOR COL_INIT		= D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);	// �O�Ղ̐F
const float HEIGHT_PARABOLA		= 100.0f;							// �������̍���
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPeckLine::FuncUpdateState CPeckLine::s_aFuncUpdateState[] =
{
	nullptr,				// �������Ă��Ȃ���Ԃ̍X�V
	&CPeckLine::UpdateMove,	// �����Ă��Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CPeckLine::CPeckLine() : m_pPlayer(nullptr), m_pPosOrbit(nullptr),m_pOrbit(nullptr), m_fTimer(0.0f), m_posDest(), m_state(E_State::STATE_NONE), m_fTimerWait(0.0f)
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CPeckLine::~CPeckLine()
{

}

//====================================================
// ��������
//====================================================
CPeckLine *CPeckLine::Create(CPlayer *pPlayer)
{
	CPeckLine *pPeckLine = nullptr;

	pPeckLine = new CPeckLine;

	if (pPeckLine != nullptr)
	{
		pPeckLine->m_pPlayer = pPlayer;
		pPeckLine->Init();
	}

	return pPeckLine;
}

//====================================================
// ����������
//====================================================
HRESULT CPeckLine::Init(void)
{
	assert(m_pPlayer != nullptr);	// �v���C���[�n���Y��p

	// �I�u�W�F�N�g�̐���
	CreateObject();

	return S_OK;
}

//====================================================
// �I�u�W�F�N�g����
//====================================================
void CPeckLine::CreateObject(void)
{
	//--------------------------
	// �O�Ր�[�p3D�I�u�W�F�N�g�̐���
	//--------------------------
	m_pPosOrbit = CObject3D::Create();

	if (m_pPosOrbit == nullptr)
		return;

	//--------------------------
	// �O�Ղ̐���
	//--------------------------
	D3DXMATRIX mtx = m_pPosOrbit->GetMatrix();
	m_pOrbit = COrbit::Create(mtx, orbit::OFFSET_LEFT, orbit::OFFSET_RIGHT, orbit::NUM_EDGE);

	if (m_pOrbit == nullptr)
		return;

	//m_pOrbit->EnableZtest(true);
}

//====================================================
// �I������
//====================================================
void CPeckLine::Uninit(void)
{
	Release();
}

//====================================================
// �X�V����
//====================================================
void CPeckLine::Update(void)
{
	// ��Ԃ��Ƃ̍X�V
	if(s_aFuncUpdateState[m_state] != nullptr)
		(this->*(s_aFuncUpdateState[m_state]))();
}

//====================================================
// �ړ��J�n
//====================================================
void CPeckLine::StartMove(D3DXVECTOR3 posDest)
{
	// �ړ���Ԃɐݒ�
	m_state = E_State::STATE_MOVE;

	// �ڕW�ʒu��ݒ�
	m_posDest = posDest;

	if (m_pPosOrbit != nullptr)
	{
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posDest - posPlayer;

		float fRot = atan2f(vecDiff.x, vecDiff.z);

		m_pPosOrbit->SetRotation(D3DXVECTOR3(0.0f, fRot, 0.0f));
	}
}

//====================================================
// �ړ���Ԃł̍X�V
//====================================================
void CPeckLine::UpdateMove(void)
{
	m_fTimerWait += CManager::GetDeltaTime();

	// �O�Ղ̒Ǐ]
	FollowOrbit();

	if (m_fTimerWait < orbit::WAIT_TIME)
		return;

	m_fTimer += CManager::GetDeltaTime();

	// �^�C�}�[�̊������v�Z
	float fRate = m_fTimer / orbit::MOVE_TIME;

	// �I�u�W�F�N�g�̈ʒu��ݒ�
	D3DXVECTOR3 posOwner = m_pPlayer->GetPosition();
	D3DXVECTOR3 posObject = universal::Lerp(posOwner, m_posDest, fRate);

	// �����ɕ�������ǉ�
	float fParabola = universal::ParabolaY(fRate - 0.5f, orbit::FACT_PARABOLA);
	fParabola *= -orbit::HEIGHT_PARABOLA; 
	fParabola += orbit::HEIGHT_PARABOLA;
	posObject.y += fParabola;

	m_pPosOrbit->SetPosition(posObject);

	// ��莞�Ԍo�߂ňړ��I��
	if (m_fTimer > orbit::MOVE_TIME)
	{
		m_fTimerWait = 0.0f;
		m_fTimer = 0.0f;
	}
}

//====================================================
// �O�Ղ̒Ǐ]
//====================================================
void CPeckLine::FollowOrbit(void)
{
	if (m_pOrbit == nullptr || m_pPosOrbit == nullptr)
		return;

	D3DXMATRIX mtx = m_pPosOrbit->GetMatrix();

	m_pOrbit->SetOffset(mtx);
	m_pOrbit->SetColor(orbit::COL_INIT);
}

//====================================================
// �ړ��I��
//====================================================
void CPeckLine::EndMove(void)
{
	// �������ĂȂ���Ԃɐݒ�
	m_state = E_State::STATE_NONE;

	// �^�C�}�[���Z�b�g
	m_fTimer = 0.0f;
	m_fTimerWait = orbit::WAIT_TIME;
}

//====================================================
// �`�揈��
//====================================================
void CPeckLine::Draw(void)
{

}