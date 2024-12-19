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
#include "fade2D.h"
#include "texture.h"
#include "gauge.h"
#include "flowIceFactory.h"
#include "sound.h"

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
const float WIDTH = 0.3f;								// 幅
const float HEIGHT = 0.1f;								// 高さ
const D3DXVECTOR3 POS_INIT = { 0.5f,0.114f,0.0f };		// 位置
const float TIME_FADE = 2.0f;							// フェードにかかる時間
const string PATH_TEX[CTutorial::E_State::STATE_MAX] =	// テクスチャパス
{
	"",												// 何でもない状態
	"data\\TEXTURE\\UI\\tutorial_move.png",			// 移動状態
	"data\\TEXTURE\\UI\\tutorial_piston.png",		// 突っつき状態
	"data\\TEXTURE\\UI\\tutorial_pistonice.png",	// 氷説明
	"data\\TEXTURE\\UI\\tutorial_separate.png",		// 破壊説明
	"data\\TEXTURE\\UI\\tutorial_jump.png",			// ジャンプ
	"data\\TEXTURE\\UI\\tutorial_enemy.png",		// 敵説明
	"data\\TEXTURE\\UI\\tutorial_albatross.png",	// アホウドリ
	"",												// 終了状態
};
const string PATH_TEX_ENEMY = "data\\TEXTURE\\UI\\tutorial_rival.png";	// マルチ用の敵説明パス
const D3DXVECTOR2 SIZE[CTutorial::E_State::STATE_MAX] =	// サイズ
{
	{ 0.0f,0.0f },	// 何でもない状態
	{ 0.2f,0.1f },	// 移動状態
	{ 0.2f,0.1f },	// 突っつき状態
	{ 0.2f,0.1f },	// 氷説明
	{ 0.2f,0.1f },	// 破壊説明
	{ 0.15f,0.1f },	// ジャンプ
	{ 0.17f,0.1f },	// 敵説明
	{ 0.15f,0.1f },	// アホウドリ
	{ 0.0f,0.0f },	// 終了状態
};
}

//------------------------------
// ゲージの定数
//------------------------------
namespace gauge
{
const float TIME_SKIP = 1.0f;				// スキップにかかる時間
const D3DXVECTOR3 POS = { 0.85f,0.9f,0.0f };	// 位置
}

//------------------------------
// ヒントの定数
//------------------------------
namespace hint
{
const float WIDTH = 0.1f;											// 幅
const float HEIGTH = 0.05f;											// 高さ
const string PATH_TRYIT = "data\\TEXTURE\\UI\\tutorialtryit.png";	// やってみようのテクスチャパス
const string PATH_SKIP = "data\\TEXTURE\\UI\\tutorialskip.png";		// スキップのテクスチャパス
D3DXVECTOR3 POS = { 0.85f,0.15f,0.0f };								// 位置
const bool FRAG_TRY[CTutorial::E_State::STATE_MAX] =				// やってみようフラグ
{
	false,	// 何でもない状態
	true,	// 移動状態
	true,	// 突っつき状態
	false,	// 氷説明
	false,	// 破壊説明
	true,	// ジャンプ
	false,	// 敵説明
	false,	// アホウドリ
	false,	// 終了状態
};
}
//----------------------------
// ボタンUI定数
//----------------------------
namespace buttonUI
{
	const string PATH = "data\\TEXTURE\\UI\\BACK_Skip.png";
	const float WIDTH = 0.11f;
	const float HEIGHT = 0.033f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.85f, 0.81f, 0.0f);
}

const string PATH_TEMP_FRAG = "data\\TEMP\\tutorialfrag.bin";	// チュートリアルフラグの保存パス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CTutorial::FuncUpdateState CTutorial::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何でもない状態
	nullptr,					// 移動状態
	nullptr,					// 突っつき状態
	nullptr,					// 氷説明
	nullptr,					// 破壊説明
	nullptr,					// ジャンプ
	nullptr,					// 敵説明
	nullptr,					// アホウドリ説明
	&CTutorial::UpdateEnd,		// 終了状態
};

CTutorial *CTutorial::s_pTutorial = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial() : m_state(E_State::STATE_NONE), m_pManager(nullptr), m_fTimeEnd(0.0f) , m_nCntProgress(0), m_pUIPlayer(nullptr), m_abComplete(),
m_pCaption(nullptr), m_pFadeCaption(nullptr), m_pGaugeSkip(nullptr), m_fTimerSkip(0.0f), m_pHint(nullptr),m_pFadeHint(nullptr), m_pButtonUI(nullptr)
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
		if (!abFrag[i])
			continue;

		CPlayerTutorial *pPlayer = CPlayerTutorial::Create();
	}

	// 全プレイヤーの入力割り当て
	CPlayer::BindInputAllPlayer();

	if (CPlayer::GetNumPlayer() > 1)
	{// 複数人プレイ時のみ行う処理
		for (int i = 0; i < CPlayer::GetNumPlayer(); i++)
		{
			// プレイヤーの人数を取得
			CPlayer* pPlayer = CPlayerTutorial::GetInstance().at(i);

			if (pPlayer == nullptr)
				continue;

			// プレイヤーのモデル再読み込み
			pPlayer->ReLoadModel(&player::PATH_BODY[i][0]);

			// プレイヤーナンバー表示
			pPlayer->ShowNumber();
		}

		// プレイヤーUIの生成
		m_pUIPlayer = CUIPlayer::Create();
	}

	//--------------------------------
	// キャプションの生成
	//--------------------------------
	m_pCaption = CUI::Create();
	
	if (m_pCaption == nullptr)
		return E_FAIL;

	m_pCaption->SetSize(caption::SIZE[E_State::STATE_MOVE].x, caption::SIZE[E_State::STATE_MOVE].y);
	m_pCaption->SetPosition(caption::POS_INIT);
	m_pCaption->SetVtx();

	int nIdxTexture = Texture::GetIdx(&caption::PATH_TEX[E_State::STATE_MOVE][0]);
	m_pCaption->SetIdxTexture(nIdxTexture);

	m_pFadeCaption = CFade2D::Create(m_pCaption, caption::TIME_FADE);

	if (m_pFadeCaption != nullptr)
	{
		m_pFadeCaption->EnableBouceOut(true);
		m_pFadeCaption->SetState(CFade2D::E_State::STATE_IN);
	}

	//--------------------------------
	// ヒントの生成
	//--------------------------------
	m_pHint = CUI::Create();

	if (m_pHint == nullptr)
		return E_FAIL;

	m_pHint->SetSize(hint::WIDTH, hint::HEIGTH);
	m_pHint->SetPosition(hint::POS);
	m_pHint->SetVtx();

	nIdxTexture = Texture::GetIdx(&hint::PATH_TRYIT[0]);
	m_pHint->SetIdxTexture(nIdxTexture);

	m_pFadeHint = CFade2D::Create(m_pHint, caption::TIME_FADE);

	if (m_pFadeHint != nullptr)
	{
		m_pFadeHint->EnableBouceOut(true);
		m_pFadeHint->SetState(CFade2D::E_State::STATE_IN);
	}

	// ゲージの生成
	m_pGaugeSkip = CGauge::Create(gauge::TIME_SKIP);

	if (m_pGaugeSkip == nullptr)
		return E_FAIL;

	m_pGaugeSkip->SetPosition(gauge::POS);

	// ボタンUIの生成
	m_pButtonUI = CUI::Create();
	if (m_pButtonUI != nullptr)
	{
		// 設定
		m_pButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&buttonUI::PATH[0]));	// テクスチャ割当
		m_pButtonUI->SetPosition(buttonUI::POS);					// 位置
		m_pButtonUI->SetSize(buttonUI::WIDTH, buttonUI::HEIGHT);	// 大きさ
		m_pButtonUI->SetVtx();	// 頂点反映
	}

	// チュートリアルフラグをリセット
	tutorial::SaveFrag(false);

	// 流氷ファクトリーの生成
	CFlowIceFct::Create();

	// BGM再生
	Sound::Play(CSound::LABEL::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pManager);
	Object::DeleteObject((CObject**)&m_pUIPlayer);
	Object::DeleteObject((CObject**)&m_pCaption);
	Object::DeleteObject((CObject**)&m_pFadeCaption);
	Object::DeleteObject((CObject**)&m_pButtonUI);

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

	// スキップの入力
	InputSkip();

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
	if (m_pFadeCaption != nullptr)
	{
		if (m_pFadeCaption->GetState() != CFade2D::E_State::STATE_NONE)
			return;
	}

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
// スキップの入力
//=====================================================
void CTutorial::InputSkip(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetPress(CInputManager::BUTTON_SKIP))
	{
		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimerSkip += fDeltaTime;

		if (m_fTimerSkip >= gauge::TIME_SKIP)
		{// 一定時間長押しでタイトルに遷移
			m_fTimerSkip = gauge::TIME_SKIP;

			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
				pFade->SetFade(CScene::MODE_GAME);
		}
	}
	else
	{
		if (m_fTimerSkip < gauge::TIME_SKIP)
		{
			m_fTimerSkip = 0;
		}
	}

	if (m_pGaugeSkip != nullptr)
		m_pGaugeSkip->SetParam(m_fTimerSkip);
}

//=====================================================
// 状態を進める処理
//=====================================================
void CTutorial::ProgressState(void)
{
	// 状態を進める
	m_state = (E_State)(m_state + 1);

	if (m_state == E_State::STATE_ALBATROSS)
		int n = 0;

	// カウンターのリセット
	m_nCntProgress = 0;

	// チェックマークのリセット
	for (CUI *pUI : m_apCheck)
		pUI->Uninit();

	m_apCheck.clear();

	if (m_pFadeCaption != nullptr)
	{// キャプションのフェード設定
		// フェードアウトにして、次の情報を設定
		m_pFadeCaption->SetState(CFade2D::E_State::STATE_OUT);
		m_pFadeCaption->SetPathNext(caption::PATH_TEX[m_state]);
		m_pFadeCaption->SetSizeNext(caption::SIZE[m_state]);

		if (m_state == E_State::STATE_EXPLAIN_ENEMY)
		{// マルチプレイ用のテクスチャ分岐
			if (CPlayer::GetNumPlayer() > 1)
				m_pFadeCaption->SetPathNext(caption::PATH_TEX_ENEMY);
		}
	}

	if (m_pFadeHint != nullptr)
	{// キャプションのフェード設定
		// フェードアウトにして、次の情報を設定
		m_pFadeHint->SetState(CFade2D::E_State::STATE_OUT);

		if(hint::FRAG_TRY[m_state])
			m_pFadeHint->SetPathNext(hint::PATH_TRYIT);
		else
			m_pFadeHint->SetPathNext(hint::PATH_SKIP);
	}

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

namespace tutorial
{
// フラグのセーブ
void SaveFrag(bool bValue)
{
	// ファイルを開く
	std::ofstream outputFile(PATH_TEMP_FRAG, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// データの保存
	outputFile.write(reinterpret_cast<char*>(&bValue), sizeof(bool));

	outputFile.close();
}

// フラグのロード
bool LoadFrag(void)
{
	// ファイルを開く
	std::ifstream inputFile(PATH_TEMP_FRAG, std::ios::binary);

	if (!inputFile.is_open())
		assert(false);

	bool bFrag;

	inputFile.read(reinterpret_cast<char*>(&bFrag), sizeof(bool));

	inputFile.close();

	return bFrag;
}
}