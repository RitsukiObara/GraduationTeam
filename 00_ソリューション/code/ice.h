//*****************************************************
//
// 氷の処理[ice.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ICE_H_
#define _ICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CIce : public CObjectX
{
public:
	// 列挙型定義
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_STOP,	// 停止状態
		STATE_FLOWS,	// 流れてる状態
		STATE_MAX
	};

	CIce(int nPriority = 2);	// コンストラクタ
	~CIce();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数

	// 静的メンバ関数
	static CIce *Create(STATE state = STATE::STATE_FLOWS);

private:
	// メンバ関数
	void Flows(void);	// 流れる処理

	// メンバ変数
	STATE m_state;	// 状態
};

#endif

