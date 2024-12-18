//*****************************************************
//
// マネージャー処理[manager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"
#include "sound.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "polygon3D.h"
#include "objectX.h"
#include "model.h"
#include "texture.h"
#include "particle.h"
#include "fade.h"
#include "fade_fallice.h"
#include "inputManager.h"
#include "debrisSpawner.h"
#include "cameraState.h"
#include "MyEffekseer.h"
#include "selectStageManager.h"
#include "option.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CCamera *CManager::m_pCamera = nullptr;						// カメラのポインタ
CMyEffekseer* CManager::m_pMyEffekseer = nullptr;			// エフェクシアのポインタ
CScene *CManager::m_pScene = nullptr;						// シーンへのポインタ
CScene::MODE CManager::m_mode = CScene::MODE_TITLE;			// 現在のモード
float CManager::m_fDeltaTime = 0.0f;						// 前回のフレームから経過した時間
CManager *CManager::m_pManager = nullptr;					// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CManager::CManager()
{
	m_pScene = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CManager::~CManager()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_pManager = this;

	// レンダラーの生成
	CRenderer::Create(hWnd, bWindow);

	// 入力デバイス初期化
	CInputManager::InitDevice(hInstance, hWnd);

	// デバッグ表示の生成
	CDebugProc::Create();

	// サウンド生成
	CSound::Create(hWnd);

	if (m_pCamera == nullptr)
	{// カメラ生成
		m_pCamera = new CCamera;

		if (m_pCamera != nullptr)
		{
			// 初期化
			if (FAILED(m_pCamera->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	// テクスチャ管理の生成
	CTexture::Create();

	// フェードの生成
	CFade::Create();
	CFade_FallIce::Create();

	// パーティクルの読込
	CParticle::Load();

	// ステージ情報の読込
	CSelectStageManager::Load();

	SetMode(m_mode);

	// エフェクシアの生成
	CMyEffekseer::Create();

	// オプションの情報読み込み
	COption::LoadSetting();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CManager::Uninit(void)
{
	m_pManager = nullptr;

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	// フェード終了
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Uninit();
	}
	CFade_FallIce* pFadeFallIce = CFade_FallIce::GetInstance();

	if (pFadeFallIce != nullptr)
	{
		pFadeFallIce->Uninit();
	}

	// パーティクル情報破棄
	CParticle::Unload();

	// 破片スポナー破棄
	CDebrisSpawner::Unload();

	// レンダラー終了
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Uninit();
	}

	// 入力デバイス初期化
	CInputManager::UninitDevice();

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// サウンドの終了・破棄
		pSound->Uninit();
		pSound = nullptr;
	}

	// デバッグプロシージャの終了
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Uninit();
	}

	if (m_pCamera != nullptr)
	{// カメラの終了・破棄
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;

		if (m_pCameraStateLast != nullptr)
		{
			delete m_pCameraStateLast;
			m_pCameraStateLast = nullptr;
		}
	}

	// テクスチャの終了・破棄
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		pTexture->Unload();

		delete pTexture;
		pTexture = nullptr;
	}

	// エフェクシア破棄
	if (CMyEffekseer::GetInstance() != nullptr)
		CMyEffekseer::GetInstance()->Uninit();

	// モデル破棄
	CModel::Unload();
}

//=====================================================
// 更新処理
//=====================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(500, 60), ImGuiCond_Appearing);

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Appearing);

	ImGui::Begin("Edit");
#endif

	// フェード更新
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Update();
	}
	CFade_FallIce* pFadeFallIce = CFade_FallIce::GetInstance();

	if (pFadeFallIce != nullptr)
	{
		pFadeFallIce->Update();
	}

	CInputManager::UpdateAll();

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// サウンドの更新
		pSound->Update();
	}

	// デバッグプロシージャの更新
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Update();
	}

	if (CInputKeyboard::GetInstance() != nullptr)
	{
		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_F3))
		{// 強制リセット
			CManager::SetMode(CScene::MODE_TITLE);
		}
	}

#ifdef _DEBUG
	ImGui::End();
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CManager::Draw(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}

	// レンダラー描画
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Draw();
	}
}

//=====================================================
// モード設定
//=====================================================
void CManager::SetMode(CScene::MODE mode)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// 音の停止
		pSound->Stop();
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}

	// シーンを破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	// シーン生成
	m_pScene = CScene::Create(mode);

	// モード設定
	m_mode = mode;
}

//=====================================================
// デバッグカメラ切り替え
//=====================================================
void CManager::ToggleDebugCamera(void)
{
	m_bDebugCamera = m_bDebugCamera ? false : true;

	if (m_bDebugCamera)
	{
		m_pCameraStateLast = Camera::GetState();
		Camera::ChangeState(new CMoveControl);
	}
	else
	{
		Camera::ChangeState(m_pCameraStateLast);
	}
}