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
#include "selectStageManager.h"
#include "iceManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\icestagemulti00.txt";	// サンプルの初期配置
const int SIZE_GRID = 15;												// ステージのサイズ
const float MAX_SPEED_SUDDONDEATH = 10.0f;								// サドンデスの海流の強さ
const float ACCELE_SUDDONDEATH = 0.036f;								// サドンデスの海流の加速量
const int NUM_PLAYER_SUDDONDEATH = 2;									// サドンデスになるプレイヤーの数
const int TIME_SUDDONDEATH = 60;										// サドンデスになる残り時間]
const float TIME_CHANGE_SUDDONDEATH = 10.0f;							// サドンデスで海流の変わる速さ

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
	//------------------------------------
	// ステージの生成
	//------------------------------------
	// モードの取得
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	// マップ番号の読込
	int nIdxMap = gameManager::LoadIdxMap();

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStageMulti();

	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID, SIZE_GRID);
	if (pIceManager == nullptr)
		return E_FAIL;

	if (nIdxMap != -1)
	{// 海流の初期化
		pIceManager->SetDirStream((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
		pIceManager->SetDirStreamNext((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
	}

	if (apInfoStage.empty())	// ステージ情報が空だったらデフォルトマップ
		pIceManager->Load(PATH_SAMPLE_ICESTAGE);
	else
	{// ランダムでステージを読み込む
		CSelectStageManager::S_InfoStage *pInfoCurrent = universal::RandomFromVector(apInfoStage);

		pIceManager->Load(&pInfoCurrent->pathMap[0]);
	}

	//------------------------------------
	// 各種設定
	//------------------------------------
	// カメラのステイト設定
	Camera::ChangeState(new CMultiGame);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		m_apPlayer.push_back(pPlayer);

		pPlayer->ReLoadModel(&player::PATH_BODY[i][0]);

		pPlayer->ShowNumber();
	}

	// 基底クラスの初期化
	CGameManager::Init();

	// 全プレイヤーの入力割り当て
	CPlayer::BindInputAllPlayer();

	// プレイヤーUIの生成
	CUIPlayer *pUIPlayer = CUIPlayer::Create();

	if (pUIPlayer != nullptr)
		pUIPlayer->StartScatter();

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

	// 海流の強さ管理
	ManageLevelOcean();

	// ゲーム終了の確認
	CheckEndGame();
}

//=====================================================
// 海流の強さの管理
//=====================================================
void CGameManagerMulti::ManageLevelOcean(void)
{
	// 海流を速くする条件
	bool bAccele = false;

	// 加速判定
	bAccele = JudgeAccele();

	if (bAccele)
	{// 海流の強さを設定
		COcean *pOcean = COcean::GetInstance();

		if (pOcean != nullptr)
		{
			pOcean->SetMaxSpeed(MAX_SPEED_SUDDONDEATH);
			pOcean->SetAccele(ACCELE_SUDDONDEATH);
			pOcean->SetBaseTimeChangeRot(TIME_CHANGE_SUDDONDEATH);
		}
	}
}

//=====================================================
// 加速する判定
//=====================================================
bool CGameManagerMulti::JudgeAccele(void)
{
	// プレイヤーが2人になったら
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.size() <= NUM_PLAYER_SUDDONDEATH)
		return true;

	// 残り時間が少なくなったら
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		int nTime = pGame->GetTimeSecond();

		if(nTime <= TIME_SUDDONDEATH)
			return true;
	}

	// 何の条件にも通らなかったら加速しない
	return false;
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