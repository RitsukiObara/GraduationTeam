//*****************************************************
//
// シングルリザルトの処理[resultSingle.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULTSINGLE_H_
#define _RESULTSINGLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラス定義
//*****************************************************
class CResultSingle : CObject
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// 何もしてない状態
		STATE_FADE,			// フェード状態
		STATE_SELECT,		// 選択状態
		STATE_END,			// 終了状態
		STATE_MAX
	};

	CResultSingle();	// コンストラクタ
	~CResultSingle();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CResultSingle *Create(bool bWin = false);

private:
	// 列挙型定義
	enum SELECT
	{
		SELECT_YES = 0,	// はい
		SELECT_NO,		// いいえ
		SELECT_MAX
	};

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingle::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void Create2D(bool bWin);

	void UpdateFade(void);			// フェード状態の更新
	void UpdateSelect(void);		// 選択状態の更新

	// メンバ変数
	CUI *m_pBg;			// 背景のポインタ
	CUI *m_pCaption;	// 見出しのポインタ

	CUI *m_pContinue;				// コンテニューロゴのポインタ
	CUI *m_apSelect[SELECT_MAX];	// 選択肢のポインタ

	E_State m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
	int m_nCurSelect;	// 現在の選択肢
	int m_nOldSelect;	// 前回の選択肢
};

#endif