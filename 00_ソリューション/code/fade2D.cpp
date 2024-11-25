//*****************************************************
//
// 2D�t�F�[�h�̏���[fade2D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fade2D.h"
#include "polygon2D.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CFade2D::FuncUpdateState CFade2D::s_aFuncUpdateState[] =
{
	nullptr,			// �������Ă��Ȃ���Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CFade2D::CFade2D() : m_state(E_State::STATE_NONE), m_fTimer(0.0f)
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CFade2D::~CFade2D()
{

}

//====================================================
// ��������
//====================================================
CFade2D *CFade2D::Create(CPolygon2D *pPolygon, float fTimeMax)
{
	CFade2D *pFade2D = nullptr;

	pFade2D = new CFade2D;

	if (pFade2D != nullptr)
	{
		pFade2D->m_pPoygon = pPolygon;
		pFade2D->m_fTimeMax = fTimeMax;
		pFade2D->Init();
	}

	return pFade2D;
}

//====================================================
// ����������
//====================================================
HRESULT CFade2D::Init(void)
{
	return S_OK;
}

//====================================================
// �I������
//====================================================
void CFade2D::Uninit(void)
{
	Release();
}

//====================================================
// �X�V����
//====================================================
void CFade2D::Update(void)
{
	switch (m_state)
	{
	case CFade2D::STATE_IN:
		m_fTimer += CManager::GetDeltaTime();
		break;
	case CFade2D::STATE_OUT:
		m_fTimer -= CManager::GetDeltaTime();
		break;
	default:
		break;
	}

	universal::LimitValuefloat(&m_fTimer, m_fTimeMax, 0.0f);

	float fRate = easing::EaseInOutExpo(m_fTimer);

	// �F��ݒ�
	if (m_pPoygon != nullptr)
		m_pPoygon->SetAlpha(fRate);
}

//====================================================
// ��Ԃɂ�鏉����
//====================================================
void CFade2D::InitByState(E_State state)
{
	switch (state)
	{
	case CFade2D::STATE_IN:
		m_fTimer = 0.0f;
		break;
	case CFade2D::STATE_OUT:
		m_fTimer = m_fTimeMax;
		break;
	default:
		break;
	}

	SetState(state);
}

//====================================================
// �`�揈��
//====================================================
void CFade2D::Draw(void)
{

}