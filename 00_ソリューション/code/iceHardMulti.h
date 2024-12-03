//*****************************************************
//
// マルチ用の硬い氷の処理[iceHardMulti.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ICEHARDMULTI_H_
#define _ICEHARDMULTI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "iceHard.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CIceHardMulti : public CIceHard
{
public:
	CIceHardMulti(int nPriority = 2);	// コンストラクタ
	~CIceHardMulti();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool IsCanPeck(void) override { return false; };	// 突っつけるかの判定を返す

	// 変数取得・設定関数
	void EnableBreak(bool bBreak) override {};

	// 静的メンバ関数

private:

};

#endif

