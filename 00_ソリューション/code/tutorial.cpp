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
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CTutorial::FuncUpdateState CTutorial::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// ���ł��Ȃ����
	nullptr,					// �ړ����
	nullptr,					// �˂������
	nullptr,					// �j����
	nullptr,					// �������
	&CTutorial::UpdateEnd,		// �I�����
};

CTutorial *CTutorial::s_pTutorial = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial() : m_state(E_State::STATE_NONE), m_pManager(nullptr), m_fTimeEnd(0.0f) , m_nCntProgress(0)
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

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pManager);

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
// ��Ԃ�i�߂鏈��
//=====================================================
void CTutorial::ProgressState(void)
{
	// ��Ԃ�i�߂�
	m_state = (E_State)(m_state + 1);

	// �J�E���^�[�̃��Z�b�g
	m_nCntProgress = 0;

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

	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))	// ��Ԑi�߂�
		ProgressState();

	pDebugProc->Print("\n�`���[�g���A�����========================================");
	pDebugProc->Print("\n���[%d]", m_state);
}