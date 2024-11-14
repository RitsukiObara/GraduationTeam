//*****************************************************
//
// �V���O���������U���g�̏���[resultSingleLose.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultSingleLose.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
CResultSingleLose::FuncUpdateState CResultSingleLose::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,									// �������Ȃ��X�V
	nullptr,									// �J�����ړ��̍X�V
	nullptr,									// �I����Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CResultSingleLose::CResultSingleLose() : m_fTimer(0.0f)
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CResultSingleLose::~CResultSingleLose()
{

}

//====================================================
// ����������
//====================================================
HRESULT CResultSingleLose::Init(void)
{
	// �e�N���X�̏�����
	CResultSingle::Init();

	// 2D�I�u�W�F�N�g�̐���
	Create2D();

	return S_OK;
}

//====================================================
// 2D�I�u�W�F�N�g�̐���
//====================================================
void CResultSingleLose::Create2D(void)
{

}

//====================================================
// �I������
//====================================================
void CResultSingleLose::Uninit(void)
{
	CResultSingle::Uninit();
}

//====================================================
// �X�V����
//====================================================
void CResultSingleLose::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// �e�N���X�̍X�V
	CResultSingle::Update();
}

//====================================================
// �`�揈��
//====================================================
void CResultSingleLose::Draw(void)
{
	// �e�N���X�̕`��
	CResultSingle::Draw();
}