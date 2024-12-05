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
class CPlayer;

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
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_JUMP,		// ジャンプ状態
		STATE_MAX
	};

	// メンバ関数
	void IsOnPlayer(void);		// プレイヤーの検出
	void UpdateState(void);		// 状態ごとの更新
	void StartJump(void);			// 飛ばす処理
	void UpdateNormal(void);	// 通常の更新
	void UpdateJump(void);		// ジャンプの更新

	// メンバ変数
	E_State m_state;					// 状態
	float m_fTimerJump;					// 飛ばすカウンター
	vector<CPlayer*> m_apPlayerJump;	// 飛ばすプレイヤーの配列
};

#endif

