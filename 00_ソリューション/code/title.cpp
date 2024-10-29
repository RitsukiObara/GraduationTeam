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
	m_state = STATE_NONE;
	m_pLogo = nullptr;
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
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	右からくる氷
					// ポリゴンの設定
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.65f;
				}

				//if (nCntUI == TITLE_UI_LOGO)
				//{//	タイトルに戻る
				//	// ポリゴンの設定
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.4f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.2f, MENU_HEIGHT - 0.1f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.25f;
				//}

				//if (nCntUI == TITLE_UI_ICE)
				//{//	ポーズ
				//	// ポリゴンの設定
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(2.5f, 0.12f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT - 0.05f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.5f;
				//}

				//if (nCntUI == TITLE_UI_PENGUIN)
				//{//	ポーズ
				//	// ポリゴンの設定
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(2.5f, 0.12f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT - 0.05f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.5f;
				//}

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
	Object::DeleteObject((CObject**)&m_pLogo);

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

	// 状態管理
	ManageState();

	// 入力
	Input();

	// 色の管理
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (m_apTitle_UI[i] != nullptr)
		{
			D3DXCOLOR colDest = COL_INITIAL_MENU;

			if (i == m_menu)
			{
				colDest = COL_CURRENT_MENU;
			}

			D3DXCOLOR col = m_apTitle_UI[i]->GetCol();

			col += (colDest - col) * 0.3f;

			m_apTitle_UI[i]->SetCol(col);
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

		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}
	else if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
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

	switch (m_menu)
	{
	case CTitle::MENU_GAME:	// ゲーム

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
// 状態管理
//====================================================
void CTitle::ManageState(void)
{
	if (m_state != STATE_OUT)
	{
		Input();
	}

	// 終了フラグ用
	int nEnd = 0;

	// ポリゴンを目標位置に向かわせる
	for (int nCntMenu = 0; nCntMenu < TITLE_UI_MAX; nCntMenu++)
	{
		if (m_apTitle_UI[nCntMenu] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntMenu]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntMenu] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntMenu]->SetPosition(vecDiff);
			m_apTitle_UI[nCntMenu]->SetVtx();

			float fDiff = m_aPosDest[nCntMenu].x - vecDiff.x;

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_state == STATE_OUT)
			{// 終了のライン
				nEnd++;
			}
		}
	}

	if (nEnd == TITLE_UI_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// UIの状態管理
//====================================================
void CTitle::TitleUIState(void)
{
	// ポリゴンを目標位置に向かわせる
	for (int nCntMenu = 0; nCntMenu < TITLE_UI_MAX; nCntMenu++)
	{
		if (m_apTitle_UI[nCntMenu] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntMenu]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntMenu] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntMenu]->SetPosition(vecDiff);
			m_apTitle_UI[nCntMenu]->SetVtx();

			float fDiff = m_aPosDest[nCntMenu].x - vecDiff.x;
		}
	}
}
