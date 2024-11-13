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
class CNumber;

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
		STATE_ENDFADE,		// フェード終了状態
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

	// 変数取得・設定関数
	void SetState(CResultSingle::E_State state) { m_state = state; }	// 状態
	CResultSingle::E_State GetState(void) { return m_state; }

	// 静的メンバ関数
	static CResultSingle *Create(bool bWin = false);

private:
	// メンバ関数
	void Create2D(bool bWin);	// 2DUIの生成
	void CreateBg(void);		// 背景の生成
	
	void UpdateFade(void);			// フェード状態の更新

	// メンバ変数
	bool m_bWin;	// 勝利フラグ
	CUI *m_pBg;			// 背景のポインタ
	CUI *m_pCaption;	// 見出しのポインタ

	E_State m_state;		// 状態
	float m_fTimer;	// タイマー

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingle::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif