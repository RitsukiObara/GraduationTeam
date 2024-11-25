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
#include "UIplayer.h"
#include "texture.h"

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

//------------------------------
// チェックマークの定数
//------------------------------
namespace check
{
const string PATH_TEX = "data\\TEXTURE\\UI\\CheckMark.png";		// テクスチャパス
const float WIDTH = 0.05f;										// 幅
const float HEIGHT = 0.086f;									// 高さ
const D3DXVECTOR3 OFFSET = { WIDTH,-HEIGHT,0.0f };	// オフセット
}

//------------------------------
// キャプションの定数
//------------------------------
namespace caption
{
const float WIDTH = 0.3f;							// 幅
const float HEIGHT = 0.1f;							// 高さ
const D3DXVECTOR3 POS_INIT = { 0.5f,0.114f,0.0f };	// 位置
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
	nullptr,					// 破壊説明
	nullptr,					// 敵説明
	&CTutorial::UpdateEnd,		// 終了状態
};

CTutorial *CTutorial::s_pTutorial = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial() : m_state(E_State::STATE_NONE), m_pManager(nullptr), m_fTimeEnd(0.0f) , m_nCntProgress(0), m_pUIPlayer(nullptr), m_abComplete(),
m_pCaption(nullptr)
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

	// プレイヤーUIの生成
	m_pUIPlayer = CUIPlayer::Create();

	//--------------------------------
	// キャプションの生成
	//--------------------------------
	m_pCaption = CUI::Create();
	
	if (m_pCaption == nullptr)
		return E_FAIL;

	m_pCaption->SetSize(caption::WIDTH, caption::HEIGHT);
	m_pCaption->SetPosition(caption::POS_INIT);
	m_pCaption->SetVtx();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pManager);
	Object::DeleteObject((CObject**)&m_pUIPlayer);

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
	CheckProgress();

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
// 進行カウンター加算
//=====================================================
void CTutorial::AddCntProgress(CPlayer *pPlayer)
{
	// 対応したIDのアイコンを取得
	int nID = pPlayer->GetID();

	if (m_abComplete[nID])
		return;	// 既に完了していたら通らない

	// チェックマークの生成
	CreateCheck(nID);

	m_abComplete[nID] = true;

	// 進行カウンター加算
	m_nCntProgress++;
}

//=====================================================
// チェックマークの生成
//=====================================================
void CTutorial::CreateCheck(int nIdx)
{
	if (m_pUIPlayer == nullptr)
		return;

	CUI *pIcon = m_pUIPlayer->GetIcon(nIdx);

	if (pIcon == nullptr)
		return;

	// アイコンの位置取得
	D3DXVECTOR3 posIcon = pIcon->GetPosition();

	D3DXVECTOR3 posCheck = posIcon + check::OFFSET;

	CUI *pCheck = CUI::Create();

	if (pCheck == nullptr)
		return;

	// チェックマークの初期設定
	pCheck->SetSize(check::WIDTH, check::HEIGHT);
	pCheck->SetPosition(posCheck);
	pCheck->SetVtx();

	int nIdxTexture = Texture::GetIdx(&check::PATH_TEX[0]);
	pCheck->SetIdxTexture(nIdxTexture);

	m_apCheck.push_back(pCheck);
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

	// チェックマークのリセット
	for (CUI *pUI : m_apCheck)
		pUI->Uninit();

	m_apCheck.clear();

	// 完了フラグリセット
	for (int i = 0; i < NUM_PLAYER; i++)
		m_abComplete[i] = false;

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

	pDebugProc->Print("\nチュートリアル情報========================================");
	pDebugProc->Print("\n状態[%d]", m_state);
	pDebugProc->Print("\nカウンター[%d]", m_nCntProgress);
}