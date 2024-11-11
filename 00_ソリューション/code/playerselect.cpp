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
#include "playerselect.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "inputjoypad.h"
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
#include "meshCube.h"
#include "blur.h"
#include "light.h"
#include "ice.h"
#include "iceManager.h"
#include "player.h"
#include "seals.h"
#include "flowIce.h"
#include "BG_Ice.h"
#include "flowIceFactory.h"
#include "UI.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(100)	// 終了までの余韻のフレーム数
#define SPEED_TIME	(60)	// タイマーが減っていく速度

namespace
{
	const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// ゲームメッシュロードのパス
	const int NUM_LIGHT = 3;	// ライトの数
	const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ライトのデフォルト色
	const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ライトの色が変わる速度

	const int SIZE_GRID[CPlayerSelect::E_GameMode::MODE_MAX] = { 0, 10, 15 };	// モードごとのステージのサイズ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayerSelect::E_State CPlayerSelect::m_state = STATE_NONE;	// 状態
CPlayerSelect* CPlayerSelect::m_pGame = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nCountPlayer = -1;
	m_posMid = { 0.0f,0.0f,0.0f };
	m_GameMode = E_GameMode::MODE_NONE;
	m_StandbyState[0] = STANDBY_WATE;
	m_StandbyState[1] = STANDBY_WATE;
	m_StandbyState[2] = STANDBY_WATE;
	m_StandbyState[3] = STANDBY_WATE;
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	m_pGame = this;

	m_colLight = COL_LIGHT_DEFAULT;
	m_state = STATE_NORMAL;

	// UIマネージャーの追加
	CUIManager::Create();

	// スカイボックスの生成
	CSkybox::Create();

	//背景氷のロード
	//CBgIce::Load("data\\TEXT\\BG_Ice.txt");

	// フォグをかける
	CRenderer* pRenderer = CRenderer::GetInstance();

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

	// モードの取得
	m_GameMode = E_GameMode::MODE_SINGLE;

	// 氷マネージャー
	CIceManager::Create(SIZE_GRID[m_GameMode], SIZE_GRID[m_GameMode]);

	// 流氷生成
	CFlowIce::Create();

	// 流氷ファクトリーの生成
	CFlowIceFct::Create();

	// テクスチャ番号取得
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	// UI生成
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		m_apPlayerUI[nCount] = CUI::Create();

		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(0.05f,0.05f);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + 0.25f * nCount, 0.8f, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + 0.25f * nCount, 0.0f), D3DXVECTOR2(0.25f + 0.25f * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		m_apStateUI[nCount] = CUI::Create();

		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.05f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + 0.25f * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerSelect::Uninit(void)
{
	// オブジェクト全棄
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	CInputManager* pInputManager = CInputManager::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();


	// シーンの更新
	CScene::Update();

	
	if(m_StandbyState[0] != STANDBY_OK)
	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) == true)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\standby.png");

		// プレイヤー生成
		CPlayer::Create();
		m_nCountPlayer++;
		m_apStateUI[m_nCountPlayer]->SetIdxTexture(nIdx);
		m_StandbyState[m_nCountPlayer] = STANDBY_PLAY;
	}
	if (m_StandbyState[0] == STANDBY_PLAY && m_StandbyState[1] == STANDBY_PLAY && m_StandbyState[2] == STANDBY_PLAY && m_StandbyState[3] == STANDBY_PLAY)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Junbi_ok.png");

		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			m_StandbyState[nCnt] = STANDBY_OK;
			m_apStateUI[nCnt]->SetIdxTexture(nIdx);

		}
	}

	// カメラ更新
	UpdateCamera();

	// ライトの更新
	UpdateLight();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// カメラの更新
//=====================================================
void CPlayerSelect::UpdateCamera(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// ライトの生成
//=====================================================
void CPlayerSelect::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight* pLight = CLight::Create();

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
void CPlayerSelect::UpdateLight(void)
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
// ライトの色リセット
//=====================================================
void CPlayerSelect::ResetDestColLight(void)
{
	m_colLight = COL_LIGHT_DEFAULT;
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_I))
	{// アイス仮生成
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
			pIceManager->CreateIce(2, -1);
	}

	pDebugProc->Print("\n中心座標[%f,%f,%f]", m_posMid.x, m_posMid.y, m_posMid.z);
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerSelect::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char* apString[E_State::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"END",
	};
}