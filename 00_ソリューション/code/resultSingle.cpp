//*****************************************************
//
// シングルリザルトの処理[resultSingle.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingle.h"
#include "manager.h"
#include "ui.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "timer.h"
#include "ranking.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace bg
{
const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// パス
const float WIDTH = 0.5f;	// 幅
const float HEIGHT = 0.5f;	// 高さ
const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 初期の色
const float DEST_ALPHA = 0.5f;	// 目標色
const float DIFF_ALPHA = DEST_ALPHA - INIT_COL.a;			// 差分アルファ値
const D3DXVECTOR3 POS = D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// 位置
const float TIME_FADE = 2.0f;	// フェードにかかる時間
}

namespace caption
{
const float	MOVE_TIME = 1.5f;	// 移動時間
const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 目標色
const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// 初期色
const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// 差分色
const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(0.5f, 0.2f, 0.0f);			// 目標位置
const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// 初期位置
const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;						// 差分位置
const float WIDTH = 0.2f;		// 幅
const float HEIGHT = 0.17f;	// 高さ
}

namespace scoreCaption
{
const float TIME_APPER = 2.0f;	// 出現にかかる時間
const float WIDTH = 0.16f;	// 幅
const float HEIGHT = 0.1f;	// 高さ
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingle::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.3f,-HEIGHT_INIT,0.0f },
	{ 0.7f,-HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

namespace scoreNumber
{
const float SIZE_INIT = 0.03f;	// サイズ
const float HEIGHT_INIT = -0.1f;
const D3DXVECTOR3 POS_INIT[CResultSingle::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

namespace score
{
const int NUM_PLACE[CResultSingle::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// スコアの桁数
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultSingle::FuncUpdateState CResultSingle::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何もしない更新
	nullptr,					// カメラ移動の更新
	&CResultSingle::UpdateFade,		// フェード状態の更新
	&CResultSingle::UpdateApperScore,		// フェード状態の更新
	&CResultSingle::UpdateSelect,		// 選択状態の更新
	nullptr,					// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingle::CResultSingle() : m_fTimer(0.0f), m_apCaptionScore(),m_apNumberOwn(),m_bWin(false)
{

}

//====================================================
// デストラクタ
//====================================================
CResultSingle::~CResultSingle()
{

}

//====================================================
// 生成処理
//====================================================
CResultSingle *CResultSingle::Create(bool bWin)
{
	CResultSingle *pResult = nullptr;

	pResult = new CResultSingle;

	if (pResult != nullptr)
	{
		pResult->m_bWin = bWin;
		pResult->Init();
	}

	return pResult;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CResultSingle::Init(void)
{
	// ゲーム画面をリザルト状態にする
	CGame::SetState(CGame::E_State::STATE_RESULT);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	// サウンドの再生
	pSound->Play(CSound::LABEL_BGM_RESULT);

	// 2Dオブジェクトの生成
	Create2D(m_bWin);

	// 状態の初期設定
	if (m_bWin)
	{
		m_state = STATE_MOVECAMERA;

		Camera::ChangeState(new CCameraResultSingle(this));
	}
	else
		m_state = STATE_FADE;

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultSingle::Create2D(bool bWin)
{
	//-----------------------------------------
	// 背景の生成
	//-----------------------------------------
	m_pBg = CUI::Create();

	if (m_pBg == nullptr)
		return;

	m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
	m_pBg->SetPosition(bg::POS);
	m_pBg->SetCol(bg::INIT_COL);
	m_pBg->SetVtx();

	//-----------------------------------------
	// 自身の成績表示の生成
	//-----------------------------------------
	for (int i = 0; i < E_ScoreCaption::CAPTION_MAX; i++)
	{
		// キャプションの生成
		m_apCaptionScore[i] = CUI::Create();
		if (m_apCaptionScore[i] == nullptr)
			continue;

		m_apCaptionScore[i]->SetSize(scoreCaption::WIDTH, scoreCaption::HEIGHT);
		m_apCaptionScore[i]->SetPosition(scoreCaption::POS_INIT[i]);
		m_apCaptionScore[i]->SetVtx();

		// 数字の生成
		m_apNumberOwn[i] = CNumber::Create(score::NUM_PLACE[i], 0);
		if (m_apNumberOwn[i] == nullptr)
			continue;

		m_apNumberOwn[i]->SetPosition(scoreNumber::POS_INIT[i]);
		m_apNumberOwn[i]->SetSizeAll(scoreNumber::SIZE_INIT, scoreNumber::SIZE_INIT);
	}
}

//====================================================
// 終了処理
//====================================================
void CResultSingle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pBg);
	Object::DeleteObject((CObject**)&m_pCaption);

	Release();
}

//====================================================
// 更新処理
//====================================================
void CResultSingle::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// フェード状態の更新処理
//=====================================================
void CResultSingle::UpdateFade(void)
{
	if (m_pBg == nullptr)
		return;

	m_fTimer += CManager::GetDeltaTime();

	// タイマーのイージング
	float fTime = m_fTimer / bg::TIME_FADE;
	float fRate = easing::EaseOutExpo(fTime);

	// 色の設定
	m_pBg->SetAlpha(bg::INIT_COL.a + bg::DIFF_ALPHA * fRate);

	if (bg::TIME_FADE < m_fTimer)
	{// フェード状態の終了
		m_state = E_State::STATE_APPERSCORE;
		m_fTimer = 0.0f;
		Camera::ChangeState(nullptr);

		return;
	}
}

//=====================================================
// スコア出現状態状態の更新処理
//=====================================================
void CResultSingle::UpdateApperScore(void)
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

	}
}

//=====================================================
// 選択状態の更新処理
//=====================================================
void CResultSingle::UpdateSelect(void)
{

}

//====================================================
// 描画処理
//====================================================
void CResultSingle::Draw(void)
{

}

//====================================================
// カメラ移動の終了
//====================================================
void CResultSingle::EndMove(void)
{
	m_state = E_State::STATE_FADE;
}