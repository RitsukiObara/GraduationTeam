//*****************************************************
//
// ゲーム処理[game.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
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
#include "result.h"
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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int TRANS_TIME = 100;	// 終了までの余韻のフレーム数
const int SPEED_TIME = 60;	// タイマーが減っていく速度
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// ゲームメッシュロードのパス
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_01.txt";	// サンプルの初期配置
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ライトの色が変わる速度

const int SIZE_GRID[CGame::E_GameMode::MODE_MAX] = { 0, 10, 15 };	// モードごとのステージのサイズ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGame::E_State CGame::m_state = STATE_NONE;	// 状態
CGame *CGame::m_pGame = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
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
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	m_pGame = this;

	m_state = STATE_NORMAL;
	m_bStop = false;

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
		pRenderer->SetStart(50000);
		pRenderer->SetEnd(70000);
		pRenderer->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	// モードの取得
	m_GameMode = E_GameMode::MODE_SINGLE;

	// 氷マネージャー
	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID[m_GameMode], SIZE_GRID[m_GameMode]);
	pIceManager->Load(PATH_SAMPLE_ICESTAGE);

	// ゲームマネージャーの生成
	CGameManager::Create(m_GameMode);
	
	// タイマー生成
	m_pTimer = CTimer::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGame::Uninit(void)
{
	// オブジェクト全棄
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager* pInputManager = CInputManager::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (!m_bStop)
	{
		// シーンの更新
		CScene::Update();
	}

	// ポーズ========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
	{
		if(m_pPause == nullptr)
			m_pPause = CPause::Create();
	}

	// カメラ更新
	UpdateCamera();

	// 状態管理
	ManageState();

	// ポーズの更新
	UpdatePause();

	if (pKeyboard->GetTrigger(DIK_L))
	{// アザラシ生成
		CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// カメラの更新
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
// 状態管理
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();
	m_nTimerCnt++;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		// タイマーが減っていく速度
		if (m_nTimerCnt >= SPEED_TIME)
		{
			m_nTimerCnt = 0;

			if(m_pTimer != nullptr)
				m_pTimer->AddSecond(-1);	// タイマーの更新
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
// 停止状態の切り替え
//=====================================================
void CGame::ToggleStop(void)
{
	m_bStop = m_bStop ? false : true;

	if (m_bStop)
	{
		Camera::ChangeState(new CMoveControl);
	}
	else
	{
		Camera::ChangeState(new CFollowPlayer);
	}
}

//=====================================================
// ポーズの更新
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
// デバッグ処理
//=====================================================
void CGame::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{// 停止状態の切り替え
		ToggleStop();
	}

	if (pKeyboard->GetTrigger(DIK_0))
	{// 勝利
		CResult::Create(true);
	}
	if (pKeyboard->GetTrigger(DIK_9))
	{// 敗北
		CResult::Create(false);
	}

	if (pKeyboard->GetTrigger(DIK_I))
	{// アイス仮生成
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
			pIceManager->CreateIce(2,-1);
	}

	// 敵を倒したスコア出す
	if (pKeyboard->GetTrigger(DIK_K))
	{
		//敵を倒した時のスコア生成
		CDestroyScore::GetInstance()->AddDestroyScore(CEnemy::TYPE_SEALS);

		//敵を倒した時のコンボUI生成
		CUI_Combo::GetInstance()->AddCombo();
	}
}

//=====================================================
// 描画処理
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
// スコアの生成
//=====================================================
void CGame::CreateScore(void)
{
	if (m_pScore == nullptr)
		m_pScore = CScore::Create();
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