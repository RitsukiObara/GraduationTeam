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
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CIce : public CPolygon3D
{
public:
	// 列挙型定義
	enum E_State
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
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// 探索可能フラグ
	bool IsCanFind(void) { return m_bCanFind; }
	void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// 破壊フラグ
	bool IsBreak(void) { return m_bBreak; }

	// 静的メンバ関数
	static CIce *Create(E_State state = E_State::STATE_FLOWS);	// 生成処理
	static int GetNumAll(void) { return s_nNumAll; }	// 総数取得

private:
	// メンバ関数
	void Flows(void);	// 流れる処理

	// メンバ変数
	E_State m_state;	// 状態
	bool m_bCanFind;	// 探索できるフラグ
	bool m_bBreak;	// 壊れるフラグ

	// 静的メンバ変数
	static int s_nNumAll;	// 総数
};

#endif

