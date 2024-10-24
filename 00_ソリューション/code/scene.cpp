//*****************************************************
//
// シーン処理[scene.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"
#include "game.h"
#include "logo.h"
#include "title.h"
#include "tutorial.h"
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "inputManager.h"

#include "texture.h"
#include "objectX.h"
#include "meshcylinder.h"
#include "polygon3D.h"

//=====================================================
// コンストラクタ
//=====================================================
CScene::CScene()
{
	m_nTimerTrans = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CScene::~CScene()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScene::Init(void)
{
	// 山シリンダーの生成
	CMeshCylinder* pCylinder = CMeshCylinder::Create();

	if (pCylinder)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\mountain000.png");
		pCylinder->SetIdxTexture(nIdx);
		pCylinder->SetRadius(6000.0f);
		pCylinder->SetHeight(5000.0f);
		pCylinder->SetVtx();
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CScene::Uninit(void)
{
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
		pSound->Stop();

	CLight::ReleaseAll();

	CInputManager::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CScene::Update(void)
{
	// レンダラーの取得
	CRenderer *pRenderer = CRenderer::GetInstance();
	if (pRenderer != nullptr)
	{
		// レンダラーの更新
		pRenderer->Update();
	}

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();
	if (pCamera != nullptr)
	{
		// カメラの更新
		pCamera->Update();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CScene::Draw(void)
{

}

//=====================================================
// シーンの生成処理
//=====================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	switch (mode)
	{
	case CScene::MODE_LOGO:

		pScene = new CLogo;

		break;
	case CScene::MODE_TITLE:

		pScene = new CTitle;

		break;
	case CScene::MODE_TUTORIAL:

		pScene = new CTutorial;

		break;
	case CScene::MODE_GAME:

		pScene = new CGame;

		break;
	case CScene::MODE_RANKING:

		pScene = new CRanking;

		break;
	default:

		pScene = new CGame;

		break;
	}

	if (pScene != nullptr)
	{// シーン生成
		pScene->Init();
	}

	return pScene;
}