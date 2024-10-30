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
#define MENU_WIDTH	(0.2f)	// 項目の幅
#define MENU_HEIGHT	(0.2f)	// 項目の高さ
#define MOVE_FACT	(0.01f)	// 移動速度
#define LINE_UNINIT	(0.2f)	// 終了するまでのしきい値

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// メニュー項目の初期色
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// メニュー項目の選択色
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_State = STATE_NONE;
	m_TitleState = TITLESTATE_ICEFLOW;

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{
		m_apMenu_UI[nCntMenu] = nullptr;
	}

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
	const char* aPath[MENU_MAX] =
	{// メニューのテクスチャパス
		"data\\TEXTURE\\UI\\menu00.png",
		"data\\TEXTURE\\UI\\menu01.png",
	};

	const char* aTitle[TITLE_UI_MAX] =
	{// タイトルUIのテクスチャパス
		"data\\TEXTURE\\TITLE\\ice_block_Left_2.png",
		"data\\TEXTURE\\TITLE\\ice_block_Right_2.png",
		"data\\TEXTURE\\TITLE\\ice_block.png",
		"data\\TEXTURE\\TITLE\\title_full.png",
		"data\\TEXTURE\\TITLE\\title_ice.png",
		"data\\TEXTURE\\TITLE\\penguin.png",
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

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// メニュー項目のポリゴンを生成

		if (m_apMenu_UI[nCntMenu] == nullptr)
		{
			m_apMenu_UI[nCntMenu] = CUI::Create();

			if (m_apMenu_UI[nCntMenu] != nullptr)
			{
				if (nCntMenu == MENU_START)
				{//	スタートメニュー
					// ポリゴンの設定
					m_apMenu_UI[nCntMenu]->SetPosition(D3DXVECTOR3(-0.2f, 0.4f, 0.0f));
					m_apMenu_UI[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu_UI[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}

				if (nCntMenu == MENU_OPTION)
				{//	オプションメニュー
					// ポリゴンの設定
					m_apMenu_UI[nCntMenu]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apMenu_UI[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu_UI[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}

				// テクスチャの設定
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntMenu]);

				m_apMenu_UI[nCntMenu]->SetIdxTexture(nIdxTexture);
				m_apMenu_UI[nCntMenu]->SetVtx();
			}
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
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	右からくる氷
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.65f;
				}

				if (nCntUI == TITLE_UI_ICEBLOCK)
				{//	氷ブロック合体
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, -0.25f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT + 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}


				if (nCntUI == TITLE_UI_LOGO)
				{//	タイトルに戻る
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, -0.3f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.2f, MENU_HEIGHT - 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_ICE)
				{//	ポーズ
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.8f, -0.2f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH - 0.1f, MENU_HEIGHT - 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PENGUIN)
				{//	ポーズ
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.25f, -0.15f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH - 0.1f, MENU_HEIGHT - 0.05f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				// テクスチャの設定
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntUI]);

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
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apMenu_UI[nCnt] != nullptr)
		{
			m_apMenu_UI[nCnt]->Uninit();

			m_apMenu_UI[nCnt] = nullptr;
		}
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

	// 状態管理
	ManageState();

	// タイトルUIの状態管理
	TitleUIState();

	// 色の管理
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (m_apMenu_UI[i] != nullptr)
		{
			D3DXCOLOR colDest = COL_INITIAL_MENU;

			if (i == m_Menu)
			{
				colDest = COL_CURRENT_MENU;
			}

			D3DXCOLOR col = m_apMenu_UI[i]->GetCol();

			col += (colDest - col) * 0.3f;

			m_apMenu_UI[i]->SetCol(col);
		}
	}
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

	// 項目選択
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_Menu = (MENU)((m_Menu + MENU_MAX - 1) % MENU_MAX);
	}
	else if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_Menu = (MENU)((m_Menu + 1) % MENU_MAX);
	}

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

	switch (m_Menu)
	{
	case CTitle::MENU_START:	// スタート

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CTitle::MENU_OPTION:	// 設定

		pFade->SetFade(CScene::MODE_TUTORIAL);

		break;
	default:
		break;
	}
}

//====================================================
// メニューUI状態管理
//====================================================
void CTitle::ManageState(void)
{
	if (m_State != STATE_OUT)
	{
		Input();
	}

	// 終了フラグ用
	int nEnd = 0;

	// ポリゴンを目標位置に向かわせる
	for (int nCntUI = 0; nCntUI < MENU_MAX; nCntUI++)
	{
		if (m_apMenu_UI[nCntUI] != nullptr)
		{
			D3DXVECTOR3 pos = m_apMenu_UI[nCntUI]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntUI] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apMenu_UI[nCntUI]->SetPosition(vecDiff);
			m_apMenu_UI[nCntUI]->SetVtx();

			float fDiff = m_aPosDest[nCntUI].x - vecDiff.x;

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_State == STATE_OUT)
			{// 終了のライン
				nEnd++;
			}
		}
	}

	if (nEnd == TITLE_UI_MAX &&
		m_State == STATE_OUT)
	{
		Uninit();
	}
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
// 氷が流れてくる状態
//====================================================
void CTitle::IceFlowState(void)
{

}
