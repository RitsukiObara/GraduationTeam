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
#include "skybox.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "slow.h"
#include "meshfield.h"
#include "CameraState.h"
#include "particle.h"
#include "timer.h"
#include "meshCube.h"
#include "blur.h"
#include "light.h"
#include "result.h"
#include "title.h"
#include "ice.h"
#include "iceManager.h"
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "seals.h"
#include "ocean.h"
#include "stageResultUI.h"
#include "UI_enemy.h"
#include "ocean_flow_UI.h"
#include "flowIce.h"
#include "BG_Ice.h"
#include "flowIceFactory.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(100)	// 終了までの余韻のフレーム数
namespace
{
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// ゲームメッシュロードのパス
const int NUM_LIGHT = 3;	// ライトの数
const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ライトのデフォルト色
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ライトの色が変わる速度

const int NUM_GRID_V = 10;	// 縦グリッドの数
const int NUM_GRID_H = 10;	// 横グリッドの数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// 状態
CGame *CGame::m_pGame = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_posMid = { 0.0f,0.0f,0.0f };
	m_pPause = nullptr;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	m_pGame = this;

	m_colLight = COL_LIGHT_DEFAULT;
	m_state = STATE_NORMAL;
	m_bStop = false;

	// UIマネージャーの追加
	CUIManager::Create();

	// スカイボックスの生成
	CSkybox::Create();

	 //海の追加
	COcean::Create();

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
		pRenderer->SetStart(50000);
		pRenderer->SetEnd(70000);
		pRenderer->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	// スロー管理の生成
	CSlow::Create();
	
	// ライトの生成
	CreateLight();

	Camera::ChangeState(new CFollowPlayer);
	
	// タイマー表示の生成
	m_pTimer = CTimer::Create();
	m_pTimer->SetPosition(D3DXVECTOR3(0.48f, 0.07f, 0.0f));
	m_pTimer->SetSecond(MAX_TIME);

	// スコア表示の生成
	m_pScore = CScore::Create();
	m_pScore->SetPosition(D3DXVECTOR3(0.09f, 0.07f, 0.0f));

	// ステージリザルト表示の生成
	//m_pStageResultUI = CStageResultUI::Create();
	//m_pStageResultUI->SetPosition(D3DXVECTOR3(0.4f, 0.07f, 0.0f));

	// 氷マネージャー
	CIceManager::Create(NUM_GRID_V, NUM_GRID_H);

	// 矢印モデルの生成
	m_pOceanFlowUI = COceanFlowUI::Create();
	m_pOceanFlowUI->SetPosition(D3DXVECTOR3(0.09f, 0.07f, 0.0f));

	// 敵数表示UI生成
	CUIEnemy::Create();

	// 敵生成
	CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);

	// プレイヤー生成
	CPlayer::Create();

	// 流氷生成
	CFlowIce::Create();

	// 背景氷生成
	CBgIce::Create(D3DXVECTOR3(0.0f,200.0f,0.0f),CBgIce::TYPE_SMALL);

	// 流氷ファクトリーの生成
	CFlowIceFct::Create();

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
	CInputManager *pInputManager = CInputManager::GetInstance();
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

	// ライトの更新
	UpdateLight();

	// ポーズの更新
	UpdatePause();

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

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		// タイマーの更新
		m_pTimer->AddSecond(-CManager::GetDeltaTime());
		m_pTimer->Update();

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
// ライトの生成
//=====================================================
void CGame::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight *pLight = CLight::Create();

		if (pLight == nullptr)
			continue;

		D3DLIGHT9 infoLight = pLight->GetLightInfo();

		infoLight.Type = D3DLIGHT_DIRECTIONAL;
		infoLight.Diffuse = m_colLight;

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
		infoLight.Direction = vecDir;
		
		pLight->SetLightInfo(infoLight);

		m_aLight.push_back(pLight);
	}
}

//=====================================================
// ライトの更新
//=====================================================
void CGame::UpdateLight(void)
{
	for (auto it : m_aLight)
	{
		D3DLIGHT9 infoLight = it->GetLightInfo();

		D3DXCOLOR col = infoLight.Diffuse;

		col += (m_colLight - col) * SPEED_CHANGE_LIGHTCOL;

		infoLight.Diffuse = col;

		it->SetLightInfo(infoLight);
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
// ライトの色リセット
//=====================================================
void CGame::ResetDestColLight(void)
{
	m_colLight = COL_LIGHT_DEFAULT;
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

	pDebugProc->Print("\n中心座標[%f,%f,%f]", m_posMid.x, m_posMid.y, m_posMid.z);
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

	char *apString[STATE::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"END",
	};
}