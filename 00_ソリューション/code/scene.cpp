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
#include "selectmode.h"
#include "playerselect.h"
#include "selectStage.h"
#include "tutorial.h"
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"
#include "sound.h"
#include "inputManager.h"
#include "slow.h"

#include "texture.h"
#include "objectX.h"
#include "meshcylinder.h"
#include "polygon3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_LIGHT = 3;	// ライトの数
const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ライトのデフォルト色
}

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

	// スロー管理の生成
	CSlow::Create();

	// ライトの生成
	CreateLight();

	// スカイボックスの生成
	CSkybox::Create();

	return S_OK;
}

//=====================================================
// ライトの生成
//=====================================================
void CScene::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight *pLight = CLight::Create();

		if (pLight == nullptr)
			continue;

		D3DLIGHT9 infoLight = pLight->GetLightInfo();

		infoLight.Type = D3DLIGHT_DIRECTIONAL;
		infoLight.Diffuse = COL_LIGHT_DEFAULT;

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
		infoLight.Direction = vecDir;

		pLight->SetLightInfo(infoLight);
	}
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
	case CScene::MODE_SELECTMODE:

		pScene = new CSelectMode;

		break;
	case CScene::MODE_SELECTPLAYER:

		pScene = new CPlayerSelect;

		break;
	case CScene::MODE_SELECTSTAGE:

		pScene = new CSelectStage;

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