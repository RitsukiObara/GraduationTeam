//*****************************************************
//
// ゲームマネージャー[gameManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gameManager.h"
#include "UIManager.h"
#include "skybox.h"
#include "ocean.h"
#include "BG_Ice.h"
#include "camera.h"
#include "cameraState.h"
#include "iceManager.h"
#include "UI_ready.h"
#include "ocean_flow_UI.h"
#include "flowIceFactory.h"
#include "gameManagerSingle.h"
#include "gameManagerMulti.h"

//=====================================================
// コンストラクタ
//=====================================================
CGameManager::CGameManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CGameManager *CGameManager::Create(CGame::E_GameMode mode)
{
	CGameManager* pGameManager = nullptr;

	switch (mode)
	{
	case CGame::MODE_SINGLE:
		pGameManager = new CGameManagerSingle;
		break;
	case CGame::MODE_MULTI:
		pGameManager = new CGameManagerMulti;
		break;
	default:
		assert(false);
		break;
	}

	if (pGameManager == nullptr)
		return nullptr;

	pGameManager->Init();

	return pGameManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManager::Init(void)
{
	// UIマネージャーの追加
	CUIManager::Create();

	// スカイボックスの生成
	CSkybox::Create();

	//海の生成
	COcean::Create();

#if 0
	//背景氷のロード
	CBgIce::Load("data\\TEXT\\BG_Ice.txt");

	//ゲームスタート告知UI
	CUIready::Create();
#endif

	// カメラのステイト設定
	Camera::ChangeState(new CFollowPlayer);

	// 矢印モデルの生成
	COceanFlowUI::Create();

	// 流氷ファクトリーの生成
	CFlowIceFct::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManager::Uninit(void)
{

}

//=====================================================
// 更新処理
//=====================================================
void CGameManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CGameManager::Draw(void)
{

}