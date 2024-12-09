//*****************************************************
//
// マルチタイムアップリザルト[resultMultiTimeUp.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULTMULTITIMEUP_H_
#define _RESULTMULTITIMEUP_H_

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
class CResultMultiTimeUp : public CResultSingle
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,		// 何もしてない状態
		STATE_APPER_MENU,	// メニュー選択状態
		STATE_WAIT,			// 待機状態
		STATE_END,			// 終了状態
		STATE_MAX
	};

	CResultMultiTimeUp();	// コンストラクタ
	~CResultMultiTimeUp();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ関数
	void Create2D(void);			// 2DUIの生成
	void CreateMenu(void);			// メニューの生成
	void UpdateApperMenu(void);		// メニューの出現
	void UpdateWait(void);			// 待機中の更新

	// メンバ変数
	E_State m_state;	// 状態
	float m_fTimer;		// タイマー
	CUI *m_pMenu;		// メニュー

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultMultiTimeUp::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif