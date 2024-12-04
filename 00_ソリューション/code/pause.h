//*****************************************************
//
// ポーズの処理[pause.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

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
class CPause : CObject
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// フェードイン状態
		STATE_OUT,	// フェードアウト状態
		STATE_MAX
	};

	CPause();	// コンストラクタ
	~CPause();	// デストラクタ

	static CPause *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum MENU
	{// 項目の種類
		MENU_RESUME = 0,	// 閉じる
		MENU_RESTART,	// リスタート
		MENU_QUIT,	// タイトルに戻る
		MENU_PAUSE,	// ポーズ
		MENU_MAX
	};

	void ManageState(void);
	void ManageBg(void);
	void Input(void);
	void Fade(MENU menu);
	void OffPosition(void);
	void SelectMove(void);

	MENU m_menu;						// 選択項目
	CUI *m_apMenu[MENU_MAX];			// メニュー項目の２Dオブジェクト
	CUI *m_pBg;							// 背景２Dオブジェクト
	D3DXVECTOR3 m_aPosDest[MENU_MAX];	// 目標の位置
	STATE m_state;						// 状態
	bool m_bSound;
	int nCountMove;
};

#endif