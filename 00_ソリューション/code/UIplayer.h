//*****************************************************
//
// プリヤーUIの処理[UIplayer.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIPLAYER_H_
#define _UIPLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CUIPlayer : public CObject
{
public:
	CUIPlayer(int nPriority = 5);	// コンストラクタ
	~CUIPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void StartScatter(void);	// 散らばり始める処理

	// 取得・設定関数
	CUI *GetIcon(int nIdx) { return m_aIcon[nIdx]->pPolygon; }	// アイコンの取得

	// 静的メンバ関数
	static CUIPlayer *Create();

private:
	// 列挙型定義
	enum E_State 
	{// 状態
		STATE_NONE = 0,	// 何もしていない状態
		STATE_SCATTER,	// 散らばる状態
		STATE_MAX
	};
	// 構造体定義
	struct S_Icon
	{// アイコン
		CUI *pPolygon;		// ポリゴンのポインタ
		float fTimerFade;	// アイコンのフェードタイマー
		bool bFade;			// フェードフラグ

		// コンストラクタ
		S_Icon::S_Icon() : pPolygon(nullptr), fTimerFade(0.0f), bFade(false) {}
	};

	// メンバ関数
	void UpdateScatter(void);	// 散らばる状態の更新
	void UpdateFadeIcon(void);	// アイコンのフェード

	// メンバ変数
	vector<S_Icon*> m_aIcon;	// プレイヤーアイコンの配列
	E_State m_state;			// 状態
	float m_fTimerScatter;		// 散らばるタイマー

	// 関数ポインタ型を定義
	typedef void (CUIPlayer::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数

};

#endif

