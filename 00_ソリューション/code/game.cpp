//*****************************************************
//
// �Q�[������[game.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "game.h"
#include "gameManager.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "debugproc.h"
#include "UIManager.h"
#include "polygon3D.h"
#include "texture.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "meshfield.h"
#include "CameraState.h"
#include "particle.h"
#include "timer.h"
#include "blur.h"
#include "resultSingle.h"
#include "ice.h"
#include "iceManager.h"
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "seals.h"
#include "stageResultUI.h"
#include "UI_enemy.h"
#include "destroy_score.h"
#include "UI_combo.h"
#include "albatross.h"
#include "selectStageManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int TRANS_TIME = 100;	// �I���܂ł̗]�C�̃t���[����
const int SPEED_TIME = 60;	// �^�C�}�[�������Ă������x
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// �Q�[�����b�V�����[�h�̃p�X
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_00.txt";	// �T���v���̏����z�u
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ���C�g�̐F���ς�鑬�x

const int SIZE_GRID[CGame::E_GameMode::MODE_MAX] = { 0, 10, 15 };	// ���[�h���Ƃ̃X�e�[�W�̃T�C�Y
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CGame::E_State CGame::m_state = STATE_NONE;	// ���
CGame *CGame::m_pGame = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_nTimerCnt = 0;
	m_bStop = false;
	m_pPause = nullptr;
	m_GameMode = E_GameMode::MODE_NONE;
	m_nNumEnemyMax = 0;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGame::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// ���[�h�̎擾
	vector<bool> abFrag;
	gameManager::LoadMode(&m_GameMode, abFrag);

	// �X�}�l�[�W���[�̓Ǎ�����
	int nIdxMap = gameManager::LoadIdxMap();

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStage();

	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID[m_GameMode], SIZE_GRID[m_GameMode]);
	if (pIceManager == nullptr)
		return E_FAIL;

	pIceManager->SetDirStream((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
	pIceManager->SetDirStreamNext((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);

	if (apInfoStage.empty())	// �X�e�[�W��񂪋󂾂�����f�t�H���g�}�b�v
		pIceManager->Load(PATH_SAMPLE_ICESTAGE);
	else						// �Ή������X�e�[�W��Ǎ���
		pIceManager->Load(&apInfoStage[nIdxMap]->pathMap[0]);

	// �^�C�}�[����
	m_pTimer = CTimer::Create();

	// �Q�[���}�l�[�W���[�̐���
	CGameManager::Create(m_GameMode);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager* pInputManager = CInputManager::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CSound* pSound = CSound::GetInstance();
	COcean::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

	if (!m_bStop)
	{
		// �V�[���̍X�V
		CScene::Update();
	}

	// �|�[�Y========================================
	if (m_state != STATE_RESULT && pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
	{
		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create();
		}
		else
		{
			m_pPause->SetState(m_pPause->STATE_OUT);
		}
	}

	// �J�����X�V
	UpdateCamera();

	// ��ԊǗ�
	ManageState();

	if (m_state != STATE_RESULT)
	{
		// �|�[�Y�̍X�V
		UpdatePause();
	}


	if (pKeyboard->GetTrigger(DIK_L))
	{// �A�U���V����
		CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);
	}

	if (pKeyboard->GetTrigger(DIK_K))
	{// �V���N�}����
		CEnemy::Create((int)CEnemy::TYPE::TYPE_BEARS);
	}

	if (pKeyboard->GetTrigger(DIK_O))
	{// �A�z�E�h������
		CAlbatross::Create(OceanFlow);
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
//=====================================================
void CGame::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();
	m_nTimerCnt++;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		// �^�C�}�[�������Ă������x
		if (m_nTimerCnt >= SPEED_TIME)
		{
			m_nTimerCnt = 0;

			if(m_pTimer != nullptr)
				m_pTimer->AddSecond(-1);	// �^�C�}�[�̍X�V
		}

		break;
	case CGame::STATE_RESULT:

		break;
	case CGame::STATE_END:

		m_nCntState++;
		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// ��~��Ԃ̐؂�ւ�
//=====================================================
void CGame::ToggleStop(void)
{
	//m_bStop = m_bStop ? false : true;

	if (true)
	{
		Camera::ChangeState(new CMoveControl);
	}
	else
	{
		Camera::ChangeState(new CFollowPlayer);
	}
}

//=====================================================
// �|�[�Y�̍X�V
//=====================================================
void CGame::UpdatePause(void)
{
	if (m_pPause == nullptr)
	{
		return;
	}

	m_pPause->Update();
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CGame::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_F))
	{// ��~��Ԃ̐؂�ւ�
		ToggleStop();
	}

	if (pKeyboard->GetTrigger(DIK_0))
	{// ����
		CResultSingle::Create(true);
	}
	else if (pKeyboard->GetTrigger(DIK_9))
	{// �s�k
		CResultSingle::Create(false);
	}

	pDebugProc->Print("\n�Q�[���f�o�b�O�L�[======================");
	pDebugProc->Print("\nF:���Ԓ�~");
	pDebugProc->Print("\n0:����");
	pDebugProc->Print("\n9:�s�k");
	pDebugProc->Print("\nK:�G�|�����X�R�A����");
	pDebugProc->Print("\n7:�G�|��");
}

//=====================================================
// �`�揈��
//=====================================================
void CGame::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char *apString[E_State::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"END",
	};
}

//=====================================================
// �X�R�A�̐���
//=====================================================
void CGame::CreateScore(void)
{
	if (m_pScore == nullptr)
		m_pScore = CScore::Create();
}

//=====================================================
// �Q�[���̊J�n
//=====================================================
void CGame::StartGame(void)
{
	// �Q�[���̊J�n
	m_state = E_State::STATE_NORMAL;

	// �S�v���C���[������\�ɂȂ�
	CPlayer::EnableInputAll(true);
}

//=====================================================
// �G�̒ǉ�
//=====================================================
void CGame::AddEnemy(int nType)
{
	// �GUI�̓G�������Z
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->AddEnemy(nType);

	m_nNumEnemyMax++;
}

namespace game
{
void AddScore(int nScore)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	CScore *pScore = pGame->GetScore();

	if (pScore == nullptr)
		return;

	pScore->AddScore(nScore);
}
}