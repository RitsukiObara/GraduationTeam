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
#include "fade_fallice.h"
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
#include "effect2D.h"
#include "inputjoypad.h"
#include "iceManager.h"
#include "ocean.h"
#include "npcpenguin.h"
#include "npcpenguinstate_title.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_FACT	(0.01f)	// 移動速度
#define LINE_UNINIT	(0.2f)	// 終了するまでのしきい値
#define EFFECTMAX	(15)	// つるはしで壊したときのエフェクトの最大数

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const string PATH_TEX = "data\\TEXTURE\\EFFECT\\snow01.png";	// テクスチャパス
	const string MAP_PATH = "data\\TEXT\\ice_stage_title.txt";		// 配置ステージパス
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// メニュー項目の初期色
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// メニュー項目の選択色
	const D3DXVECTOR3 UI_POS[CTitle::TITLE_UI_MAX] =  // UIの初期位置
	{
		D3DXVECTOR3(-0.2f, 0.4f,0.0f),
		D3DXVECTOR3(1.2f, 0.36f,0.0f),
		D3DXVECTOR3(0.5f, 0.28f,0.0f),
		D3DXVECTOR3(0.8f, 0.23f,0.0f),
		D3DXVECTOR3(0.25f, 0.18f,0.0f),
		D3DXVECTOR3(0.48f, 0.33f,0.0f),
		D3DXVECTOR3(0.72f, 0.68f,0.0f),
		D3DXVECTOR3(0.5f, 0.5f,0.0f),
	};
	const D3DXVECTOR2 UI_SIZE[CTitle::TITLE_UI_MAX] =  // UIの初期サイズ
	{
		D3DXVECTOR2(0.2f,0.2f),
		D3DXVECTOR2(0.2f,0.2f),
		D3DXVECTOR2(0.3f,0.3f),
		D3DXVECTOR2(0.1f,0.1f),
		D3DXVECTOR2(0.1f,0.15f),
		D3DXVECTOR2(0.4f,0.1f),
		D3DXVECTOR2(0.08f,0.15f),
		D3DXVECTOR2(0.5f,0.5f),
	};
	const vector<D3DXVECTOR3> PENGUIN_POS =
	{
		D3DXVECTOR3(0.0f,0.0f,-350.0f),
		D3DXVECTOR3(-220.0f,0.0f,150.0f),
		D3DXVECTOR3(300.0f,0.0f,0.0f),
		D3DXVECTOR3(600.0f,0.0f,700.0f),
	};
	const vector<D3DXVECTOR3> PENGUIN_ROT =
	{
		D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DXVECTOR3(0.0f,D3DX_PI,0.0f),
		D3DXVECTOR3(0.0f,-0.8f * D3DX_PI,0.0f),
		D3DXVECTOR3(0.0f,0.2f * D3DX_PI,0.0f),
	};

	const float VIBRATION_POWER = 0.2f;	//バイブの強さ
	const int VIBRATION_TIME = 20;	//バイブの時間
}

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_nCntState = 0;
	m_nCntMove = 0;
	m_nStateLogo = 0;
	m_State = STATE_NONE;
	m_TitleState = TITLESTATE_ICEFLOW;
	m_Title_UI = TITLE_UI_LEFT;
	m_apMenu_UI = nullptr;
	m_bFade = false;
	m_bMove = false;

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
		"data\\TEXTURE\\TITLE\\title_full.png",
		"data\\TEXTURE\\TITLE\\turuhasi.png",
		"data\\TEXTURE\\TITLE\\pengui.png",
	};

	// フォグをかけない
	CRenderer* pRenderer = CRenderer::GetInstance();

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
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 45.38f,84.71f,270.10f };
	pInfoCamera->posR = { 0.0f,100.0f,0.0f };
	pInfoCamera->fLength = 1500.0f;

	//海の生成
	COcean::Create();

	// マップ配置
	CIceManager* pIceManager = CIceManager::Create(15, 15);
	if (pIceManager == nullptr)
		return E_FAIL;

	pIceManager->SetDirStream(COcean::E_Stream::STREAM_UP);
	pIceManager->SetDirStreamNext(COcean::E_Stream::STREAM_UP);
	pIceManager->Load(&MAP_PATH[0]);

	// ペンギン配置
	CNPCPenguin* pPenguin = nullptr;
	for (int cnt = 0; cnt < (int)PENGUIN_POS.size(); cnt++)
	{
		pPenguin = CNPCPenguin::Create(new CNPCPenguinState_StandTitle);

		if (pPenguin != nullptr)
		{
			pPenguin->SetPosition(PENGUIN_POS[cnt]);
			pPenguin->SetRotation(PENGUIN_ROT[cnt]);
		}
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

	Camera::ChangeState(new CCameraStateTitle);

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{// メニュー項目のポリゴンを生成
		if (m_apTitle_UI[nCntUI] == nullptr)
		{
			m_apTitle_UI[nCntUI] = CUI::Create();

			if (m_apTitle_UI[nCntUI] != nullptr)
			{
				m_apTitle_UI[nCntUI]->SetPosition(UI_POS[nCntUI]);

				if (nCntUI == TITLE_UI_LEFT)
				{//	左からくる氷
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	右からくる氷
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.6f;
				}

				if (nCntUI == TITLE_UI_ICEBLOCK)
				{//	氷ブロック合体
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_ICE)
				{//	氷
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PENGUIN)
				{//	ペンギン
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PICKAXE)
				{//	つるはし
					m_apTitle_UI[nCntUI]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_LOGO)
				{//	タイトルロゴ
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_FLASH)
				{//	フラッシュ
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
	switch (m_TitleState)
	{
	case CTitle::TITLESTATE_ICEFLOW:

		// 氷が流れてくる状態管理
		IceFlowState();

		break;
	case CTitle::TITLESTATE_LOGO:

		// ロゴを出して動かす処理
		LogoState();

		// 入力
		Input();

		break;
	case CTitle::TITLESTATE_PICKAXE:

		// ロゴを出して動かす処理
		LogoState();

		// ピッケルを動かす処理
		PickaxeState();

		break;
	default:
		break;
	}

	// シーンの更新
	CScene::Update();

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{
		m_apTitle_UI[nCntUI]->SetVtx();
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
	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	if (pInputJoypad == nullptr)
		return;

	if (m_bFade == true)
	{
		if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))	// ENTER押したとき
		{
			// joypad振動させる
			pInputJoypad->Vibration(0, VIBRATION_POWER, VIBRATION_TIME);

			// 決定音を鳴らす
			CSound::GetInstance()->Play(CSound::LABEL_SE_DECISION);

			m_nCntState = 0;
			m_nStateLogo = 0;
			m_TitleState = TITLESTATE_PICKAXE;

			for (int nCntUI = 0; nCntUI < EFFECTMAX; nCntUI++)
			{
				float fmoveX = universal::RandRange(40, -40) * 0.1f;
				float fmoveY = universal::RandRange(40, -40) * 0.1f;
				CEffect2D* pEffect2D = CEffect2D::Create(D3DXVECTOR3(640.0f, 550.0f, 0.0f), 60.0f, 80, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(fmoveX, fmoveY, 0.0f));
				int nIdx = Texture::GetIdx(&PATH_TEX[0]);
				pEffect2D->SetIdxTexture(nIdx);
			}
		}
	}
}

//=====================================================
// フェード処理
//=====================================================
void CTitle::Fade(void)
{// 各種フェード
	CFade_FallIce* pFade = CFade_FallIce::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE_NONE)
		return;

	pFade->SetFade(CScene::MODE_SELECTMODE);
}

//====================================================
// 氷が流れてくる状態処理
//====================================================
void CTitle::IceFlowState(void)
{
	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	// 氷が流れてくる状態の時
	D3DXVECTOR3 pos_left = m_apTitle_UI[TITLE_UI_LEFT]->GetPosition();
	D3DXVECTOR3 pos_right = m_apTitle_UI[TITLE_UI_RIGHT]->GetPosition();

	pos_left.x += 0.003f;
	pos_right.x -= 0.003f;

	// 目的の位置に現在の位置が近い時
	if (m_aPosDest[TITLE_UI_LEFT].x + 0.01f < pos_left.x)
	{
		// 氷をくっつける処理
		IceConnect(&pos_left, &pos_right);
	}
	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{
		// 決定音を鳴らす
		CSound::GetInstance()->Play(CSound::LABEL_SE_DECISION);

		// 氷をくっつける処理
		IceConnect(&pos_left, &pos_right);
	}

	m_apTitle_UI[TITLE_UI_LEFT]->SetPosition(pos_left);
	m_apTitle_UI[TITLE_UI_RIGHT]->SetPosition(pos_right);
}

//====================================================
// 氷をくっつける処理
//====================================================
void CTitle::IceConnect(D3DXVECTOR3* left, D3DXVECTOR3* right)
{
	*left = m_aPosDest[TITLE_UI_LEFT];	// 現在の位置に目標の位置を入れる
	*right = m_aPosDest[TITLE_UI_RIGHT];

	m_apTitle_UI[TITLE_UI_LEFT]->SetAlpha(0.0f);	// 透明度調整
	m_apTitle_UI[TITLE_UI_RIGHT]->SetAlpha(0.0f);
	m_apTitle_UI[TITLE_UI_FLASH]->SetAlpha(1.0f);
	m_TitleState = TITLESTATE_LOGO;
}

//====================================================
// ロゴを出して動かす処理
//====================================================
void CTitle::LogoState(void)
{
	// 画面にフラッシュが入る状態になった時
	m_nCntMove++;

	float fAlpha = m_apTitle_UI[TITLE_UI_FLASH]->GetAlpha();
	fAlpha -= 0.004f;
	m_bFade = true;

	for (int nCntUI = TITLE_UI_ICEBLOCK; nCntUI < TITLE_UI_PICKAXE; nCntUI++)
	{
		D3DXVECTOR3 pos = m_apTitle_UI[nCntUI]->GetPosition();

		// ポリゴンを動かす
		if (m_nCntMove > 0 && m_nCntMove < 40)
		{
			m_bMove = false;
		}

		if (m_nCntMove > 40 && m_nCntMove < 80)
		{
			m_bMove = true;
		}

		if (m_nCntMove > 79)
		{
			m_nCntMove = 0;
		}

		if (m_bMove == false)
		{
			pos.y += 0.0005f;
		}

		if (m_bMove == true)
		{
			pos.y -= 0.0005f;
		}

		m_apTitle_UI[nCntUI]->SetPosition(pos);
	}

	// フラッシュの位置と透明度調整
	m_apTitle_UI[TITLE_UI_FLASH]->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));
	m_apTitle_UI[TITLE_UI_FLASH]->SetAlpha(fAlpha);

	// UIの透明度調整
	m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apTitle_UI[TITLE_UI_ICEBLOCK]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apTitle_UI[TITLE_UI_ICE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apTitle_UI[TITLE_UI_PENGUIN]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apTitle_UI[TITLE_UI_PICKAXE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apTitle_UI[TITLE_UI_LOGO]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//====================================================
// ピッケル動かす処理
//====================================================
void CTitle::PickaxeState(void)
{
	D3DXVECTOR3 rot = m_apTitle_UI[TITLE_UI_PICKAXE]->GetRotation();

	m_apTitle_UI[TITLE_UI_FLASH]->SetAlpha(0.0f);	// 透明度を0にする

	rot.z += 0.07f;	// つるはしの向きを傾ける

	m_nStateLogo++;

	if (rot.z > 0.5f)
	{
		rot.z = 0.5f;
	}

	m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	// スタートロゴの透明度を0にする

	m_apTitle_UI[TITLE_UI_PICKAXE]->SetRotation(rot);

	if (m_bFade == true)
	{
		m_nCntState++;

		if (m_nCntState > 70)
		{
			// フェード処理
			Fade();
		}
	}
}
