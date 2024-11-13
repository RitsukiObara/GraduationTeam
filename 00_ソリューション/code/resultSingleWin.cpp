//*****************************************************
//
// �V���O���������U���g�̏���[resultSingleWin.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultSingleWin.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "inputManager.h"
#include "fade.h"
#include "score.h"
#include "game.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace scoreCaption
{
const string PATH_TEX[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
{// �L���v�V�����̃p�X
	"data\\TEXTURE\\UI\\Restart.png",
	"data\\TEXTURE\\UI\\Resume.png",
};
const float TIME_APPER = 2.0f;	// �o���ɂ����鎞��
const float WIDTH = 0.16f;	// ��
const float HEIGHT = 0.1f;	// ����
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
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
const D3DXVECTOR3 POS_INIT[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
{// �����ʒu
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// �ڕW�̍���
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// �����̍���
}

namespace score
{
const int NUM_PLACE[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// �X�R�A�̌���
}
}

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
CResultSingleWin::FuncUpdateState CResultSingleWin::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,									// �������Ȃ��X�V
	&CResultSingleWin::UpdateMoveCamera,		// �J�����ړ��̍X�V
	&CResultSingleWin::UpdateApperScore,		// �X�R�A�o����Ԃ̍X�V
	&CResultSingleWin::UpdateApperRanking,		// �����L���O�o����Ԃ̍X�V
	&CResultSingleWin::UpdateWait,				// �ҋ@��Ԃ̍X�V
	nullptr,									// �I����Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CResultSingleWin::CResultSingleWin() : m_fTimer(0.0f), m_apCaptionScore(),m_apNumberOwn()
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CResultSingleWin::~CResultSingleWin()
{

}

//====================================================
// ����������
//====================================================
HRESULT CResultSingleWin::Init(void)
{
	// �e�N���X�̏�����
	CResultSingle::Init();

	// 2D�I�u�W�F�N�g�̐���
	Create2D();

	// ��Ԃ̏�����
	m_state = E_State::STATE_MOVECAMERA;

	// �J�����̐ݒ�
	Camera::ChangeState(new CCameraResultSingle(this));

	return S_OK;
}

//====================================================
// 2D�I�u�W�F�N�g�̐���
//====================================================
void CResultSingleWin::Create2D(void)
{
	// ���g�̃X�R�A�̐���
	CreatepOwnScore();
}

//====================================================
// ���g�̃X�R�A�̐���
//====================================================
void CResultSingleWin::CreatepOwnScore(void)
{
	//----------------------------
	// �X�R�A�̎擾
	//----------------------------
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr)
		return;

	CScore *pScore = pGame->GetScore();
	if (pScore == nullptr)
		return;

	int nScore = pScore->GetScore();

	//----------------------------
	// �˂����񐔂̎擾
	//----------------------------
	int nTimePeck = 0;

	//----------------------------
	// �|���S���̐���
	//----------------------------
	int aData[E_ScoreCaption::CAPTION_MAX] = { nScore,nTimePeck };	// �����̒l
	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		// �L���v�V�����̐���
		m_apCaptionScore[i] = CUI::Create();
		if (m_apCaptionScore[i] == nullptr)
			continue;

		m_apCaptionScore[i]->SetSize(scoreCaption::WIDTH, scoreCaption::HEIGHT);
		m_apCaptionScore[i]->SetPosition(scoreCaption::POS_INIT[i]);
		int nIdxTexture = Texture::GetIdx(&scoreCaption::PATH_TEX[i][0]);
		m_apCaptionScore[i]->SetIdxTexture(nIdxTexture);
		m_apCaptionScore[i]->SetVtx();

		// �����̐���
		m_apNumberOwn[i] = CNumber::Create(score::NUM_PLACE[i], aData[i]);
		if (m_apNumberOwn[i] == nullptr)
			continue;

		m_apNumberOwn[i]->SetPosition(scoreNumber::POS_INIT[i]);
		m_apNumberOwn[i]->SetSizeAll(scoreNumber::SIZE_INIT, scoreNumber::SIZE_INIT);
	}
}

//====================================================
// �I������
//====================================================
void CResultSingleWin::Uninit(void)
{
	CResultSingle::Uninit();
}

//====================================================
// �X�V����
//====================================================
void CResultSingleWin::Update(void)
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

//=====================================================
// �J�����ړ���Ԃ̍X�V
//=====================================================
void CResultSingleWin::UpdateMoveCamera(void)
{
	if (GetState() == CResultSingle::E_State::STATE_ENDFADE)
		m_state = E_State::STATE_APPERSCORE;
}

//=====================================================
// �X�R�A�o����Ԃ̍X�V����
//=====================================================
void CResultSingleWin::UpdateApperScore(void)
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
		m_state = E_State::STATE_WAIT;
	}
}

//=====================================================
// �����L���O�o����Ԃ̍X�V����
//=====================================================
void CResultSingleWin::UpdateApperRanking(void)
{
	
}

//=====================================================
// �ҋ@��Ԃ̍X�V����
//=====================================================
void CResultSingleWin::UpdateWait(void)
{
	vector<CInputManager*> aInputMgr = CInputManager::GetArray();

	for (CInputManager *pInputMgr : aInputMgr)
	{
		if (pInputMgr == nullptr)
			continue;

		if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		{// �t�F�[�h���ă^�C�g���ɖ߂�
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
				pFade->SetFade(CScene::MODE::MODE_TITLE);
		}
	}
}

//====================================================
// �`�揈��
//====================================================
void CResultSingleWin::Draw(void)
{
	// �e�N���X�̕`��
	CResultSingle::Draw();
}