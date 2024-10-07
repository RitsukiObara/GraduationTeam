//*****************************************************
//
// リザルトの処理[result.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CTimer;

//*****************************************************
// クラス定義
//*****************************************************
class CResult : CObject
{
public:
	enum STATE
	{
		STATE_NONE = 0,		// 何もしてない状態
		STATE_FADE,			// フェード状態
		STATE_CLEAR_MOVE,	// クリア状況移動状態
		STATE_CONT_MOVE,	// コンテニュー移動状態
		STATE_SELECT_MOVE,	// 選択項目の移動状態
		STATE_SELECT,		// 選択状態
		STATE_END,			// 終了状態
		STATE_MAX
	};

	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CResult *Create(bool bWin = false);
	static CResult *GetInstance(void) { return s_pResult; }

private:
	// 列挙型定義
	enum SELECT
	{
		SELECT_YES = 0,	// はい
		SELECT_NO,		// いいえ
		SELECT_MAX
	};

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResult::*AFuncUpdateState)(void);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void Create2D(bool bWin);

	void UpdateFade(void);			// フェード状態の更新
	void UpdateClearMove(void);		// クリア状況移動状態の更新
	void UpdateContMove(void);		// コンテニュー移動状態の更新
	void UpdateSelectMove(void);	// 選択項目の移動状態の更新
	void UpdateSelect(void);		// 選択状態の更新

	// メンバ変数
	CUI *m_pBg;			// 背景のポインタ
	CUI *m_pCaption;	// 見出しのポインタ
	CTimer* m_pCurTime;	// 今回の経過時間

	CUI *m_pContinue;				// コンテニューロゴのポインタ
	CUI *m_apSelect[SELECT_MAX];	// 選択肢のポインタ

	STATE m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
	int m_nCurSelect;	// 現在の選択肢
	int m_nOldSelect;	// 前回の選択肢

	// 静的メンバ関数
	static CResult *s_pResult;	// 自身のポインタ
};

#endif