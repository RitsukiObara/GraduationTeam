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
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "debugproc.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "model.h"
#include "texture.h"
#include "field.h"
#include "motion.h"
#include "particle.h"
#include "skybox.h"
#include <stdio.h>

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CRenderer *CManager::m_pRenderer = NULL;	// レンダラーのポインタ
CInputKeyboard *CManager::m_pKeyboard = NULL;	// キーボードのポインタ
CInputMouse *CManager::m_pMouse = NULL;	// マウスのポインタ
CInputJoypad *CManager::m_pJoypad = NULL;	// ジョイパッドのポインタ
CDebugProc *CManager::m_pDebugProc = NULL;	// デバッグプロシージャのポインタ
CCamera *CManager::m_pCamera = NULL;	// カメラのポインタ
CLight *CManager::m_pLight = NULL;	// ライトのポインタ
CTexture *CManager::m_pTexture = NULL;	// テクスチャ管理へのポインタ

//=====================================================
// コンストラクタ
//=====================================================
CManager::CManager()
{

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
	if (m_pRenderer == NULL)
	{// レンダラー生成
		m_pRenderer = CRenderer::Create(hWnd, bWindow);
	}

	if (m_pKeyboard == NULL)
	{// キーボード生成
		m_pKeyboard = CInputKeyboard::Create(hInstance,hWnd);
	}

	if (m_pMouse == NULL)
	{// マウス生成
		m_pMouse = new CInputMouse;

		if (m_pMouse != NULL)
		{
			// マウス初期化
			if (FAILED(m_pMouse->Init(hInstance, hWnd)))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pJoypad == NULL)
	{// ジョイパッド生成
		m_pJoypad = new CInputJoypad;

		if (m_pJoypad != NULL)
		{
			// ジョイパッド初期化
			if (FAILED(m_pJoypad->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pDebugProc == NULL)
	{// デバッグプロシージャ生成
		m_pDebugProc = CDebugProc::Create();
	}

	if (m_pCamera == NULL)
	{// カメラ生成
		m_pCamera = new CCamera;

		if (m_pCamera != NULL)
		{
			// 初期化
			if (FAILED(m_pCamera->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pLight == NULL)
	{// ライト生成
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			// 初期化
			if (FAILED(m_pLight->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pTexture == NULL)
	{// テクスチャ管理生成
		m_pTexture = CTexture::Create();
	}

	// 床の生成
	CField *pField = CField::Create();

    // スカイボックスの生成
    CSkybox::Create();

    // 編集するモーションのパス読み込み
	FILE *pFile;

	char aTemp[MAX_STRING];

	pFile = fopen("data\\Init.txt","r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aTemp[0]);
	}

	// モーションの生成
	CMotion *CMotion = CMotion::Create(&aTemp[0]);


	CParticle::Load();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CManager::Uninit(void)
{
	CParticle::Unload();

	if (m_pRenderer != NULL)
	{// レンダラーの終了・破棄
		m_pRenderer->Uninit();
		m_pRenderer = NULL;
	}

	if (m_pKeyboard != NULL)
	{// キーボードの終了・破棄
		m_pKeyboard->Uninit();
		m_pKeyboard = NULL;
	}

	if (m_pMouse != NULL)
	{// マウスの終了・破棄
		m_pMouse->Uninit();

		delete m_pMouse;
		m_pMouse = NULL;
	}

	if (m_pJoypad != NULL)
	{// ジョイパッドの終了・破棄
		m_pJoypad->Uninit();

		delete m_pJoypad;
		m_pJoypad = NULL;
	}

	if (m_pDebugProc != NULL)
	{// デバッグプロシージャの終了・破棄
		m_pDebugProc->Uninit();
		m_pDebugProc = NULL;
	}

	if (m_pLight != NULL)
	{// ライトの終了・破棄
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pCamera != NULL)
	{// カメラの終了・破棄
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pTexture != NULL)
	{// テクスチャの終了・破棄
		m_pTexture->Unload();

		m_pTexture = NULL;
	}

	// モデル破棄
	CModel::Unload();
}

//=====================================================
// 更新処理
//=====================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{
		// レンダラーの更新
		m_pRenderer->Update();
	}

	if (m_pKeyboard != NULL)
	{
		// キーボードの更新
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{
		// マウスの更新
		m_pMouse->Update();
	}

	if (m_pJoypad != NULL)
	{
		// ジョイパッドの更新
		m_pJoypad->Update();
	}

	if (m_pDebugProc != NULL)
	{
		// デバッグプロシージャの更新
		m_pDebugProc->Update();
	}

	if (m_pCamera != NULL)
	{
		// カメラの更新
		m_pCamera->Update();
	}

	if (m_pLight != NULL)
	{
		// ライトの更新
		m_pLight->Update();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		// レンダラーの描画
		m_pRenderer->Draw();
	}
}

namespace Manager
{
D3DXVECTOR3 GetPosVCamera(void)
{
    D3DXVECTOR3 posV = {};

    CCamera *pCamera = CManager::GetCamera();

    if (pCamera == nullptr)
        return posV;

    posV = pCamera->GetCamera()->posV;

    return posV;
}
}
