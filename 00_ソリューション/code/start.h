//*****************************************************
//
// スタート表示の処理[start.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _START_H_
#define _START_H_

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
class CStart : CObject
{
public:
	CStart();	// コンストラクタ
	~CStart();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CStart *Create(void);
	static CStart *GetInstance(void) { return s_pStart; }

private:
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_HAKKEYOI,	// はっけよい
		STATE_NOKOTTA,	// のこった
		STATE_MAX
	};

	// メンバ関数
	STATE m_state;	// 状態

	// メンバ変数
	CUI *m_pCaption;	// 見出しのポインタ
	float m_fTimer;	// タイマー

	// 静的メンバ関数
	static CStart *s_pStart;	// 自身のポインタ
};

#endif