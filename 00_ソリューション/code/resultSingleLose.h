//*****************************************************
//
// シングル負けリザルトの処理[resultSingleLose.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULTSINGLELOSE_H_
#define _RESULTSINGLELOSE_H_

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
class CResultSingleLose : public CResultSingle
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// 何もしてない状態
		STATE_END,			// 終了状態
		STATE_MAX
	};
	CResultSingleLose();	// コンストラクタ
	~CResultSingleLose();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数

private:
	// 列挙型定義
	enum E_Select
	{
		SELECT_NO = 0,	// いいえ
		SELECT_YES,		// はい
		SELECT_MAX
	};

	// メンバ関数
	void Create2D(void);	// 2DUIの生成

	// メンバ変数

	E_State m_state;		// 状態
	float m_fTimer;	// タイマー

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingleLose::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif