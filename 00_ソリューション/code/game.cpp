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

//*****************************************************
// èè`
//*****************************************************
namespace
{
const int TRANS_TIME = 100;	// I¹ÜÅÌ]CÌt[
const int SPEED_TIME = 60;	// ^C}[ª¸ÁÄ¢­¬x
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// Q[bV[hÌpX
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_01.txt";	// TvÌúzu
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// CgÌFªÏíé¬x

const int SIZE_GRID[CGame::E_GameMode::MODE_MAX] = { 0, 10, 15 };	// [h²ÆÌXe[WÌTCY
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CGame::E_State CGame::m_state = STATE_NONE;	// óÔ
CGame *CGame::m_pGame = nullptr;	// ©gÌ|C^

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

	// X}l[W[
	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID[m_GameMode], SIZE_GRID[m_GameMode]);
	pIceManager->Load(PATH_SAMPLE_ICESTAGE);

	// ^C}[¶¬
	m_pTimer = CTimer::Create();

	// Q[}l[W[Ì¶¬
	CGameManager::Create(m_GameMode);

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CGame::Uninit(void)
{
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
	CFade *pFade = CFade::GetInstance();
	CInputManager* pInputManager = CInputManager::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CSound* pSound = CSound::GetInstance();
	COcean::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

	if (!m_bStop)
	{
		// V[ÌXV
		CScene::Update();
	}

	// |[Y========================================
	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
		{
			if (m_pPause == nullptr)
				m_pPause = CPause::Create();
		}
	}

	// JXV
	UpdateCamera();

	// óÔÇ
	ManageState();

	// |[YÌXV
	UpdatePause();

	if (pKeyboard->GetTrigger(DIK_L))
	{// AUV¶¬
		CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);
	}

	if (pKeyboard->GetTrigger(DIK_K))
	{// VN}¶¬
		CEnemy::Create((int)CEnemy::TYPE::TYPE_BEARS);
	}

	if (pKeyboard->GetTrigger(DIK_O))
	{// AzEh¶¬
		CAlbatross::Create(OceanFlow);
	}

#ifdef _DEBUG
	Debug();
#endif
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
// â~óÔÌØèÖ¦
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
		CResultSingle::Create(true);
	}
	else if (pKeyboard->GetTrigger(DIK_9))
	{// sk
		CResultSingle::Create(false);
	}

	// Gð|µ½XRAo·
	if (pKeyboard->GetTrigger(DIK_K))
	{
		//Gð|µ½ÌXRA¶¬
		CDestroyScore::GetInstance()->AddDestroyScore(CEnemy::TYPE_SEALS);

		//Gð|µ½ÌR{UI¶¬
		CUI_Combo::GetInstance()->AddCombo();
	}

	pDebugProc->Print("\nQ[fobOL[======================");
	pDebugProc->Print("\nF:Ôâ~");
	pDebugProc->Print("\n0:");
	pDebugProc->Print("\n9:sk");
	pDebugProc->Print("\nK:G|µ½XRA¶¬");
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