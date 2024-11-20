//*****************************************************
//
// シロクマの処理[bears.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _BEARS_H_
#define _BEARS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CBears : public CEnemy
{
public:
	CBears(int nPriority = 4);	// コンストラクタ
	~CBears();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void UpdateApper(void) override;	// 出現状態の更新
	bool CollideLand(void);	// 着地判定

	void UpdateStop(void) override;		// 停止状態の更新

	void UpdateMove(void) override;		// 移動状態の更新

	void UpdateAttack(void) override;	// 攻撃状態の更新

	void UpdateDrift(void) override;	// 漂流状態の更新

	void ManageMotion(void);	// モーション管理

	// 静的メンバ関数
	static CBears* Create(void);

private:
	enum E_Motion
	{// モーション
		MOTION_WALK = 0,	// 歩き
		MOTION_CHARGE,		// 突撃
		MOTION_STARTJUMP,	// ジャンプ開始
		MOTION_STAYJUMP,	// ジャンプ中
		MOTION_LANDING,		// 着地
		MOTION_TURN,		// 振り向き
		MOTION_TURNCHARGE,	// 突進振り向き
		MOTION_MAX
	};

	// メンバ関数
	void SetApperTransform(void) override;
	void CollidePlayer(void);						// プレイヤーとの判定
	void MoveToIce(void);							// 氷に向かって移動
	void Decreasemove(void);						// 移動量の減衰
	void SarchTarget(void);							// ターゲットの探索
	void StopMoveByNotGrid(CIce *pIce) override;	// グリッド基準じゃない移動を止める

	bool CanCharge(D3DXVECTOR3 pos, int nIdxTargetV, int nIdxTargetH);							// 突撃できるかの判定
	bool IsAliveTarget(int nIdxV, int nIdxH,float fRot,int nIdxTargetV, int nIdxTargetH);		// ターゲットに到達したかの再帰関数

	void ReadyCharge(void);	// 突撃の準備
	void StartCharge(void);	// 突撃の開始
	void Charge(void);		// 突撃中の処理
	void EndCharge(void);	// 突撃の終了

	void AliveDestGrid(void) override;				// グリッドに到着したときの処理
	void DecideNextStrollGrid(void);				// 次の散歩先を決める
	void Death(void) override;						// 死亡時の処理
	void Event(EVENT_INFO* pEventInfo) override;	// モーションイベント

	// メンバ変数
	CPlayer *m_pPlayerTarget;	// ターゲットプレイヤー
	D3DXVECTOR3 m_vecCharge;	// 突進するベクトル
	float m_fTimerAcceleCharge;	// 突進加速カウンター
};

#endif
