//*****************************************************
//
// �V���O�����U���g�̏���[resultSingle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultSingle.h"
#include "manager.h"
#include "ui.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "timer.h"
#include "ranking.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace bg
{
const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// �p�X
const float WIDTH = 0.5f;	// ��
const float HEIGHT = 0.5f;	// ����
const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �����̐F
const float DEST_ALPHA = 0.5f;	// �ڕW�F
const float DIFF_ALPHA = DEST_ALPHA - INIT_COL.a;			// �����A���t�@�l
const D3DXVECTOR3 POS = D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �ʒu
const float TIME_FADE = 2.0f;	// �t�F�[�h�ɂ����鎞��
}

namespace caption
{
const float	MOVE_TIME = 1.5f;	// �ړ�����
const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �ڕW�F
const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// �����F
const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// �����F
const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(0.5f, 0.2f, 0.0f);			// �ڕW�ʒu
const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// �����ʒu
const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;						// �����ʒu
const float WIDTH = 0.2f;		// ��
const float HEIGHT = 0.17f;	// ����
}

namespace scoreCaption
{
const float TIME_APPER = 2.0f;	// �o���ɂ����鎞��
const float WIDTH = 0.16f;	// ��
const float HEIGHT = 0.1f;	// ����
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingle::E_ScoreCaption::CAPTION_MAX] =
{// �����ʒu
	{ 0.3f,-HEIGHT_INIT,0.0f },
	{ 0.7f,-HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// �ڕW�̍���
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// �����̍���
}

namespace scoreNumber
{
const float SIZE_INIT = 0.03f;	// �T�C�Y
const float HEIGHT_INIT = -0.1f;
const D3DXVECTOR3 POS_INIT[CResultSingle::E_ScoreCaption::CAPTION_MAX] =
{// �����ʒu
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// �ڕW�̍���
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// �����̍���
}

namespace score
{
const int NUM_PLACE[CResultSingle::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// �X�R�A�̌���
}
}

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
CResultSingle::FuncUpdateState CResultSingle::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// �������Ȃ��X�V
	nullptr,					// �J�����ړ��̍X�V
	&CResultSingle::UpdateFade,		// �t�F�[�h��Ԃ̍X�V
	&CResultSingle::UpdateApperScore,		// �t�F�[�h��Ԃ̍X�V
	&CResultSingle::UpdateSelect,		// �I����Ԃ̍X�V
	nullptr,					// �I����Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CResultSingle::CResultSingle() : m_fTimer(0.0f), m_apCaptionScore(),m_apNumberOwn(),m_bWin(false)
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CResultSingle::~CResultSingle()
{

}

//====================================================
// ��������
//====================================================
CResultSingle *CResultSingle::Create(bool bWin)
{
	CResultSingle *pResult = nullptr;

	pResult = new CResultSingle;

	if (pResult != nullptr)
	{
		pResult->m_bWin = bWin;
		pResult->Init();
	}

	return pResult;
}

//====================================================
// ����������
//====================================================
HRESULT CResultSingle::Init(void)
{
	// �Q�[����ʂ����U���g��Ԃɂ���
	CGame::SetState(CGame::E_State::STATE_RESULT);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	// �T�E���h�̍Đ�
	pSound->Play(CSound::LABEL_BGM_RESULT);

	// 2D�I�u�W�F�N�g�̐���
	Create2D(m_bWin);

	// ��Ԃ̏����ݒ�
	if (m_bWin)
	{
		m_state = STATE_MOVECAMERA;

		Camera::ChangeState(new CCameraResultSingle(this));
	}
	else
		m_state = STATE_FADE;

	return S_OK;
}

//====================================================
// 2D�I�u�W�F�N�g�̐���
//====================================================
void CResultSingle::Create2D(bool bWin)
{
	//-----------------------------------------
	// �w�i�̐���
	//-----------------------------------------
	m_pBg = CUI::Create();

	if (m_pBg == nullptr)
		return;

	m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
	m_pBg->SetPosition(bg::POS);
	m_pBg->SetCol(bg::INIT_COL);
	m_pBg->SetVtx();

	//-----------------------------------------
	// ���g�̐��ѕ\���̐���
	//-----------------------------------------
	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		// �L���v�V�����̐���
		m_apCaptionScore[i] = CUI::Create();
		if (m_apCaptionScore[i] == nullptr)
			continue;

		m_apCaptionScore[i]->SetSize(scoreCaption::WIDTH, scoreCaption::HEIGHT);
		m_apCaptionScore[i]->SetPosition(scoreCaption::POS_INIT[i]);
		m_apCaptionScore[i]->SetVtx();

		// �����̐���
		m_apNumberOwn[i] = CNumber::Create(score::NUM_PLACE[i], 0);
		if (m_apNumberOwn[i] == nullptr)
			continue;

		m_apNumberOwn[i]->SetPosition(scoreNumber::POS_INIT[i]);
		m_apNumberOwn[i]->SetSizeAll(scoreNumber::SIZE_INIT, scoreNumber::SIZE_INIT);
	}
}

//====================================================
// �I������
//====================================================
void CResultSingle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pBg);
	Object::DeleteObject((CObject**)&m_pCaption);

	Release();
}

//====================================================
// �X�V����
//====================================================
void CResultSingle::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// �t�F�[�h��Ԃ̍X�V����
//=====================================================
void CResultSingle::UpdateFade(void)
{
	if (m_pBg == nullptr)
		return;

	m_fTimer += CManager::GetDeltaTime();

	// �^�C�}�[�̃C�[�W���O
	float fTime = m_fTimer / bg::TIME_FADE;
	float fRate = easing::EaseOutExpo(fTime);

	// �F�̐ݒ�
	m_pBg->SetAlpha(bg::INIT_COL.a + bg::DIFF_ALPHA * fRate);

	if (bg::TIME_FADE < m_fTimer)
	{// �t�F�[�h��Ԃ̏I��
		m_state = E_State::STATE_APPERSCORE;
		m_fTimer = 0.0f;
		Camera::ChangeState(nullptr);

		return;
	}
}

//=====================================================
// �X�R�A�o����ԏ�Ԃ̍X�V����
//=====================================================
void CResultSingle::UpdateApperScore(void)
{
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / scoreCaption::TIME_APPER;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		//-----------------------------------------
		// �L���v�V�����̈ړ�
		//-----------------------------------------
		if (m_apCaptionScore[i] == nullptr)
			continue;

		D3DXVECTOR3 posCaption = m_apCaptionScore[i]->GetPosition();
		posCaption.y = scoreCaption::HEIGHT_INIT + scoreCaption::DIFF_HEIGHT * fRate;

		m_apCaptionScore[i]->SetPosition(posCaption);
		m_apCaptionScore[i]->SetVtx();

		//-----------------------------------------
		// �����̈ړ�
		//-----------------------------------------
		// �����̐���
		if (m_apNumberOwn[i] == nullptr)
			continue;

		D3DXVECTOR3 posNumber = m_apNumberOwn[i]->GetPosition();
		posNumber.y = scoreNumber::HEIGHT_INIT + scoreCaption::DIFF_HEIGHT * fRate;

		m_apNumberOwn[i]->SetPosition(posNumber);
	}

	if (m_fTimer > scoreCaption::TIME_APPER)
	{// ��莞�Ԍo�߂Ŏ��̏�ԂɈڂ�

	}
}

//=====================================================
// �I����Ԃ̍X�V����
//=====================================================
void CResultSingle::UpdateSelect(void)
{

}

//====================================================
// �`�揈��
//====================================================
void CResultSingle::Draw(void)
{

}

//====================================================
// �J�����ړ��̏I��
//====================================================
void CResultSingle::EndMove(void)
{
	m_state = E_State::STATE_FADE;
}