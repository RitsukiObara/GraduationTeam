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

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	STATE GetState(void) { return m_state; }

private:
	void Input(void);
	void Fade(void);
	void ManageState(void);
	void TitleUIState(void);

	STATE m_state;				// 状態
	CPolygon2D* m_pLogo;		// タイトルロゴのポインタ
	CUI *m_apTitle_UI[TITLE_UI_MAX];	// タイトルUI
	CUI *m_apMenu_UI[MENU_MAX];	// タイトルUI
	D3DXVECTOR3 m_aPosDest[TITLE_UI_MAX];	// 目標の位置
	MENU m_menu;	// 選択メニュー項目
	TITLE_UI m_Title_UI;	// タイトルUI
};

#endif