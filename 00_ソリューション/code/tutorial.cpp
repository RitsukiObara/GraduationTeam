//*****************************************************
//
// �`���[�g���A������[tutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorial.h"
#include "iceManager.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "game.h"
#include "gameManager.h"
#include "tutorialManager.h"
#include "playerTutorial.h"
#include "debugproc.h"
#include "manager.h"
#include "fade.h"
#include "UIplayer.h"
#include "fade2D.h"
#include "texture.h"
#include "gauge.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
//------------------------------
// �X�e�[�W�̒萔
//------------------------------
namespace stage
{
const string PATH_MAP = "data\\TEXT\\ice_stage_tutorial.txt";	// �}�b�v�̃p�X
const int SIZE_MAP = 15;										// �}�b�v�̃T�C�Y
}

//------------------------------
// ��Ԃ̒萔
//------------------------------
namespace state
{
const float TIME_END = 1.0f;	// �I���̗P�\
}

//------------------------------
// �`�F�b�N�}�[�N�̒萔
//------------------------------
namespace check
{
const string PATH_TEX = "data\\TEXTURE\\UI\\CheckMark.png";		// �e�N�X�`���p�X
const float WIDTH = 0.05f;										// ��
const float HEIGHT = 0.086f;									// ����
const D3DXVECTOR3 OFFSET = { WIDTH,-HEIGHT,0.0f };	// �I�t�Z�b�g
}

//------------------------------
// �L���v�V�����̒萔
//------------------------------
namespace caption
{
const float WIDTH = 0.3f;								// ��
const float HEIGHT = 0.1f;								// ����
const D3DXVECTOR3 POS_INIT = { 0.5f,0.114f,0.0f };		// �ʒu
const float TIME_FADE = 2.0f;							// �t�F�[�h�ɂ����鎞��
const string PATH_TEX[CTutorial::E_State::STATE_MAX] =	// �e�N�X�`���p�X
{
	"",												// ���ł��Ȃ����
	"data\\TEXTURE\\UI\\tutorial_move.png",			// �ړ����
	"data\\TEXTURE\\UI\\tutorial_piston.png",		// �˂������
	"data\\TEXTURE\\UI\\tutorial_pistonice.png",	// �X����
	"data\\TEXTURE\\UI\\tutorial_separate.png",		// �j�����
	"data\\TEXTURE\\UI\\tutorial_jump.png",			// �W�����v
	"data\\TEXTURE\\UI\\tutorial_enemy.png",		// �G����
	"",												// �I�����
};
const string PATH_TEX_ENEMY = "data\\TEXTURE\\UI\\tutorial_rival.png";	// �}���`�p�̓G�����p�X
}

//------------------------------
// �Q�[�W�̒萔
//------------------------------
namespace gauge
{
const float TIME_SKIP = 3.0f;				// �X�L�b�v�ɂ����鎞��
const D3DXVECTOR3 POS = { 0.85f,0.9f,0.0f };	// �ʒu
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CTutorial::FuncUpdateState CTutorial::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// ���ł��Ȃ����
	nullptr,					// �ړ����
	nullptr,					// �˂������
	nullptr,					// �X����
	nullptr,					// �j�����
	nullptr,					// �W�����v
	nullptr,					// �G����
	&CTutorial::UpdateEnd,		// �I�����
};

CTutorial *CTutorial::s_pTutorial = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial() : m_state(E_State::STATE_NONE), m_pManager(nullptr), m_fTimeEnd(0.0f) , m_nCntProgress(0), m_pUIPlayer(nullptr), m_abComplete(),
m_pCaption(nullptr), m_pFade2D(nullptr), m_pGaugeSkip(nullptr), m_fTimerSkip(0.0f)
{
	s_pTutorial = this;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorial::~CTutorial()
{
	s_pTutorial = nullptr;
}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorial::Init(void)
{
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �`���[�g���A���p�}�b�v�̐���
	CIceManager* pIceManager = CIceManager::Create(stage::SIZE_MAP, stage::SIZE_MAP);

	if (pIceManager != nullptr)
		pIceManager->Load(&stage::PATH_MAP[0]);
	
	// �J�����̐ݒ�
	Camera::ChangeState(new CFollowPlayer);

	// �C�̐���
	COcean::Create();

	// �`���[�g���A���}�l�[�W���[�̐���
	m_pManager = CTutorialManager::Create();
	
	// ��Ԃ̏�����
	m_state = E_State::STATE_MOVE;

	//--------------------------------
	// �v���C���[�̐���
	//--------------------------------
	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{// �v���C���[�̐���
		CInputManager *pInpuMgr = CInputManager::Create();

		if (!abFrag[i])
			continue;

		CPlayerTutorial *pPlayer = CPlayerTutorial::Create();

		if (pPlayer == nullptr)
			continue;

		pPlayer->BindInputMgr(pInpuMgr);
		pPlayer->SetID(i);
	}

	if (CPlayer::GetNumPlayer() > 1)
	{// �����l�v���C���̂ݍs������
		// �v���C���[UI�̐���
		m_pUIPlayer = CUIPlayer::Create();
	}

	//--------------------------------
	// �L���v�V�����̐���
	//--------------------------------
	m_pCaption = CUI::Create();
	
	if (m_pCaption == nullptr)
		return E_FAIL;

	m_pCaption->SetSize(caption::WIDTH, caption::HEIGHT);
	m_pCaption->SetPosition(caption::POS_INIT);
	m_pCaption->SetVtx();

	int nIdxTexture = Texture::GetIdx(&caption::PATH_TEX[E_State::STATE_MOVE][0]);
	m_pCaption->SetIdxTexture(nIdxTexture);

	m_pFade2D = CFade2D::Create(m_pCaption, caption::TIME_FADE);

	if (m_pFade2D != nullptr)
	{
		m_pFade2D->EnableBouceOut(true);
		m_pFade2D->SetState(CFade2D::E_State::STATE_IN);
	}

	// �Q�[�W�̐���
	m_pGaugeSkip = CGauge::Create(gauge::TIME_SKIP);

	if (m_pGaugeSkip == nullptr)
		return E_FAIL;

	m_pGaugeSkip->SetPosition(gauge::POS);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pManager);
	Object::DeleteObject((CObject**)&m_pUIPlayer);
	Object::DeleteObject((CObject**)&m_pCaption);
	Object::DeleteObject((CObject**)&m_pFade2D);

	// �V�[���̏I��
	CScene::Uninit();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorial::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// ��Ԃ��i�ނ��̊m�F�����鏈��
	CheckProgress();

	// �X�L�b�v�̓���
	InputSkip();

	// �V�[���̍X�V
	CScene::Update();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �I����Ԃ̍X�V
//=====================================================
void CTutorial::UpdateEnd(void)
{
	m_fTimeEnd += CManager::GetDeltaTime();

	if (m_fTimeEnd > state::TIME_END)
	{
		// ��莞�Ԍo�߂Ńt�F�[�h����
		CFade *pFade = CFade::GetInstance();
		if (pFade == nullptr)
			return;

		pFade->SetFade(CScene::MODE::MODE_GAME);
	}
}

//=====================================================
// �i�s����
//=====================================================
void CTutorial::CheckProgress(void)
{
	int nNumPlayer = CPlayer::GetNumPlayer();

	if (m_nCntProgress == nNumPlayer)
	{// �v���C���[�Ɛi�s�J�E���^�[����v������i�s
		ProgressState();
	}
}

//=====================================================
// �i�s�J�E���^�[���Z
//=====================================================
void CTutorial::AddCntProgress(CPlayer *pPlayer)
{
	if (m_pFade2D != nullptr)
	{
		if (m_pFade2D->GetState() != CFade2D::E_State::STATE_NONE)
			return;
	}

	// �Ή�����ID�̃A�C�R�����擾
	int nID = pPlayer->GetID();

	if (m_abComplete[nID])
		return;	// ���Ɋ������Ă�����ʂ�Ȃ�

	// �`�F�b�N�}�[�N�̐���
	CreateCheck(nID);

	m_abComplete[nID] = true;

	// �i�s�J�E���^�[���Z
	m_nCntProgress++;
}

//=====================================================
// �`�F�b�N�}�[�N�̐���
//=====================================================
void CTutorial::CreateCheck(int nIdx)
{
	if (m_pUIPlayer == nullptr)
		return;

	CUI *pIcon = m_pUIPlayer->GetIcon(nIdx);

	if (pIcon == nullptr)
		return;

	// �A�C�R���̈ʒu�擾
	D3DXVECTOR3 posIcon = pIcon->GetPosition();

	D3DXVECTOR3 posCheck = posIcon + check::OFFSET;

	CUI *pCheck = CUI::Create();

	if (pCheck == nullptr)
		return;

	// �`�F�b�N�}�[�N�̏����ݒ�
	pCheck->SetSize(check::WIDTH, check::HEIGHT);
	pCheck->SetPosition(posCheck);
	pCheck->SetVtx();

	int nIdxTexture = Texture::GetIdx(&check::PATH_TEX[0]);
	pCheck->SetIdxTexture(nIdxTexture);

	m_apCheck.push_back(pCheck);
}

//=====================================================
// �X�L�b�v�̓���
//=====================================================
void CTutorial::InputSkip(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetPress(CInputManager::BUTTON_SKIP))
	{
		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimerSkip += fDeltaTime;

		if (m_fTimerSkip >= gauge::TIME_SKIP)
		{// ��莞�Ԓ������Ń^�C�g���ɑJ��
			m_fTimerSkip = gauge::TIME_SKIP;

			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}
	}
	else
	{
		if (m_fTimerSkip < gauge::TIME_SKIP)
		{
			m_fTimerSkip = 0;
		}
	}

	if (m_pGaugeSkip != nullptr)
		m_pGaugeSkip->SetParam(m_fTimerSkip);
}

//=====================================================
// ��Ԃ�i�߂鏈��
//=====================================================
void CTutorial::ProgressState(void)
{
	// ��Ԃ�i�߂�
	m_state = (E_State)(m_state + 1);

	// �J�E���^�[�̃��Z�b�g
	m_nCntProgress = 0;

	// �`�F�b�N�}�[�N�̃��Z�b�g
	for (CUI *pUI : m_apCheck)
		pUI->Uninit();

	m_apCheck.clear();

	if (m_pFade2D != nullptr)
	{
		m_pFade2D->SetState(CFade2D::E_State::STATE_OUT);
		m_pFade2D->SetPathNext(caption::PATH_TEX[m_state]);

		if (m_state == E_State::STATE_EXPLAIN_ENEMY)
		{
			if (CPlayer::GetNumPlayer() > 1)
				m_pFade2D->SetPathNext(caption::PATH_TEX_ENEMY);
		}
	}

	// �����t���O���Z�b�g
	for (int i = 0; i < NUM_PLAYER; i++)
		m_abComplete[i] = false;

	// �`���[�g���A���}�l�[�W���[���ŏ�Ԃ��ς�����Ƃ��̏���
	if (m_pManager != nullptr)
		m_pManager->ChangeState(m_state);
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{
	// �V�[���̕`��
	CScene::Draw();
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CTutorial::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputManager *pInputMgr = CInputManager::GetInstance();

	if (pDebugProc == nullptr || pInputMgr == nullptr)
		return;

	pDebugProc->Print("\n�`���[�g���A�����========================================");
	pDebugProc->Print("\n���[%d]", m_state);
	pDebugProc->Print("\n�J�E���^�[[%d]", m_nCntProgress);
}