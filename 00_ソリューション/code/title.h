//*****************************************************
//
// タイトル処理[title.h]
// Author:藤田勇一
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,		// 何もしてない状態
		STATE_LOGO_MOVE,	// ロゴの移動状態
		STATE_SELECT_MOVE,	// 選択項目の移動状態
		STATE_SELECT,		// 選択状態
		STATE_END,			// 終了状態
		STATE_MAX
	};

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	STATE GetState(void) { return m_state; }
	
private:
	enum SELECT
	{
		SELECT_GAME = 0,	// ゲーム開始
		SELECT_TUTORIAL,	// チュートリアル表示
		SELECT_MAX
	};

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CTitle::*AFuncUpdateState)(void);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void UpdateLogoMove(void);		// ロゴの移動状態の更新
	void UpdateSelectMove(void);	// 選択項目の移動状態の更新
	void UpdateSelect(void);		// 選択状態の更新
	void CreateLight(void);			// ライトの生成

	// メンバ変数
	STATE m_state;					// 状態
	CUI *m_apSelect[SELECT_MAX];	// 選択肢のポインタ
	CUI *m_pLogo;					// ロゴのポインタ
	float m_fCurTime;				// 現在の待機時間
	int m_nCurSelect;				// 現在の選択肢
	int m_nOldSelect;				// 前回の選択肢
};

#endif