//*****************************************************
//
// シーン処理[scene.cpp]
// Author:髙山桃也
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
	// マップモデルの生成
	CObjectX* pMapX = CObjectX::Create();

	if (pMapX)
	{
		int nIdx = CModel::Load("data\\MODEL\\block\\map01.x");
		pMapX->BindModel(nIdx);
	}

	//	土俵モデルの生成
	CObjectX* pDohyou = CObjectX::Create();

	if (pDohyou)
	{
		int nIdx = CModel::Load("data\\MODEL\\dohyou\\dohyou01.x");
		pDohyou->BindModel(nIdx);
		pDohyou->SetRotation({0.0f, 1.57f, 0.0f});
	}

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

	// 地面ポリゴンの生成
	CPolygon3D* pPoly = CPolygon3D::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.57f });

	if (pPoly)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pPoly->SetIdxTexture(nIdx);
		pPoly->SetSize(20000.0f, 20000.0f);
		pPoly->SetVtx();
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