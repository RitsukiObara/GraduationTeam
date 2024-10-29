//*****************************************************
//
// アザラシの処理[seals.h]
// Author:早川友貴
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
	CSeals(int nPriority = 4);	// コンストラクタ
	~CSeals();	// デストラクタ

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
	static CSeals* Create(void);

private:
	enum E_Motion
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK,		// 歩き
		MOTION_STARTJUMP,	// ジャンプ開始
		MOTION_STAYJUMP,	// ジャンプ中
		MOTION_LANDING,		// 着地
		MOTION_MAX
	};

	// メンバ関数
	void SetApperTransform(void) override;
	void CollidePlayer(void);	// プレイヤーとの判定
	void MoveToIce(void);	// 氷に向かって移動
	void SarchTarget(void);	// ターゲットの探索
	void FindPlayerGrid(void);	// プレイヤーグリッドの発見

	// メンバ変数
	CPlayer *m_pPlayerTarget;	// ターゲットプレイヤー
};

#endif
