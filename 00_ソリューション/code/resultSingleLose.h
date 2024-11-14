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

//*****************************************************
// クラス定義
//*****************************************************
class CResultSingleLose : public CResultSingle
{
public:
	enum E_State
	{
		STATE_NONE = 0,			// 何もしてない状態
		STATE_APPER_MENU,		// メニュー選択状態
		STATE_SELECT,			// 選択状態
		STATE_END,				// 終了状態
		STATE_MAX
	};
	// 列挙型定義
	enum E_Select
	{
		SELECT_CONTINUE = 0,	// いいえ
		SELECT_QUIT,		// はい
		SELECT_MAX
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
	// メンバ関数
	void Create2D(void);			// 2DUIの生成
	void CreateMenu(void);			// メニューの生成
	void UpdateApperMenu(void);		// メニューの出現
	void UpdateSelect(void);		// 選択状態
	void Fade(E_Select select);			// フェードする処理

	// メンバ変数
	E_State m_state;						// 状態
	float m_fTimer;							// タイマー
	CUI *m_apMenu[E_Select::SELECT_MAX];	// メニュー項目のポリゴン
	E_Select m_select;						// 選択項目

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultSingleLose::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif