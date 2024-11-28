//*****************************************************
//
// タイトル処理[title.h]
// Author:若木一真
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMenu;
class CUI;
class CTitleBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしてない状態
		STATE_OUT,		// フェードアウト状態
		START_MAX
	};
	
	enum TITLESTATE
	{
		TITLESTATE_ICEFLOW = 0,	// 氷が流れてくる状態
		TITLESTATE_LOGO,		// ロゴをだす状態
		TITLESTATE_PICKAXE,		// つるはし状態
		TITLESTATE_MAX
	};

	enum TITLE_UI
	{
		TITLE_UI_LEFT = 0,	// 左側
		TITLE_UI_RIGHT,	// 右側
		TITLE_UI_ICEBLOCK,	// 氷ブロック合体
		TITLE_UI_ICE,	// 氷
		TITLE_UI_PENGUIN,	// ペンギン
		TITLE_UI_LOGO,	// ロゴ
		TITLE_UI_PICKAXE,	// つるはし
		TITLE_UI_FLASH,	// フラッシュ
		TITLE_UI_MAX
	};

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	STATE GetState(void) { return m_State; }

private:
	void Input(void);
	void Fade(void);
	void IceFlowState(void);
	void IceConnect(D3DXVECTOR3 *left, D3DXVECTOR3 *right);
	void LogoState(void);
	void PickaxeState(void);

	int m_nCntState;	// カウントの状態
	int m_nCntMove;	// UIの移動カウント
	int m_nCntFade;	// カウントの状態
	int m_nStateLogo;	// スタートロゴカウント
	bool m_bFade; // フェイド
	bool m_bMove; // 移動
	STATE m_State;				// 状態
	TITLESTATE m_TitleState;	// タイトル状態管理
	TITLE_UI m_Title_UI;	// タイトルUI
	CUI* m_apMenu_UI;	// メニューUI
	CUI *m_apTitle_UI[TITLE_UI_MAX];	// タイトルUI
	D3DXVECTOR3 m_aPosDest[TITLE_UI_MAX];	// 目標の位置
};

#endif