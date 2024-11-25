//*****************************************************
//
// チュートリアル処理[tutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorial.h"
#include "iceManager.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "game.h"
#include "gameManager.h"
#include "tutorialManager.h"
#include "playerTutorial.h"
#include "debugproc.h"
#include "manager.h"
#include "fade.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
//------------------------------
// ステージの定数
//------------------------------
namespace stage
{
const string PATH_MAP = "data\\TEXT\\ice_stage_tutorial.txt";	// マップのパス
const int SIZE_MAP = 15;										// マップのサイズ
}

//------------------------------
// 状態の定数
//------------------------------
namespace state
{
const float TIME_END = 1.0f;	// 終了の猶予
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CTutorial::FuncUpdateState CTutorial::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何でもない状態
	nullptr,					// 移動状態
	nullptr,					// 突っつき状態
	nullptr,					// 破壊状態
	nullptr,					// 説明状態
	&CTutorial::UpdateEnd,		// 終了状態
};

CTutorial *CTutorial::s_pTutorial = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial() : m_state(E_State::STATE_NONE), m_pManager(nullptr), m_fTimeEnd(0.0f) , m_nCntProgress(0)
{
	s_pTutorial = this;
}

//=====================================================
// デストラクタ
//=====================================================
CTutorial::~CTutorial()
{
	s_pTutorial = nullptr;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorial::Init(void)
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// チュートリアル用マップの生成
	CIceManager* pIceManager = CIceManager::Create(stage::SIZE_MAP, stage::SIZE_MAP);

	if (pIceManager != nullptr)
		pIceManager->Load(&stage::PATH_MAP[0]);
	
	// カメラの設定
	Camera::ChangeState(new CFollowPlayer);

	// 海の生成
	COcean::Create();

	// チュートリアルマネージャーの生成
	m_pManager = CTutorialManager::Create();
	
	// 状態の初期化
	m_state = E_State::STATE_MOVE;

	//--------------------------------
	// プレイヤーの生成
	//--------------------------------
	// モードの取得
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{// プレイヤーの生成
		CInputManager *pInpuMgr = CInputManager::Create();

		if (!abFrag[i])
			continue;

		CPlayerTutorial *pPlayer = CPlayerTutorial::Create();

		if (pPlayer == nullptr)
			continue;

		pPlayer->BindInputMgr(pInpuMgr);
		pPlayer->SetID(i);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pManager);

	// シーンの終了
	CScene::Uninit();

	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorial::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// 状態が進むかの確認をする処理


	// シーンの更新
	CScene::Update();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 終了状態の更新
//=====================================================
void CTutorial::UpdateEnd(void)
{
	m_fTimeEnd += CManager::GetDeltaTime();

	if (m_fTimeEnd > state::TIME_END)
	{
		// 一定時間経過でフェードする
		CFade *pFade = CFade::GetInstance();
		if (pFade == nullptr)
			return;

		pFade->SetFade(CScene::MODE::MODE_GAME);
	}
}

//=====================================================
// 進行判定
//=====================================================
void CTutorial::CheckProgress(void)
{
	int nNumPlayer = CPlayer::GetNumPlayer();

	if (m_nCntProgress == nNumPlayer)
	{// プレイヤーと進行カウンターが一致したら進行
		ProgressState();
	}
}

//=====================================================
// 状態を進める処理
//=====================================================
void CTutorial::ProgressState(void)
{
	// 状態を進める
	m_state = (E_State)(m_state + 1);

	// カウンターのリセット
	m_nCntProgress = 0;

	// チュートリアルマネージャー側で状態が変わったときの処理
	if (m_pManager != nullptr)
		m_pManager->ChangeState(m_state);
}

//=====================================================
// 描画処理
//=====================================================
void CTutorial::Draw(void)
{
	// シーンの描画
	CScene::Draw();
}

//=====================================================
// デバッグ処理
//=====================================================
void CTutorial::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputManager *pInputMgr = CInputManager::GetInstance();

	if (pDebugProc == nullptr || pInputMgr == nullptr)
		return;

	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))	// 状態進める
		ProgressState();

	pDebugProc->Print("\nチュートリアル情報========================================");
	pDebugProc->Print("\n状態[%d]", m_state);
}