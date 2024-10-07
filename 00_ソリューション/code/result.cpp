//*****************************************************
//
// リザルトの処理[result.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "result.h"
#include "manager.h"
#include "ui.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "timer.h"
#include "ranking.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace bg
	{
		const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// パス
		const float WIDTH		= 0.5f;	// 幅
		const float HEIGHT		= 0.5f;	// 高さ
		const float	MOVE_TIME	= 1.5f;	// 移動時間
		const float DEST_ALPHA	= 0.5f;	// 目標色
		const float INIT_ALPHA	= 0.0f;	// 初期色
		const float DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;			// 差分色
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// 位置
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

	namespace curtime
	{
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.48f, 0.41f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// 差分位置
	}

	namespace cont
	{
		const char* PATH = "data\\TEXTURE\\UI\\continue.png";	// パス
		const float	MOVE_TIME = 1.5f;	// 移動時間

		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 目標色
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// 初期色
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// 差分色
		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(0.5f, 0.6f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;						// 差分位置

		const float WIDTH	= 0.3f;		// 幅
		const float HEIGHT	= 0.12f;	// 高さ
	}

	namespace select
	{
		const char* PATH[] =
		{
			"data\\TEXTURE\\UI\\yes.png",	// ゲーム選択肢のパス
			"data\\TEXTURE\\UI\\no.png",	// チュートリアル選択肢のパス
		};

		const float	MOVE_TIME	= 0.84f;	// 移動時間
		const float	PLUS_TIME	= 0.0f;		// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// 待機時間
		const float WIDTH		= 0.4f  * 0.32f;	// 幅
		const float HEIGHT		= 0.29f * 0.32f;	// 高さ
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// 非選択時の色
		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// 選択時の色
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// 初期色
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(0.36f, 0.0f, 0.0f);			// 空白
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f - SPACE.x * 0.5f, 0.85f, 0.0f);	// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS;				// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;	// 差分位置
	}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResult *CResult::s_pResult = nullptr;	// 自身のポインタ
CResult::AFuncUpdateState CResult::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何もしない更新
	&CResult::UpdateFade,		// フェード状態の更新
	&CResult::UpdateClearMove,	// クリア状況移動状態の更新
	&CResult::UpdateContMove,	// コンテニュー移動状態の更新
	&CResult::UpdateSelectMove,	// 選択項目の移動状態の更新
	&CResult::UpdateSelect,		// 選択状態の更新
	nullptr,					// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResult::CResult()
{
	m_pBg = nullptr;
	m_pCaption = nullptr;
	m_pCurTime = nullptr;

	m_pContinue = nullptr;
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));

	m_state = STATE_NONE;
	m_fCurTime = 0.0f;	// 現在の待機時間
	m_nCurSelect = 0;				// 現在の選択肢
	m_nOldSelect = 0;				// 前回の選択肢
}

//====================================================
// デストラクタ
//====================================================
CResult::~CResult()
{

}

//====================================================
// 生成処理
//====================================================
CResult *CResult::Create(bool bWin)
{
	if (s_pResult == nullptr)
	{
		s_pResult = new CResult;
		if (s_pResult != nullptr)
		{
			// 経過時間を書き出し
			CTimer::SaveSecond(CGame::GetInstance()->GetTimeSecond());

			s_pResult->Init();

			s_pResult->Create2D(bWin);

			float fTime = CGame::MAX_TIME - CGame::GetInstance()->GetTimeSecond();
			if (bWin == false)
			{
				fTime = 60;
			}

			CRanking::SetTime(fTime);
		}
	}

	return s_pResult;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResult::Create2D(bool bWin)
{
	char *pPathCaption[2] =
	{
		"data\\TEXTURE\\UI\\lose.png",
		"data\\TEXTURE\\UI\\win.png",
	};

	// 背景の生成
	m_pBg = CUI::Create();
	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(bg::POS);
		m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_pBg->SetVtx();
	}

	// 見出しの生成
	m_pCaption = CUI::Create();
	if (m_pCaption != nullptr)
	{
		m_pCaption->SetIdxTexture(CTexture::GetInstance()->Regist(pPathCaption[bWin]));
		m_pCaption->SetPosition(caption::INIT_POS);
		m_pCaption->SetSize(caption::WIDTH, caption::HEIGHT);
		m_pCaption->SetCol(caption::INIT_COL);
		m_pCaption->SetVtx();
	}

	// クリア時間の生成
	m_pCurTime = CTimer::Create();
	if (m_pCurTime != nullptr)
	{
		m_pCurTime->SetSecond(CGame::MAX_TIME - CTimer::LoadSecond());
		m_pCurTime->SetPosition(curtime::INIT_POS);
		m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::INIT_COL);
	}

	// ロゴの生成
	m_pContinue = CUI::Create();
	if (m_pContinue != nullptr)
	{
		// ロゴの設定
		m_pContinue->SetIdxTexture(CTexture::GetInstance()->Regist(cont::PATH));	// テクスチャ割当
		m_pContinue->SetPosition(cont::INIT_POS);			// 位置
		m_pContinue->SetSize(cont::WIDTH, cont::HEIGHT);	// 大きさ
		m_pContinue->SetCol(cont::INIT_COL);
		m_pContinue->SetVtx();								// 頂点反映
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の生成
		m_apSelect[i] = CUI::Create();
		if (m_apSelect[i] != nullptr)
		{
			// 生成位置を計算
			D3DXVECTOR3 posSelect = select::INIT_POS + (select::SPACE * (float)i);	// 生成位置

			// 選択肢の設定
			m_apSelect[i]->SetIdxTexture(CTexture::GetInstance()->Regist(select::PATH[i]));	// テクスチャ割当
			m_apSelect[i]->SetPosition(posSelect);					// 位置
			m_apSelect[i]->SetSize(select::WIDTH, select::HEIGHT);	// 大きさ
			m_apSelect[i]->SetCol(select::INIT_COL);				// 色
			m_apSelect[i]->SetVtx();								// 頂点反映
		}
	}
}

//=====================================================
//	フェード状態の更新処理
//=====================================================
void CResult::UpdateFade(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 経過時刻の割合を計算
	float fRate = easing::InQuad(m_fCurTime, 0.0f, bg::MOVE_TIME);

	// フェードの透明度を反映
	m_pBg->SetAlpha(bg::INIT_ALPHA + (bg::DIFF_ALPHA * fRate));

	if (m_fCurTime >= bg::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// フェードの透明度を補正
		m_pBg->SetAlpha(bg::DEST_ALPHA);

		// 下線出現状態にする
		m_state = STATE_CLEAR_MOVE;
	}
}

//=====================================================
//	クリア状況移動状態の更新処理
//=====================================================
void CResult::UpdateClearMove(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 経過時刻の割合を計算
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, caption::MOVE_TIME);

	m_pCaption->SetCol(caption::INIT_COL + (caption::DIFF_COL * fRate));
	m_pCaption->SetPosition(caption::INIT_POS + (caption::DIFF_POS * fRate));
	m_pCaption->SetVtx();	// 頂点反映

	m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::INIT_COL + (caption::DIFF_COL * fRate));
	m_pCurTime->SetPosition(curtime::INIT_POS + (curtime::DIFF_POS * fRate));

	if (m_fCurTime >= caption::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		m_pCaption->SetCol(caption::DEST_COL);
		m_pCaption->SetPosition(caption::DEST_POS);
		m_pCaption->SetVtx();	// 頂点反映

		m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::DEST_COL);
		m_pCurTime->SetPosition(curtime::DEST_POS);

		// 下線出現状態にする
		m_state = STATE_CONT_MOVE;
	}
}

//=====================================================
//	コンテニュー移動状態の更新処理
//=====================================================
void CResult::UpdateContMove(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 経過時刻の割合を計算
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, cont::MOVE_TIME);

	m_pContinue->SetCol(cont::INIT_COL + (cont::DIFF_COL * fRate));
	m_pContinue->SetPosition(cont::INIT_POS + (cont::DIFF_POS * fRate));
	m_pContinue->SetVtx();	// 頂点反映

	if (m_fCurTime >= cont::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		m_pContinue->SetCol(cont::DEST_COL);
		m_pContinue->SetPosition(cont::DEST_POS);
		m_pContinue->SetVtx();	// 頂点反映

		// 下線出現状態にする
		m_state = STATE_SELECT_MOVE;
	}
}

//=====================================================
//	選択項目の移動状態の更新処理
//=====================================================
void CResult::UpdateSelectMove(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 選択肢の移動
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// アイコン背景それぞれの経過時間を計算
		float fRateTime = m_fCurTime - (select::PLUS_TIME * (float)i);
		universal::LimitNum(fRateTime, 0.0f, select::MOVE_TIME);	// 経過時間を補正

		// それぞれの経過時刻から割合を計算
		float fRate = easing::InOutQuad(fRateTime, 0.0f, select::MOVE_TIME);

		// 選択肢の位置を計算
		D3DXVECTOR3 posInit = select::INIT_POS + (select::SPACE * (float)i);

		// 選択肢の色を計算
		D3DXCOLOR colCur = select::INIT_COL;
		colCur.a = select::INIT_ALPHA + (select::DIFF_ALPHA * fRate);	// 現在の透明度を設定

		// 選択肢の位置を反映
		m_apSelect[i]->SetPosition(posInit + (select::DIFF_POS * fRate));
		m_apSelect[i]->SetVtx();	// 頂点反映

		// 選択肢の色を反映
		m_apSelect[i]->SetCol(colCur);
	}

	// 選択肢の移動補正
	if (m_fCurTime >= select::MOVE_TIME + select::PLUS_TIME * (SELECT_MAX - 1))
	{ // 全選択肢の待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の目標生成位置を計算
			D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

			// 選択肢の位置を補正
			m_apSelect[i]->SetPosition(posDest);
			m_apSelect[i]->SetVtx();	// 頂点反映

			// 選択肢の色を補正
			m_apSelect[i]->SetCol(select::DEST_COL);
		}

		// 選択状態にする
		m_state = STATE_SELECT;
	}
}

//=====================================================
//	選択状態の更新処理
//=====================================================
void CResult::UpdateSelect(void)
{
	CInputManager* pInput = CInputManager::GetInstance();	// 入力マネージャー情報
	assert(pInput != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	//--------------------------------------------------------
	//	選択肢操作
	//--------------------------------------------------------
	// 前回の選択肢を保存
	m_nOldSelect = m_nCurSelect;

	// 選択肢操作
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{
		// 左に選択をずらす
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{
		// 右に選択をずらす
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}

	// 前回の選択要素の色を白色に設定
	m_apSelect[m_nOldSelect]->SetCol(select::DEFAULT_COL);

	// 現在の選択要素の色を黄色に設定
	m_apSelect[m_nCurSelect]->SetCol(select::CHOICE_COL);

	//--------------------------------------------------------
	//	選択肢決定
	//--------------------------------------------------------
	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{
		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr) { assert(false); return; }
		if (pFade->GetState() != CFade::FADE_NONE) { assert(false); return; }
		switch (m_nCurSelect)
		{ // 選択肢ごとの処理
		case SELECT_YES:

			// ゲームに遷移する
			pFade->SetFade(CScene::MODE_GAME);
			break;

		case SELECT_NO:

			// ランキングに遷移する
			pFade->SetFade(CScene::MODE_RANKING);
			break;

		default:
			assert(false);
			break;
		}

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
}

//====================================================
// 初期化処理
//====================================================
HRESULT CResult::Init(void)
{
	// 初期状態にする
	m_state = STATE_FADE;

	// ゲーム画面をリザルト状態にする
	CGame::SetState(CGame::STATE::STATE_RESULT);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	// サウンドの再生
	pSound->Play(CSound::LABEL_BGM_RESULT);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CResult::Uninit(void)
{
	// 自身のアドレスを破棄
	s_pResult = nullptr;

	Object::DeleteObject((CObject**)&m_pBg);
	Object::DeleteObject((CObject**)&m_pCaption);

	Release();
}

//====================================================
// 更新処理
//====================================================
void CResult::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))();
	}
}

//====================================================
// 描画処理
//====================================================
void CResult::Draw(void)
{

}