//*****************************************************
//
// シングルゲームマネージャー[gameManagerSingle.cpp]
// Author:��山桃也
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
#include "selectStageManager.h"
#include "enemyfactory.h"
#include "UI_combo.h"
#include "game.h"
#include "fade.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_ENEMY_DEFAULT = 5;								// 敵の数のデフォルト値
const string PATH_ENEMY_DEFAULT = "data\\TEXT\\enemy00.txt";	// 敵配置情報のテキスト
}

//=====================================================
// コンストラクタ
//=====================================================
CGameManagerSingle::CGameManagerSingle() : m_pPlayer(nullptr), m_pEnemyFct(nullptr)
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManagerSingle::Init(void)
{
	// 敵数表示UI生成
	CUIEnemy::Create();

	// プレイヤー生成
	m_pPlayer = CPlayer::Create();

	if (m_pPlayer != nullptr)
	{
		// 入力マネージャーの割り当て
		CInputManager *pInpuMgr = CInputManager::Create();
		m_pPlayer->BindInputMgr(pInpuMgr);
	}

	// スコアの生成
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
		pGame->CreateScore();

	// カメラのステイト設定
	Camera::ChangeState(new CFollowPlayer);

	// 基底クラスの初期化
	CGameManager::Init();

	// 敵配置の設定
	int nIdxMap = gameManager::LoadIdxMap();

	m_pEnemyFct = CEnemyFct::Create();

	if (m_pEnemyFct == nullptr)
		return E_FAIL;

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStage();

	if (apInfoStage.empty())
		m_pEnemyFct->Load(PATH_ENEMY_DEFAULT);
	else
		m_pEnemyFct->Load(apInfoStage[nIdxMap]->pathEnemy);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManagerSingle::Uninit(void)
{
	// 基底クラスの終了
	CGameManager::Uninit();

	m_pPlayer = nullptr;
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

	// 敵管理
	ManageEnemy();
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
	CResultSingle::Create(CResultSingle::RESULT_DEAD);
}

//=====================================================
// 敵の管理
//=====================================================
void CGameManagerSingle::ManageEnemy(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	int nNumEnemy = pGame->GetNumEnemyMax();

	if (nNumEnemy == 0)	// 敵全滅で勝利
	{
		// 現在あるコンボのスコアを加算
		CUI_Combo *pCombo = CUI_Combo::GetInstance();
		if (pCombo != nullptr)
			pCombo->AddComboScore();
			pCombo->Uninit();

		// リザルトの生成
		CResultSingle::Create(CResultSingle::RESULT_WIN);
	}
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

//=====================================================
// ゲームの終了
//=====================================================
void CGameManagerSingle::EndGame(void)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
		pFade->SetFade(CScene::MODE_TITLE);
}