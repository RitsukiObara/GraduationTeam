//*****************************************************
//
// シングル勝ちリザルトの処理[resultSingleWin.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULTSINGLEWIN_H_
#define _RESULTSINGLEWIN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingle.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CNumber;
class CRankingSingle;

//*****************************************************
// クラス定義
//*****************************************************
class CResultSingleWin : public CResultSingle
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// 何もしてない状態
		STATE_MOVECAMERA,	// カメラ移動状態
		STATE_APPERSCORE,	// スコア出現
		STATE_APPERRANKING,	// ランキング出現
		STATE_WAIT,			// 待機状態
		STATE_END,			// 終了状態
		STATE_MAX
	};
	enum E_ScoreCaption
	{
		CAPTION_SCORE = 0,	// スコア
		CAPTION_TIMEPECK,	// つっついた回数
		CAPTION_MAX
	};

	CResultSingleWin();	// コンストラクタ
	~CResultSingleWin();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数

private:
	// 定数定義
	static constexpr int NUM_RANKING = 3;	// ランキングの数

	// メンバ関数
	void Create2D(void);			// 2DUIの生成
	void CreateOwnScore(void);		// 自身のスコアの生成
	void CreateRankingNumber(void);	// ランキング数字の生成

	void UpdateMoveCamera(void);	// カメラ移動状態の更新
	void UpdateApperScore(void);	// スコア出現状態の更新
	void UpdateApperRanking(void);	// ランキング出現状態の更新
	void UpdateWait(void);			// 待機状態の更新

	void Input(void);				// 入力処理

	// メンバ変数
	CUI *m_apCaptionScore[CAPTION_MAX];		// 自身のキャプションのポインタ
	CNumber *m_apNumberOwn[CAPTION_MAX];	// 自身の成績の数字ポインタ

	CNumber *m_apRankingNumber[NUM_RANKING][CAPTION_MAX];	// ランキングの数字の配列
	CRankingSingle *m_pRanking;								// ランキング処理
	CUI *m_pFrame;											// フレームのポリゴン

	E_State m_state;		// 状態
	float m_fTimer;			// タイマー

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingleWin::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif