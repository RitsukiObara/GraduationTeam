//*****************************************************
//
// Q[[game.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
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
#include "fishshadow.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const int TRANS_TIME = 100;	// I¹ÜÅÌ]CÌt[
const int SPEED_TIME = 60;	// ^C}[ª¸ÁÄ¢­¬x
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// Q[bV[hÌpX
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_00.txt";	// TvÌúzu
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// CgÌFªÏíé¬x
const int TIME_LIMIT_SE_PLAY_TIME = 10;		// ^C~bgSE¬µnßéÔ
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CGame::E_State CGame::m_state = STATE_NONE;	// óÔ
CGame *CGame::m_pGame = nullptr;			// ©gÌ|C^
CResultSingle::E_Result CGame::s_result = CResultSingle::E_Result::RESULT_WIN;	// Ug

//=====================================================
// RXgN^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_nTimerCnt = 0;
	m_bStop = false;
	m_pPause = nullptr;
	m_GameMode = E_GameMode::MODE_NONE;
	m_nNumEnemyMax = 0;
	m_pGameManager = nullptr;
	m_bPlayTimeLimitSE = false;
}

//=====================================================
// ú»
//=====================================================
HRESULT CGame::Init(void)
{
	// eNXÌú»
	CScene::Init();

	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// [hÌæ¾
	vector<bool> abFrag;
	gameManager::LoadMode(&m_GameMode, abFrag);

	// ^C}[¶¬
	m_pTimer = CTimer::Create();

	// Q[}l[W[Ì¶¬
	m_pGameManager = CGameManager::Create(m_GameMode);

	// Q[BGMÌÄ¶
	Sound::Play(CSound::LABEL_BGM_GAME01);

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CGame::Uninit(void)
{
	m_pGameManager = nullptr;

	// IuWFNgSü
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// XV
//=====================================================
void CGame::Update(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	COcean::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

	if (!m_bStop)
	{
		// V[ÌXV
		CScene::Update();
	}

	// |[YÌüÍ
	InputPause();

	// JXV
	UpdateCamera();

	// óÔÇ
	ManageState();

	if (m_state != STATE_RESULT)
	{
		// |[YÌXV
		UpdatePause();
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// |[YÌüÍ
//=====================================================
void CGame::InputPause(void)
{
	// UgóÔÌêA±ÌÖð²¯é
	if (m_state == STATE_RESULT)
		return;

	// |[YæÊª NULL ¶áÈ¢êA±ÌÖð²¯é
	if (m_pPause != nullptr)
		return;

	int nNumPlayer = CPlayer::GetNumPlayer();						// vC[Ì

	for (auto player : CPlayer::GetInstance())
	{
		CInputManager* pInputManager = player->GetInpuManager();	// Cvbg}l[W[
		CSound* pSound = CSound::GetInstance();						// TEhÌîñ

		if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
		{
			m_pPause = CPause::Create(pInputManager);
			pSound->Stop(CSound::LABEL_SE_TIMELIMIT);
			m_bPlayTimeLimitSE = false;

			// ±ÌÖð²¯é
			return;
		}
	}
}

//=====================================================
// JÌXV
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
// óÔÇ
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();
	CSound* pSound = CSound::GetInstance();

	// |[YÌ^C}[ðÁZ·é
	if (m_pPause == nullptr)
		m_nTimerCnt++;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		// ^C}[ª¸ÁÄ¢­¬x
		if (m_nTimerCnt >= SPEED_TIME)
		{
			m_nTimerCnt = 0;

			if(m_pTimer != nullptr)
				m_pTimer->AddSecond(-1);	// ^C}[ÌXV
		}

		// ^C~bgmF
		CheckLimit();

		break;
	case CGame::STATE_RESULT:
		
		if (pSound != nullptr)
		{
			pSound->Stop(CSound::LABEL_SE_TIMELIMIT);
		}

		break;
	case CGame::STATE_END:

		m_nCntState++;
		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			m_pGameManager->EndGame();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// â~óÔÌØèÖ¦
//=====================================================
void CGame::ToggleStop(void)
{
	m_bStop = m_bStop ? false : true;

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
// |[YÌXV
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
// ^C~bgmF
//=====================================================
void CGame::CheckLimit(void)
{
	if (m_bPlayTimeLimitSE == false && m_pTimer != nullptr && m_pTimer->GetSecond() <= TIME_LIMIT_SE_PLAY_TIME)
	{
		CSound* pSound = CSound::GetInstance();
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_TIMELIMIT);
			m_bPlayTimeLimitSE = true;
		}
	}
}

//=====================================================
// fobO
//=====================================================
void CGame::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_F))
	{// â~óÔÌØèÖ¦
		ToggleStop();
	}

	if (pKeyboard->GetTrigger(DIK_0))
	{// 
		CResultSingle::Create(CResultSingle::RESULT_WIN);
	}
	else if (pKeyboard->GetTrigger(DIK_9))
	{// sk
		CResultSingle::Create(CResultSingle::RESULT_DEAD);
	}

	pDebugProc->Print("\nQ[fobOL[======================");
	pDebugProc->Print("\nF:Ôâ~");
	pDebugProc->Print("\n0:");
	pDebugProc->Print("\n9:sk");
	pDebugProc->Print("\nK:G|µ½XRA¶¬");
	pDebugProc->Print("\n7:G|·");
}

//=====================================================
// `æ
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
// |[YÌðú
//=====================================================
void CGame::ReleasePause(void)
{
	m_pPause = nullptr;
	CheckLimit();	// ^C~bgmF
}

//=====================================================
// XRAÌ¶¬
//=====================================================
void CGame::CreateScore(void)
{
	if (m_pScore == nullptr)
		m_pScore = CScore::Create();
}

//=====================================================
// Q[ÌJn
//=====================================================
void CGame::StartGame(void)
{
	// Q[ÌJn
	m_state = E_State::STATE_NORMAL;

	// SvC[ªìÂ\ÉÈé
	CPlayer::EnableInputAll(true);
}

//=====================================================
// GÌÇÁ
//=====================================================
void CGame::AddEnemy(int nType)
{
	// GUIÌGðÁZ
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