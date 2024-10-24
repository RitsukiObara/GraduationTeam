//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;
class CIce;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	// モーション列挙
	enum MOTION
	{
		MOTION_NEUTRAL = 0,
		MOTION_JUMPSTART,
		MOTION_JUMPFLY,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_MAX
	};

	// プレイヤー状態
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常
		STATE_DEATH,		// 死
		STATE_INVINCIBLE,	// デバッグ向け無敵
		STATE_MAX
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPlayer* Create(void);
	static CPlayer* GetInstance(void) { return s_pPlayer; }

	// 取得・設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 移動量
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// 入力可能フラグ
	bool IsEnableInput(void) { return m_bEnableInput; }

private:
	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	void Input(void);	// 入力
	void MoveAnalog(void);	// アナログ移動
	void InputMoveAnalog(void);	// アナログ移動入力
	void CollideIce(void);	// 氷との判定
	CIce *SelectIceByRot(float fRot);	// 氷を向きで取得
	void WalkToDestIce(CIce *pIceDest);	// 目標の氷に向かって移動する処理
	bool CheckGridChange(void);	// グリッドが変わったかどうかの判定
	void InputPeck(void);	// 突っつきの入力
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号
	bool m_bEnableInput;	// 入力可能フラグ
	D3DXVECTOR3 m_move;	// 移動量
	float m_fTimerStartMove;	// 移動の立ち上がりのタイマー
	STATE m_state;		// プレイヤー状態
	CIce *m_pIceMoveDest;	// 移動目標の氷

	// 静的メンバ変数
	static CPlayer* s_pPlayer;	// 自身のポインタ
};

#endif