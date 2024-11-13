//*****************************************************
//
// シングルゲームマネージャー[gameManagerSingle.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gameManagerSingle.h"
#include "enemy.h"
#include "player.h"
#include "inputManager.h"
#include "resultSingle.h"

//=====================================================
// コンストラクタ
//=====================================================
CGameManagerSingle::CGameManagerSingle()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManagerSingle::Init(void)
{
	// 基底クラスの初期化
	CGameManager::Init();

	// 敵数表示UI生成
	CUIEnemy::Create();

	// 敵生成
	CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);

	// プレイヤー生成
	m_pPlayer = CPlayer::Create();

	if (m_pPlayer != nullptr)
	{
		// 入力マネージャーの割り当て
		CInputManager *pInpuMgr = CInputManager::Create();
		m_pPlayer->BindInputMgr(pInpuMgr);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManagerSingle::Uninit(void)
{
	// 基底クラスの終了
	CGameManager::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CGameManagerSingle::Update(void)
{
	// 基底クラスの更新
	CGameManager::Update();
}

//=====================================================
// 開始状態の更新
//=====================================================
void CGameManagerSingle::UpdateStart(void)
{
	// 基底クラスの更新
	CGameManager::UpdateStart();
}

//=====================================================
// 通常状態の更新
//=====================================================
void CGameManagerSingle::UpdateNormal(void)
{
	// 基底クラスの更新
	CGameManager::UpdateNormal();

	// プレイヤー管理
	ManagePlayer();
}

//=====================================================
// プレイヤーの管理
//=====================================================
void CGameManagerSingle::ManagePlayer(void)
{
	if (m_pPlayer == nullptr)
		return;

	CPlayer::E_State state = m_pPlayer->GetState();

	if (state == CPlayer::E_State::STATE_DEATH)
		DeathPlayer();	// プレイヤーの死亡
}

//=====================================================
// プレイヤーの死亡
//=====================================================
void CGameManagerSingle::DeathPlayer(void)
{
	if (m_pPlayer == nullptr)
		return;

	m_pPlayer->Uninit();
	m_pPlayer = nullptr;

	// プレイヤー死亡で敗北
	CResultSingle::Create(false);
}

//=====================================================
// リザルト状態の更新
//=====================================================
void CGameManagerSingle::UpdateResult(void)
{
	// 基底クラスの更新
	CGameManager::UpdateResult();
}

//=====================================================
// 終了状態の更新
//=====================================================
void CGameManagerSingle::UpdateEnd(void)
{
	// 基底クラスの更新
	CGameManager::UpdateEnd();
}

//=====================================================
// 描画処理
//=====================================================
void CGameManagerSingle::Draw(void)
{
	// 基底クラスの描画
	CGameManager::Draw();
}