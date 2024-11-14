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
#include "resultSingleWin.h"
#include "resultSingleLose.h"
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
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace bg
{
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
const int NUM_TEX = 2;													// テクスチャの種類
const char* PATH[NUM_TEX] =
{																		// パス
	"data\\TEXTURE\\UI\\gameover.png",
	"data\\TEXTURE\\UI\\stage_clear.png",
};
const float	MOVE_TIME = 1.5f;											// 移動時間
const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 目標色
const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);			// 初期色
const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;							// 差分色
const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(0.5f, 0.1f, 0.0f);				// 目標位置
const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 0.2f, 0.0f);	// 初期位置
const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;						// 差分位置
const float WIDTH = 0.25f;	// 幅
const float HEIGHT = 0.13f;	// 高さ
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultSingle::FuncUpdateState CResultSingle::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,							// 何もしない更新
	&CResultSingle::UpdateFade,			// フェード状態の更新
	&CResultSingle::UpdateApperCaption,	// キャプション出現状態の更新
	nullptr,							// キャプション出現終了状態の更新
	nullptr,							// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingle::CResultSingle() : m_fTimer(0.0f),m_pCaption(nullptr),m_bWin(false)
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

	if(bWin)
		pResult = new CResultSingleWin;	// 勝利のリザルト
	else 
		pResult = new CResultSingleLose;	// 敗北のリザルト

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

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultSingle::Create2D(bool bWin)
{
	// 背景の生成
	CreateBg();

	// 見出しの生成
	CreateCaption();
}

//====================================================
// 見出しの生成
//====================================================
void CResultSingle::CreateCaption(void)
{
	m_pCaption = CUI::Create();

	if (m_pCaption == nullptr)
		return;

	m_pCaption->SetSize(caption::WIDTH, caption::HEIGHT);
	m_pCaption->SetPosition(caption::INIT_POS);
	m_pCaption->SetCol(caption::INIT_COL);
	m_pCaption->SetVtx();

	int nIdxTexture = Texture::GetIdx(caption::PATH[m_bWin]);
	m_pCaption->SetIdxTexture(nIdxTexture);
}

//====================================================
// 背景の生成
//====================================================
void CResultSingle::CreateBg(void)
{
	m_pBg = CUI::Create();

	if (m_pBg == nullptr)
		return;

	m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
	m_pBg->SetPosition(bg::POS);
	m_pBg->SetCol(bg::INIT_COL);
	m_pBg->SetVtx();
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
		m_state = E_State::STATE_APPERCAPTION;
		m_fTimer = 0.0f;
		Camera::ChangeState(nullptr);

		return;
	}
}

//=====================================================
// キャプション出現状態の更新
//=====================================================
void CResultSingle::UpdateApperCaption(void)
{
	if (m_pCaption == nullptr)
		return;

	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / caption::MOVE_TIME;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	//-----------------------------------------
	// キャプションの移動
	//-----------------------------------------
	D3DXVECTOR3 posCaption = caption::INIT_POS + caption::DIFF_POS * fRate;

	m_pCaption->SetPosition(posCaption);
	m_pCaption->SetVtx();
	m_pCaption->SetAlpha(fRate);

	if (m_fTimer > caption::MOVE_TIME)
	{// 一定時間経過で次の状態に移る
		m_state = E_State::STATE_ENDAPPERCAPTION;
		m_fTimer = 0.0f;
	}
}

//====================================================
// 描画処理
//====================================================
void CResultSingle::Draw(void)
{
#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\nリザルトデバッグ情報===============================");
	CDebugProc::GetInstance()->Print("\n状態[%d]",m_state);
#endif
}