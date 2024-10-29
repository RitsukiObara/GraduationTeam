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
class CPolygon2D;
class CMotion;
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

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void ChangeBehavior(CTitleBehavior *pBehavior);
	STATE GetState(void) { return m_state; }

private:
	STATE m_state;				// 状態
	CPolygon2D* m_pLogo;		// タイトルロゴのポインタ
	CTitleBehavior *m_pBehavior;	// ビヘイビア
	float m_fTImerSmoke;	// 煙のスポーンタイマー
};

class CTitleBehavior
{// 基本ビヘイビア
public:
	CTitleBehavior();	// コンストラクタ
	virtual ~CTitleBehavior();	// デストラクタ

	virtual void Update(CTitle *pTItle) = 0;

private:
};

class CTitleStart : public CTitleBehavior
{// スタート表示状態
public:
	CTitleStart();	// コンストラクタ
	virtual ~CTitleStart();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	CPolygon2D *m_pStart;	// スタート表示のポインタ
	CPolygon2D *m_pAfter;	// スタート表示の残像
};

class CTitleMenu : public CTitleBehavior
{// メニュー
public:
	enum MENU
	{
		MENU_GAME = 0,	// ゲーム
		MENU_OPTION,	// 設定
		MENU_MAX
	};

	enum TITLE_UI
	{
		TITLE_UI_LEFT = 0,	// 左側
		TITLE_UI_RIGHT,	// 右側
		TITLE_UI_LOGO,	// ロゴ
		TITLE_UI_ICE,	// 氷
		TITLE_UI_PENGUIN,	// ペンギン
		TITLE_UI_MAX
	};

	CTitleMenu();	// コンストラクタ
	virtual ~CTitleMenu();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	void Input(void);
	void ManageCursor(void);
	void Fade(void);

	CPolygon2D *m_apMenu[MENU_MAX];	// メニュー項目
	CPolygon2D *m_pCursor;	// カーソル
	MENU m_menu;	// 選択メニュー項目
	TITLE_UI m_Title_UI;	// タイトルUI
	CUI* m_pBg;	// 背景２Dオブジェクト
};

#endif