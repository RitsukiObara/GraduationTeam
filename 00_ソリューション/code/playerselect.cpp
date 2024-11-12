//*****************************************************
//
// プレイヤー選択処理[playerselect.cpp]
// Author:堀川萩大
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
#include "ocean.h"
#include "gameManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_UI_PLAYERNUMBER = 0.05f;	// プレイヤーナンバーUIのサイズ
const float HEIGHT_UI_PLAYERNUMBER = 0.8f;	// プレイヤーナンバーUIの位置の高さ

const string PATH_UI_STANDBY = "data\\TEXTURE\\UI\\standby.png";	// スタンドバイテクスチャのパス

const D3DXVECTOR3 INIT_MOVE_PLAYER = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// プレイヤーの初期の移動量
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nCountPlayer = 0;
	ZeroMemory(&m_StandbyState[0], sizeof(m_StandbyState));
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));
	ZeroMemory(&m_apInputMgr[0], sizeof(m_apInputMgr));
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	// UIマネージャーの追加
	CUIManager::Create();

	Camera::ChangeState(new CFollowPlayer);

	// テクスチャ番号取得
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	float fRateOnePlayer = 1.0f / MAX_PLAYER;

	// UI生成
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// プレイヤーナンバーUIの生成
		m_apPlayerUI[nCount] = CUI::Create();
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(SIZE_UI_PLAYERNUMBER, SIZE_UI_PLAYERNUMBER);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, HEIGHT_UI_PLAYERNUMBER, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + fRateOnePlayer * nCount, 0.0f), D3DXVECTOR2(fRateOnePlayer + fRateOnePlayer * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		// エントリーUIの生成
		m_apStateUI[nCount] = CUI::Create();
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.07f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}

		// 入力マネージャーの生成
		CInputManager *pInputMgr = CInputManager::Create();
		m_apInputMgr[nCount] = pInputMgr;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerSelect::Uninit(void)
{
	// 各種オブジェクトの破棄
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->Uninit();
			m_apStateUI[nCount] = nullptr;
		}
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->Uninit();
			m_apPlayerUI[nCount] = nullptr;
		}
		if (m_apPlayer[nCount] != nullptr)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = nullptr;
		}
	}

	// オブジェクト全棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	CSound* pSound = CSound::GetInstance();
	
	// シーンの更新
	CScene::Update();

	// プレイヤーのエントリー操作確認
	CheckInputEntry();
	
	// 1人ずつエントリーした場合
	//if (m_StandbyState[0] != STANDBY_OK)
	//{
	//	// プレイヤー全員エントリーした場合
	//	if (m_StandbyState[0] == STANDBY_PLAY && m_StandbyState[1] == STANDBY_PLAY && m_StandbyState[2] == STANDBY_PLAY && m_StandbyState[3] == STANDBY_PLAY)
	//	{
	//		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Junbi_ok.png");

	//		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	//		{
	//			m_StandbyState[nCnt] = STANDBY_OK;
	//			m_apStateUI[nCnt]->SetIdxTexture(nIdx);
	//		}
	//	}
	//}

	//// 遷移できる状態の場合
	////if (m_StandbyState[0] == STANDBY_OK && m_StandbyState[1] == STANDBY_OK && m_StandbyState[2] == STANDBY_OK && m_StandbyState[3] == STANDBY_OK)
	//{
	//	if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) == true)
	//	{
	//		CFade* pFade = CFade::GetInstance();

	//		if (pFade == nullptr)
	//			return;

	//		if (pFade->GetState() != CFade::FADE_NONE)
	//			return;

	//		// モードの保存
	//		gameManager::SaveMode(CGame::E_GameMode::MODE_MULTI, m_nCountPlayer);
	//		
	//		// フェード
	//		pFade->SetFade(CScene::MODE_GAME);
	//	}
	//}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// エントリー入力の確認
//=====================================================
void CPlayerSelect::CheckInputEntry(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_apInputMgr[i] == nullptr)
			continue;

		if (m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
			CreatePlayer(i);	// プレイヤーのエントリー
	}
}

//=====================================================
// プレイヤーの生成
//=====================================================
void CPlayerSelect::CreatePlayer(int nIdx)
{
	if (m_apPlayer[nIdx] != nullptr)
		return;	// 枠が埋まってたら処理を通らない

	// テクスチャ変更
	int nIdxTexture = Texture::GetIdx(&PATH_UI_STANDBY[0]);
	if(m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);

	// エントリー状態を設定
	m_StandbyState[nIdx] = STANDBY_PLAY;
	
	// プレイヤーの生成
	m_apPlayer[nIdx] = CPlayer::Create();
	
	if (m_apPlayer[nIdx] != nullptr)
	{
		// プレイヤー初期設定
		m_apPlayer[nIdx]->SetMove(INIT_MOVE_PLAYER);
		m_apPlayer[nIdx]->SetState(CPlayer::STATE_NORMAL);

		// 入力マネージャーの割り当て
		m_apPlayer[nIdx]->BindInputMgr(m_apInputMgr[nIdx]);	
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;

	pDebugProc->Print("\nエントリー人数[%d人]", m_nCountPlayer);
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerSelect::Draw(void)
{

}