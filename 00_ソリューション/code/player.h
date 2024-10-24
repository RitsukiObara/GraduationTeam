//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:��山桃也
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
class CInputManager;
class CFlowIce;

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
		MOTION_WALK,
		MOTION_STARTJUMP,
		MOTION_STAYJUMP,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_FLOW,
		MOTION_MAX
	};
	// プレイヤー状態
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常
		STATE_FLOW,		// 流されている
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

	// 取得・設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 移動量
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// 入力可能フラグ
	bool IsEnableInput(void) { return m_bEnableInput; }

	// 静的メンバ関数
	static CPlayer* Create(void);

private:
	// 構造体定義
	struct S_FragMotion
	{// モーションフラグの構造体
		bool bWalk;	// 歩行
		bool bPeck;	// 突っつき
	};

	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	
	void Input(void);	// 入力
	
	void MoveAnalog(void);	// アナログ移動
	void InputMoveAnalog(void);	// アナログ移動入力
	void DisableTurn(void);	// 振り返りの無効化
	void Forward(void);	// 前進処理
	void DecreaseMove(void);	// 移動量の減衰
	void FactingRot(void);	// 向きの補正
	void JudgeTurn(void);	// 振り向きの検出
	
	void CollideIce(void);	// 氷との判定

	void InputPeck(void);	// 突っつきの入力
	
	CIce *SelectIceByRot(float fRot);	// 氷を向きで取得
	void WalkToDestIce(CIce *pIceDest);	// 目標の氷に向かって移動する処理
	bool CheckGridChange(void);	// グリッドが変わったかどうかの判定
	
	void StartFlows(void);	// 漂流開始
	bool FindFlowIce(void);	// 漂流する氷の検出
	void StayFlow(void);	// 漂流中の処理
	void EndFlows(void);	// 漂流終了

	void InputJump(void);	// ジャンプの入力
	void SarchJumpIce(void);	// ジャンプ先の氷を探す
	void StartJump(void);	// ジャンプの開始
	void StayJump(void);	// ジャンプ中の処理
	void EndJump(void);		// ジャンプの終了

	void ManageMotion(void);	// モーションの管理

	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号
	bool m_bEnableInput;	// 入力可能フラグ
	bool m_bTurn;	// 振り返っているフラグ
	float m_fRotTurn;	// 振り返る角度
	D3DXVECTOR3 m_move;	// 移動量
	float m_fTimerStartMove;	// 移動の立ち上がりのタイマー
	STATE m_state;		// プレイヤー状態
	CIce *m_pIceMoveDest;	// 移動目標の氷
	CFlowIce *m_pLandSystemFlow;	// 乗ってる流氷システム
	S_FragMotion m_fragMotion;	// モーションフラグ
	CInputManager *m_pInputMgr;	// 入力マネージャー
};

#endif