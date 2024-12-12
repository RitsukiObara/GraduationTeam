//*****************************************************
//
// アザラシの処理[seals.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SEALS_H_
#define _SEALS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "enemy.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CSeals : public CEnemy
{
public:
	enum E_Motion
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK,		// 歩き
		MOTION_STARTJUMP,	// ジャンプ開始
		MOTION_STAYJUMP,	// ジャンプ中
		MOTION_LANDING,		// 着地
		MOTION_SWIM,		// 泳ぎ
		MOTION_TURN,		// 方向転換
		MOTION_BLOW,		// 吹き飛ばしジャンプ
		MOTION_MAX
	};

	CSeals(int nPriority = 5);	// コンストラクタ
	~CSeals();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void UpdateApper(void) override;	// 出現状態の更新
	bool CollideLand(void);	// 着地判定
	void JudgeRetry(void);	// 再度出現判定

	void UpdateStop(void) override;		// 停止状態の更新

	void UpdateMove(void) override;		// 移動状態の更新

	void UpdateAttack(void) override;	// 攻撃状態の更新

	void UpdateDrift(void) override;	// 漂流状態の更新

	void ManageMotion(void);	// モーション管理

	// 静的メンバ関数
	static CSeals* Create(void);

private:
	// メンバ関数
	void SetApperTransform(void) override;
	void CollidePlayer(void);						// プレイヤーとの判定
	void MoveToIce(void);							// 氷に向かって移動
	void Decreasemove(void);						// 移動量の減衰
	void SarchTarget(void);							// ターゲットの探索
	void FindPlayerGrid(void);						// プレイヤーグリッドの発見
	void AliveDestGrid(void) override;				// グリッドに到着したときの処理
	void DecideNextStrollGrid(void);				// 次の散歩先を決める
	void Death(void) override;						// 死亡時の処理
	void Event(EVENT_INFO* pEventInfo) override;	// モーションイベント

	// メンバ変数
	CPlayer *m_pPlayerTarget;	// ターゲットプレイヤー
	D3DXVECTOR3 m_posApper;		// 出現する位置
};

#endif
