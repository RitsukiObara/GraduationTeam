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

private:
	// メンバ関数
	void IsOnPlayer(void);	// プレイヤーの検出
	void UpdateState(void);	// 状態ごとの更新

	// メンバ変数
	float m_fTimerSink;	// 沈むカウンター
};

#endif

