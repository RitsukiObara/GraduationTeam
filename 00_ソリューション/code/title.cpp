//*****************************************************
//
// タイトル処理[title.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "title.h"
#include "object.h"
#include "inputManager.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "cameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "polygon2D.h"
#include "UI.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_FACT	(0.01f)	// 移動速度
#define LINE_UNINIT	(0.2f)	// 終了するまでのしきい値

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// メニュー項目の初期色
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// メニュー項目の選択色
	const D3DXVECTOR2 UI_SIZE[CTitle::TITLE_UI_MAX] = 
	{
		D3DXVECTOR2 (0.2f,0.2f),
		D3DXVECTOR2 (0.2f,0.2f),
		D3DXVECTOR2 (0.3f,0.3f),
		D3DXVECTOR2 (0.1f,0.1f),
		D3DXVECTOR2 (0.1f,0.15f),
		D3DXVECTOR2 (0.08f,0.15f),
		D3DXVECTOR2 (0.4f,0.1f),
		D3DXVECTOR2 (0.5f,0.5f),
	};
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	nCntFlash = 0;
	nCntAlpha = 0;
	nCntMove = 0;
	m_State = STATE_NONE;
	m_TitleState = TITLESTATE_ICEFLOW;
	m_Title_UI = TITLE_UI_LEFT;
	m_apMenu_UI = nullptr;

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{
		m_apTitle_UI[nCntUI] = nullptr;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTitle::Init(void)
{
	const char* aPath =
	{// メニューのテクスチャパス
		"data\\TEXTURE\\TITLE\\Start.png",
	};

	const char* aTitle[TITLE_UI_MAX] =
	{// タイトルUIのテクスチャパス
		"data\\TEXTURE\\TITLE\\ice_block_Left_2.png",
		"data\\TEXTURE\\TITLE\\ice_block_Right_2.png",
		"data\\TEXTURE\\TITLE\\ice_block.png",
		"data\\TEXTURE\\TITLE\\title_ice.png",
		"data\\TEXTURE\\TITLE\\penguin.png",
		"data\\TEXTURE\\TITLE\\turuhasi.png",
		"data\\TEXTURE\\TITLE\\title_full.png",
		"data\\TEXTURE\\TITLE\\pengui.png",
	};

	// フォグをかけない
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		// BGMの再生
		pSound->Play(pSound->LABEL_BGM_TITLE);
	}
	else if (pSound == nullptr)
	{
		return E_FAIL;
	}

	// カメラ位置の設定
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 45.38f,84.71f,270.10f };
	pInfoCamera->posR = { -454.28f,331.03f,878.09f };

	// 背景オブジェクトの生成
	CObjectX* pArsenal = CObjectX::Create();

	if (pArsenal != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\other\\arsenal.x");
		pArsenal->BindModel(nIdx);
	}

	int nIdxTexture;

	// ゲームスタートのポリゴンを生成
	if (m_apMenu_UI == nullptr)
	{
		m_apMenu_UI = CUI::Create();

		if (m_apMenu_UI != nullptr)
		{
			{//	スタートメニュー
				// ポリゴンの設定
				m_apMenu_UI->SetPosition(D3DXVECTOR3(0.5f, 0.75f, 0.0f));
				m_apMenu_UI->SetSize(0.2f, 0.15f);
				m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}

			// テクスチャの設定
			nIdxTexture = CTexture::GetInstance()->Regist(aPath);

			m_apMenu_UI->SetIdxTexture(nIdxTexture);
			m_apMenu_UI->SetVtx();
		}
	}

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{// メニュー項目のポリゴンを生成
		if (m_apTitle_UI[nCntUI] == nullptr)
		{
			m_apTitle_UI[nCntUI] = CUI::Create();

			if (m_apTitle_UI[nCntUI] != nullptr)
			{
				if (nCntUI == TITLE_UI_LEFT)
				{//	左からくる氷
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(-0.2f, 0.4f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	右からくる氷
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.36f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.6f;
				}

				if (nCntUI == TITLE_UI_ICEBLOCK)
				{//	氷ブロック合体
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.28f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_ICE)
				{//	氷
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.8f, 0.23f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PENGUIN)
				{//	ペンギン
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.25f, 0.18f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PICKAXE)
				{//	つるはし
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.72f, 0.68f, 0.0f));
					m_apTitle_UI[nCntUI]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -0.5f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_LOGO)
				{//	タイトルロゴ
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.48f, 0.33f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_FLASH)
				{//	フラッシュ
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				// テクスチャの設定
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntUI]);

				m_apTitle_UI[nCntUI]->SetSize(UI_SIZE[nCntUI].x, UI_SIZE[nCntUI].y);
				m_apTitle_UI[nCntUI]->SetIdxTexture(nIdxTexture);
				m_apTitle_UI[nCntUI]->SetVtx();
			}
		}
	}

	CInputManager::Create();
	CScene::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	// メニュー項目の破棄
	if (m_apMenu_UI != nullptr)
	{
		m_apMenu_UI->Uninit();

		m_apMenu_UI = nullptr;
	}

	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
	{// タイトルUIの破棄
		if (m_apTitle_UI[nCnt] != nullptr)
		{
			m_apTitle_UI[nCnt]->Uninit();

			m_apTitle_UI[nCnt] = nullptr;
		}
	}

	// オブジェクト全破棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CTitle::Update(void)
{
	// シーンの更新
	CScene::Update();

	// 入力
	Input();

	// タイトルUIの状態管理
	TitleUIState();

	// 氷が流れてくる状態管理
	IceFlowState();

	// 画面にフラッシュが入る状態管理
	FlashState();

	// ロゴをだす処理
	LogoState();
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{
	CScene::Draw();
}

//=====================================================
// 入力処理
//=====================================================
void CTitle::Input(void)
{
	CInputManager *pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{// フェード処理
		Fade();
	}
}

//=====================================================
// フェード処理
//=====================================================
void CTitle::Fade(void)
{// 各種フェード
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE_NONE)
		return;

	pFade->SetFade(CScene::MODE_GAME);
}

//====================================================
// タイトルUIの状態管理
//====================================================
void CTitle::TitleUIState(void)
{
	// ポリゴンを目標位置に向かわせる
	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{
		if (m_apTitle_UI[nCntUI] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntUI]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntUI] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntUI]->SetPosition(vecDiff);
			m_apTitle_UI[nCntUI]->SetVtx();

			float fDiff = m_aPosDest[nCntUI].x - vecDiff.x;
		}
	}
}

//====================================================
// 氷が流れてくる状態処理
//====================================================
void CTitle::IceFlowState(void)
{
	// 氷が流れてくる状態の時
	if (m_TitleState == TITLESTATE_ICEFLOW)
	{
		D3DXVECTOR3 pos = m_apTitle_UI[TITLE_UI_LEFT]->GetPosition();

		// 目的の位置に現在の位置が近い時
		if (m_aPosDest[TITLE_UI_LEFT].x - 0.01f < pos.x)
		{
			m_apTitle_UI[TITLE_UI_LEFT]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_apTitle_UI[TITLE_UI_RIGHT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			m_TitleState = TITLESTATE_FLASH;
		}
	}
}

//====================================================
// 画面にフラッシュを入れる処理
//====================================================
void CTitle::FlashState(void)
{
	// 画面にフラッシュが入る状態になった時
	if (m_TitleState == TITLESTATE_FLASH ||
		m_TitleState == TITLESTATE_LOGO)
	{
		nCntFlash++;

		m_apTitle_UI[TITLE_UI_FLASH]->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));
		m_apTitle_UI[TITLE_UI_FLASH]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - nCntFlash * 0.005f));

		D3DXCOLOR col = m_apTitle_UI[TITLE_UI_FLASH]->GetCol();

		if (col.a < 0.5f)
		{
			m_TitleState = TITLESTATE_LOGO;
		}
	}
}

//====================================================
// ロゴをだす処理
//====================================================
void CTitle::LogoState(void)
{
	// 画面にフラッシュが入る状態になった時
	if (m_TitleState == TITLESTATE_LOGO)
	{
		nCntAlpha++;
		nCntMove++;

		m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_ICEBLOCK]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_ICE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_PENGUIN]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_PICKAXE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apTitle_UI[TITLE_UI_LOGO]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));

		for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
		{
			if (nCntUI != TITLE_UI_PICKAXE)
			{
				// ポリゴンをカウントごとに動かす
				if (nCntMove > 30 && nCntMove < 100)
				{
					m_aPosDest[nCntUI].y += 0.001f;
				}

				else if (nCntMove > 100 && nCntMove < 240)
				{
					m_aPosDest[nCntUI].y -= 0.001f;
				}

				else if (nCntMove > 240 && nCntMove < 380)
				{
					m_aPosDest[nCntUI].y += 0.001f;
				}

				else if (nCntMove > 380)
				{
					nCntMove = 100;
				}
			}
		}
	}
}
