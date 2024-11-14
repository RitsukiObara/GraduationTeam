//*****************************************************
//
// シングル勝ちリザルトの処理[resultSingleWin.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingleWin.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "inputManager.h"
#include "fade.h"
#include "score.h"
#include "game.h"
#include "player.h"
#include "rankingSingle.h"
#include "gameManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
//----------------------------
// スコア見出し定数
//----------------------------
namespace scoreCaption
{
const string PATH_TEX[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
{// キャプションのパス
	"data\\TEXTURE\\UI\\result_score.png",
	"data\\TEXTURE\\UI\\result_peck.png",
};
const float TIME_APPER = 2.0f;		// 出現にかかる時間
const float WIDTH = 0.2f;			// 幅
const float HEIGHT = 0.15f;			// 高さ
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.3f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.37f;						// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

//----------------------------
// スコア数字定数
//----------------------------
namespace scoreNumber
{
const float SIZE_INIT = 0.03f;	// サイズ
const float HEIGHT_INIT = -0.1f;
const D3DXVECTOR3 POS_INIT[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.45f;	// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

//----------------------------
// スコア定数
//----------------------------
namespace score
{
const int NUM_PLACE[CResultSingleWin::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// スコアの桁数
}

//----------------------------
// ランキング定数
//----------------------------
namespace ranking
{
const string PATH_SAVE[] =
{// ステージの数分のパス
	"data\\BYNARY\\ranking00.bin",
	"data\\BYNARY\\ranking01.bin",
	"data\\BYNARY\\ranking02.bin",
	"data\\BYNARY\\ranking03.bin",
};
const D3DXVECTOR3 POS_NUMBER_DEFAULT = { 0.2f, 0.6f,0.0f };	// 数字のデフォルト位置
const float DIST_NUMBER = 0.1f;									// 数字間の距離
const float WIDTH_NUMBER = 0.025f;								// 数字の幅
const float HEIGHT_NUMBER = 0.03f;								// 数字の高さ
const float OFFSET_TIMEPECK = 0.5f;								// 突っついた回数表示のオフセット

const float TIME_APPER = 2.0f;						// 出現にかかる時間
const float ALPHA_INIT = 0.0f;						// 初期アルファ値
const float ALPHA_DEST = 1.0f;						// 目標のアルファ値
const float ALPHA_DIFF = ALPHA_DEST - ALPHA_INIT;	// 差分アルファ値
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultSingleWin::FuncUpdateState CResultSingleWin::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,									// 何もしない更新
	&CResultSingleWin::UpdateMoveCamera,		// カメラ移動の更新
	&CResultSingleWin::UpdateApperScore,		// スコア出現状態の更新
	&CResultSingleWin::UpdateApperRanking,		// ランキング出現状態の更新
	&CResultSingleWin::UpdateWait,				// 待機状態の更新
	nullptr,									// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingleWin::CResultSingleWin() : m_fTimer(0.0f), m_apCaptionScore(),m_apNumberOwn(), m_pRanking(nullptr), m_apRankingNumber()
{

}

//====================================================
// デストラクタ
//====================================================
CResultSingleWin::~CResultSingleWin()
{

}

//====================================================
// 初期化処理
//====================================================
HRESULT CResultSingleWin::Init(void)
{
	// 親クラスの初期化
	CResultSingle::Init();

	// ランキング処理の生成
	int nIdxMap = gameManager::LoadIdxMap();
	m_pRanking = CRankingSingle::Create(ranking::PATH_SAVE[nIdxMap]);

	// 2Dオブジェクトの生成
	Create2D();

	// 状態の初期化
	m_state = E_State::STATE_MOVECAMERA;

	// カメラの設定
	Camera::ChangeState(new CCameraResultSingle(this));

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultSingleWin::Create2D(void)
{
	// 自身のスコアの生成
	CreateOwnScore();

	// ランキング数字の生成
	CreateRankingNumber();
}

//====================================================
// 自身のスコアの生成
//====================================================
void CResultSingleWin::CreateOwnScore(void)
{
	//----------------------------
	// スコアの取得
	//----------------------------
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr)
		return;

	CScore *pScore = pGame->GetScore();
	if (pScore == nullptr)
		return;

	int nScore = pScore->GetScore();

	//----------------------------
	// 突いた回数の取得
	//----------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();	// プレイヤーの取得
	assert(!apPlayer.empty());

	CPlayer *pPlayer = *apPlayer.begin();
	assert(pPlayer != nullptr);

	int nTimePeck = pPlayer->GetTimePeck();

	//----------------------------
	// ポリゴンの生成
	//----------------------------
	int aData[E_ScoreCaption::CAPTION_MAX] = { nScore,nTimePeck };	// 数字の値
	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		// キャプションの生成
		m_apCaptionScore[i] = CUI::Create();
		if (m_apCaptionScore[i] == nullptr)
			continue;

		m_apCaptionScore[i]->SetSize(scoreCaption::WIDTH, scoreCaption::HEIGHT);
		m_apCaptionScore[i]->SetPosition(scoreCaption::POS_INIT[i]);
		int nIdxTexture = Texture::GetIdx(&scoreCaption::PATH_TEX[i][0]);
		m_apCaptionScore[i]->SetIdxTexture(nIdxTexture);
		m_apCaptionScore[i]->SetVtx();

		// 数字の生成
		m_apNumberOwn[i] = CNumber::Create(score::NUM_PLACE[i], aData[i]);
		if (m_apNumberOwn[i] == nullptr)
			continue;

		m_apNumberOwn[i]->SetPosition(scoreNumber::POS_INIT[i]);
		m_apNumberOwn[i]->SetSizeAll(scoreNumber::SIZE_INIT, scoreNumber::SIZE_INIT);
	}
}

//====================================================
// ランキング数字の生成
//====================================================
void CResultSingleWin::CreateRankingNumber(void)
{
	if (m_pRanking == nullptr)
		return;

	vector<CRankingSingle::S_InfoRank*> aInfoRank = m_pRanking->GetRank();

	if (aInfoRank.empty())
		return;

	for (int i = 0; i < CResultSingleWin::NUM_RANKING; i++)
	{
		int aData[E_ScoreCaption::CAPTION_MAX] = { aInfoRank[i]->nScore,aInfoRank[i]->nTimePeck };

		for (int j = 0; j < E_ScoreCaption::CAPTION_MAX; j++)
		{
			// 数字の生成
			CNumber *pNumber = CNumber::Create(score::NUM_PLACE[j], aData[j]);
			if (pNumber == nullptr)
				continue;

			// 位置の計算
			D3DXVECTOR3 pos = ranking::POS_NUMBER_DEFAULT;
			pos.y += ranking::DIST_NUMBER * i;	// 数字を下にずらす

			if (j == E_ScoreCaption::CAPTION_TIMEPECK)
				pos.x += ranking::OFFSET_TIMEPECK;	// つっついた回数の表示はずらす

			// トランスフォームの設定
			pNumber->SetPosition(pos);
			pNumber->SetSizeAll(ranking::WIDTH_NUMBER,ranking::HEIGHT_NUMBER);

			// アルファ値の初期設定
			pNumber->SetAlpha(ranking::ALPHA_INIT);

			// ポインタを保存
			m_apRankingNumber[i][j] = pNumber;
		}
	}
}

//====================================================
// 終了処理
//====================================================
void CResultSingleWin::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pRanking);

	CResultSingle::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CResultSingleWin::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// 親クラスの更新
	CResultSingle::Update();
}

//=====================================================
// カメラ移動状態の更新
//=====================================================
void CResultSingleWin::UpdateMoveCamera(void)
{
	if (GetState() == CResultSingle::E_State::STATE_ENDFADE)
		m_state = E_State::STATE_APPERSCORE;
}

//=====================================================
// スコア出現状態の更新処理
//=====================================================
void CResultSingleWin::UpdateApperScore(void)
{
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / scoreCaption::TIME_APPER;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		//-----------------------------------------
		// キャプションの移動
		//-----------------------------------------
		if (m_apCaptionScore[i] == nullptr)
			continue;

		D3DXVECTOR3 posCaption = m_apCaptionScore[i]->GetPosition();
		posCaption.y = scoreCaption::HEIGHT_INIT + scoreCaption::DIFF_HEIGHT * fRate;

		m_apCaptionScore[i]->SetPosition(posCaption);
		m_apCaptionScore[i]->SetVtx();

		//-----------------------------------------
		// 数字の移動
		//-----------------------------------------
		// 数字の生成
		if (m_apNumberOwn[i] == nullptr)
			continue;

		D3DXVECTOR3 posNumber = m_apNumberOwn[i]->GetPosition();
		posNumber.y = scoreNumber::HEIGHT_INIT + scoreCaption::DIFF_HEIGHT * fRate;

		m_apNumberOwn[i]->SetPosition(posNumber);
	}

	if (m_fTimer > scoreCaption::TIME_APPER)
	{// 一定時間経過で次の状態に移る
		m_state = E_State::STATE_APPERRANKING;
		m_fTimer = 0.0f;
	}
}

//=====================================================
// ランキング出現状態の更新処理
//=====================================================
void CResultSingleWin::UpdateApperRanking(void)
{
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / scoreCaption::TIME_APPER;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	// 数字の色設定
	for (int i = 0; i < CResultSingleWin::NUM_RANKING; i++)
	{
		for (int j = 0; j < E_ScoreCaption::CAPTION_MAX; j++)
		{
			if (m_apRankingNumber[i][j] == nullptr)
				continue;

			float fAlpha = ranking::ALPHA_INIT + ranking::ALPHA_DIFF * fRate;

			m_apRankingNumber[i][j]->SetAlpha(fAlpha);
		}
	}

	if (m_fTimer > ranking::TIME_APPER)
		m_state = E_State::STATE_WAIT;
}

//=====================================================
// 待機状態の更新処理
//=====================================================
void CResultSingleWin::UpdateWait(void)
{
	vector<CInputManager*> aInputMgr = CInputManager::GetArray();

	for (CInputManager *pInputMgr : aInputMgr)
	{
		if (pInputMgr == nullptr)
			continue;

		if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		{// フェードしてタイトルに戻る
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
				pFade->SetFade(CScene::MODE::MODE_TITLE);
		}
	}
}

//====================================================
// 描画処理
//====================================================
void CResultSingleWin::Draw(void)
{
	// 親クラスの描画
	CResultSingle::Draw();
}