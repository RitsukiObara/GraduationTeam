//*****************************************************
//
// チュートリアルマネージャー[tutorialManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorialManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CTutorialManager::FuncUpdateState CTutorialManager::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,	// 何でもない状態
	&CTutorialManager::UpdateMove,		// 移動状態
	&CTutorialManager::UpdatePeck,		// 突っつき状態
	&CTutorialManager::UpdateBreak,		// 破壊状態
	&CTutorialManager::UpdateExplain,	// 説明状態
	nullptr,	// 終了状態
};

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CTutorialManager *CTutorialManager::Create(void)
{
	CTutorialManager* pTutorialManager = nullptr;

	pTutorialManager = new CTutorialManager;

	if (pTutorialManager != nullptr)
		pTutorialManager->Init();

	return pTutorialManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorialManager::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorialManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorialManager::Update(void)
{
	CTutorial *pTutorial = CTutorial::GetInstance();

	if (pTutorial == nullptr)
		return;

	CTutorial::E_State state = pTutorial->GetState();

	assert(state > -1 && state < CTutorial::E_State::STATE_MAX);
	if (s_aFuncUpdateState[state] != nullptr)
	{ // 更新関数が指定されている場合
		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[state]))();
	}
}

//=====================================================
// 移動状態の更新
//=====================================================
void CTutorialManager::UpdateMove(void)
{

}

//=====================================================
// 突っつき状態の更新
//=====================================================
void CTutorialManager::UpdatePeck(void)
{

}

//=====================================================
// 破壊状態の更新
//=====================================================
void CTutorialManager::UpdateBreak(void)
{

}

//=====================================================
// 説明状態の更新
//=====================================================
void CTutorialManager::UpdateExplain(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CTutorialManager::Draw(void)
{

}

//=====================================================
// 状態の変更
//=====================================================
void CTutorialManager::ChangeState(CTutorial::E_State stateNext)
{
	CTutorial *pTutorial = CTutorial::GetInstance();
	if (pTutorial == nullptr)
		return;


}