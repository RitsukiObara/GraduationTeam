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
#include "UIplayer.h"
#include "fade.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
}

//=====================================================
// コンストラクタ
//=====================================================
CGameManagerMulti::CGameManagerMulti() : m_nNumDeathPlayer(0)
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManagerMulti::Init(void)
{
	// カメラのステイト設定
	Camera::ChangeState(new CMultiGame);

	// 基底クラスの初期化
	CGameManager::Init();

	// モードの取得
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		m_apPlayer.push_back(pPlayer);

		pPlayer->ReLoadModel(&player::PATH_BODY[i][0]);
	}

	// 全プレイヤーの入力割り当て
	CPlayer::BindInputAllPlayer();

	// プレイヤーUIの生成
	CUIPlayer *pUIPlayer = CUIPlayer::Create();

	if (pUIPlayer != nullptr)
		pUIPlayer->StartScatter();

	CPlayer::EnableInputAll(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManagerMulti::Uninit(void)
{
	// 基底クラスの終了
	CGameManager::Uninit();

	m_apPlayer.clear();
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
// 開始状態の更新
//=====================================================
void CGameManagerMulti::UpdateStart(void)
{
	// 基底クラスの更新
	CGameManager::UpdateStart();
}

//=====================================================
// 通常状態の更新
//=====================================================
void CGameManagerMulti::UpdateNormal(void)
{
	// 基底クラスの更新
	CGameManager::UpdateNormal();

	// プレイヤー管理
	ManagePlayer();

	// ゲーム終了の確認
	CheckEndGame();
}

//=====================================================
// プレイヤーの管理
//=====================================================
void CGameManagerMulti::ManagePlayer(void)
{
	for (int i = 0; i < (int)m_apPlayer.size(); i++)
	{
		if (m_apPlayer[i] == nullptr)
			continue;

		if (m_apPlayer[i]->GetState() == CPlayer::E_State::STATE_DEATH)
			DeathPlayer(i);
	}
}

//=====================================================
// プレイヤーの死亡
//=====================================================
void CGameManagerMulti::DeathPlayer(int nIdx)
{
	m_apPlayer[nIdx]->Uninit();
	m_apPlayer[nIdx] = nullptr;

	m_nNumDeathPlayer++;
}

//=====================================================
// ゲーム終了の確認
//=====================================================
void CGameManagerMulti::CheckEndGame(void)
{
	// プレイヤー数が死亡数が下回ったらゲーム終了
	if ((int)m_apPlayer.size() - 1 <= m_nNumDeathPlayer)
	{
		CGame::SetState(CGame::E_State::STATE_END);
	}
}

//=====================================================
// リザルト状態の更新
//=====================================================
void CGameManagerMulti::UpdateResult(void)
{
	// 基底クラスの更新
	CGameManager::UpdateResult();
}

//=====================================================
// 終了状態の更新
//=====================================================
void CGameManagerMulti::UpdateEnd(void)
{
	// 基底クラスの更新
	CGameManager::UpdateEnd();
}

//=====================================================
// 描画処理
//=====================================================
void CGameManagerMulti::Draw(void)
{
	// 基底クラスの描画
	CGameManager::Draw();
}

//=====================================================
// ゲームの終了
//=====================================================
void CGameManagerMulti::EndGame(void)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
		pFade->SetFade(CScene::MODE_RESULTMULTI);

	// 勝者情報取得
	int winner = -1;
	for (int cnt = 0; cnt < (int)m_apPlayer.size(); cnt++)
	{
		if(m_apPlayer[cnt] != nullptr)
		{
			winner = cnt;
			break;
		}
	}
	// 参加人数取得
	int playerNum = m_nNumDeathPlayer + 1;

	// 勝者情報書き出し
	gameManager::SaveWinner(playerNum, winner);
}