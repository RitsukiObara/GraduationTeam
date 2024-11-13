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
	// メンバ関数
	void Create2D(void);	// 2DUIの生成
	void CreatepOwnScore(void);	// 自身のスコアの表示

	void UpdateApperScore(void);	// スコア出現状態の更新

	// メンバ変数
	CUI *m_apCaptionScore[CAPTION_MAX];		// 自身のキャプションのポインタ
	CNumber *m_apNumberOwn[CAPTION_MAX];	// 自身の成績の数字ポインタ

	E_State m_state;		// 状態
	float m_fTimer;	// タイマー

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingleWin::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif