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
		const float	MOVE_TIME	= 1.5f;	// 移動時間
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;						// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f, 0.2f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// 差分位置
		const float WIDTH	= 0.2f;		// 幅
		const float HEIGHT	= 0.17f;	// 高さ
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
	&CResultSingle::UpdateSelect,		// 選択状態の更新
	nullptr,					// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingle::CResultSingle() : m_fTimer(0.0f)
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
		m_state = E_State::STATE_SELECT;

		return;
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