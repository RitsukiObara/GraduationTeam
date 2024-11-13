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
namespace scoreCaption
{
const string PATH_TEX[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// �L���v�V�����̃p�X
	"data\\TEXTURE\\UI\\Restart.png",
	"data\\TEXTURE\\UI\\Resume.png",
};
const float TIME_APPER = 2.0f;	// �o���ɂ����鎞��
const float WIDTH = 0.16f;	// ��
const float HEIGHT = 0.1f;	// ����
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// �����ʒu
	{ 0.3f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// �ڕW�̍���
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// �����̍���
}

namespace scoreNumber
{
const float SIZE_INIT = 0.03f;	// �T�C�Y
const float HEIGHT_INIT = -0.1f;
const D3DXVECTOR3 POS_INIT[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// �����ʒu
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// �ڕW�̍���
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// �����̍���
}

namespace score
{
const int NUM_PLACE[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// �X�R�A�̌���
}
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