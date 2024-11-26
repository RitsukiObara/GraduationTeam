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
// 定数定義
//*****************************************************
namespace
{
const int TRANS_TIME = 100;	// 終了までの余韻のフレーム数
const int SPEED_TIME = 60;	// タイマーが減っていく速度
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// ゲームメッシュロードのパス
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_00.txt";	// サンプルの初期配置
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
	m_nNumEnemyMax = 0;
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

	// モードの取得
	vector<bool> abFrag;
	gameManager::LoadMode(&m_GameMode, abFrag);

	// 氷マネージャーの読込処理
	int nIdxMap = gameManager::LoadIdxMap();

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStage();

	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID[m_GameMode], SIZE_GRID[m_GameMode]);
	if (pIceManager == nullptr)
		return E_FAIL;

	pIceManager->SetDirStream((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
	pIceManager->SetDirStreamNext((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);

	if (apInfoStage.empty())	// ステージ情報が空だったらデフォルトマップ
		pIceManager->Load(PATH_SAMPLE_ICESTAGE);
	else						// 対応したステージを読込む
		pIceManager->Load(&apInfoStage[nIdxMap]->pathMap[0]);

	// タイマー生成
	m_pTimer = CTimer::Create();

	// ゲームマネージャーの生成
	CGameManager::Create(m_GameMode);

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
	COcean::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

	if (!m_bStop)
	{
		// シーンの更新
		CScene::Update();
	}

	// ポーズ========================================
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

	// カメラ更新
	UpdateCamera();

	// 状態管理
	ManageState();

	if (m_state != STATE_RESULT)
	{
		// ポーズの更新
		UpdatePause();
	}


	if (pKeyboard->GetTrigger(DIK_L))
	{// アザラシ生成
		CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);
	}

	if (pKeyboard->GetTrigger(DIK_K))
	{// シロクマ生成
		CEnemy::Create((int)CEnemy::TYPE::TYPE_BEARS);
	}

	if (pKeyboard->GetTrigger(DIK_O))
	{// アホウドリ生成
		CAlbatross::Create(OceanFlow);
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
		return;

	if (pKeyboard->GetTrigger(DIK_F))
	{// 停止状態の切り替え
		ToggleStop();
	}

	if (pKeyboard->GetTrigger(DIK_0))
	{// 勝利
		CResultSingle::Create(true);
	}
	else if (pKeyboard->GetTrigger(DIK_9))
	{// 敗北
		CResultSingle::Create(false);
	}

	pDebugProc->Print("\nゲームデバッグキー======================");
	pDebugProc->Print("\nF:時間停止");
	pDebugProc->Print("\n0:勝利");
	pDebugProc->Print("\n9:敗北");
	pDebugProc->Print("\nK:敵倒したスコア生成");
	pDebugProc->Print("\n7:敵倒す");
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

//=====================================================
// ゲームの開始
//=====================================================
void CGame::StartGame(void)
{
	// ゲームの開始
	m_state = E_State::STATE_NORMAL;

	// 全プレイヤーが操作可能になる
	CPlayer::EnableInputAll(true);
}

//=====================================================
// 敵の追加
//=====================================================
void CGame::AddEnemy(int nType)
{
	// 敵UIの敵数を加算
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