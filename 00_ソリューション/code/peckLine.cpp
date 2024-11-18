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
#include "peckWave.h"
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
const int NUM_EDGE				= 20;								// �ӂ̐�
const float MOVE_TIME			= 0.05f;							// �ړ��ɂ����鎞��
const float WAIT_TIME			= 0.5f;								// �ҋ@����
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
CPeckLine::CPeckLine() : m_posDest(), m_state(E_State::STATE_NONE), m_fTimerWait(0.0f), m_col()
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
CPeckLine *CPeckLine::Create(D3DXCOLOR col, D3DXVECTOR3 pos)
{
	CPeckLine *pPeckLine = nullptr;

	pPeckLine = new CPeckLine;

	if (pPeckLine != nullptr)
	{
		pPeckLine->SetPosition(pos);
		pPeckLine->Init();
		pPeckLine->m_col = col;
	}

	return pPeckLine;
}

//====================================================
// ����������
//====================================================
HRESULT CPeckLine::Init(void)
{
	return S_OK;
}

//====================================================
// ���̐���
//====================================================
void CPeckLine::CreateLine(void)
{
	S_InfoLine *pInfo = new S_InfoLine;
	if (pInfo == nullptr)
		return;

	pInfo->posDest = m_posDest;
	pInfo->posInit = GetPosition();

	//---------------------------------
	// �O�Ր�[�p3D�I�u�W�F�N�g�̐���
	//---------------------------------
	pInfo->pPosOrbit = CObject3D::Create();

	if (pInfo->pPosOrbit == nullptr)
		return;

	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 vecDiff = m_posDest - posPlayer;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	pInfo->pPosOrbit->SetRotation(D3DXVECTOR3(0.0f, fRot, 0.0f));
	pInfo->pPosOrbit->SetPosition(posPlayer);

	pInfo->pPosOrbit->Draw();

	//---------------------------------
	// �O�Ղ̐���
	//---------------------------------
	D3DXMATRIX mtx = pInfo->pPosOrbit->GetMatrix();
	pInfo->pOrbit = COrbit::Create(mtx, orbit::OFFSET_LEFT, orbit::OFFSET_RIGHT, orbit::NUM_EDGE);

	if (pInfo->pOrbit == nullptr)
		return;

	// �v���C���[��ID�ɍ��킹���F�ɐݒ�
	pInfo->pOrbit->SetColor(m_col);

	// �z��ɒǉ�
	m_aInfoLine.push_back(pInfo);
}

//====================================================
// �I������
//====================================================
void CPeckLine::Uninit(void)
{
	for (S_InfoLine *pInfo : m_aInfoLine)
	{
		// ���g�̏I��
		pInfo->pPosOrbit->Uninit();
		pInfo->pPosOrbit = nullptr;

		pInfo->pOrbit->Uninit();
		pInfo->pOrbit = nullptr;
		
		delete pInfo;
	}

	m_aInfoLine.clear();

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

	// �S�Ă̐��̍X�V
	UpdateAllLine();
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
}

//====================================================
// �ړ���Ԃł̍X�V
//====================================================
void CPeckLine::UpdateMove(void)
{
	m_fTimerWait += CManager::GetDeltaTime();

	if (m_fTimerWait > orbit::WAIT_TIME)
	{// ���Ԍo�߂Ő��𐶐�
		CreateLine();

		m_fTimerWait = 0.0f;
	}
}

//====================================================
// �S�Ă̐��̍X�V
//====================================================
void CPeckLine::UpdateAllLine(void)
{
	for (auto it = m_aInfoLine.begin(); it != m_aInfoLine.end(); /* no increment here */)
	{
		S_InfoLine* pInfo = *it;

		if (pInfo == nullptr || pInfo->pOrbit == nullptr || pInfo->pPosOrbit == nullptr)
		{
			it = m_aInfoLine.erase(it); // �����ȗv�f�����X�g����폜
			continue;
		}

		//---------------------------------
		// �ʒu�̍X�V
		//---------------------------------
		// ��莞�Ԍo�߂ňړ��I��
		if (pInfo->fTimer > orbit::MOVE_TIME)
		{
			pInfo->fTimer = 0.0f;

			// �g��G�t�F�N�g�̐���
			CPeckWave::Create(pInfo->posDest, pInfo->pOrbit->GetColor(0));

			// �O�Ղ�Ɨ�������
			pInfo->pOrbit->SetEnd(true);

			// ���g�̏I������
			pInfo->pPosOrbit->Uninit();
			pInfo->pPosOrbit = nullptr;

			pInfo->pOrbit = nullptr;

			delete pInfo;
			pInfo = nullptr;

			it = m_aInfoLine.erase(it); // �폜�����v�f�����X�g������S�ɍ폜
			continue;
		}

		pInfo->fTimer += CManager::GetDeltaTime();

		// �^�C�}�[�̊������v�Z
		float fRate = pInfo->fTimer / orbit::MOVE_TIME;

		// �I�u�W�F�N�g�̈ʒu��ݒ�
		D3DXVECTOR3 posOwner = pInfo->posInit;
		D3DXVECTOR3 posObject = universal::Lerp(posOwner, pInfo->posDest, fRate);

		// �����ɕ�������ǉ�
		float fParabola = universal::ParabolaY(fRate - 0.5f, orbit::FACT_PARABOLA);
		fParabola *= -orbit::HEIGHT_PARABOLA;
		fParabola += orbit::HEIGHT_PARABOLA * 2;
		posObject.y += fParabola;

		pInfo->pPosOrbit->SetPosition(posObject);

		//---------------------------------
		// �O�Ղ̒Ǐ]
		//---------------------------------
		D3DXMATRIX mtx = pInfo->pPosOrbit->GetMatrix();
		pInfo->pOrbit->SetOffset(mtx);

		++it; // ���[�v��i�߂�
	}
}

//====================================================
// �ړ��I��
//====================================================
void CPeckLine::EndMove(void)
{
	// �������ĂȂ���Ԃɐݒ�
	m_state = E_State::STATE_NONE;

	// �^�C�}�[���Z�b�g
	m_fTimerWait = orbit::WAIT_TIME;
}

//====================================================
// �`�揈��
//====================================================
void CPeckLine::Draw(void)
{

}