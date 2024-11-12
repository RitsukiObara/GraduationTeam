//*****************************************************
//
// マルチゲームマネージャー[gameManagerMulti.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gameManagerMulti.h"
#include "inputManager.h"
#include "player.h"

//=====================================================
// コンストラクタ
//=====================================================
CGameManagerMulti::CGameManagerMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManagerMulti::Init(void)
{
	// 基底クラスの初期化
	CGameManager::Init();

	// モードの取得
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		CInputManager *pInpuMgr = CInputManager::Create();

		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		pPlayer->BindInputMgr(pInpuMgr);
		m_apPlayer.push_back(pPlayer);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManagerMulti::Uninit(void)
{
	// 基底クラスの終了
	CGameManager::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CGameManagerMulti::Update(void)
{
	// 基底クラスの更新
	CGameManager::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CGameManagerMulti::Draw(void)
{
	// 基底クラスの描画
	CGameManager::Draw();
}