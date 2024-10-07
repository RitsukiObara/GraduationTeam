//*****************************************************
//
// 力士の処理[rikishi.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RIKISHI_H_
#define _RIKISHI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CRikishi : public CMotion
{
public:
	// 列挙型定義
	enum E_Attack
	{// 攻撃の種類
		ATTACK_RIGHT = 0,
		ATTACK_LEFT,
		ATTACK_BOTHHAND,
		ATTACK_MAX
	};
	enum E_Motion
	{// モーション
		MOTION_GUARD = 0,
		MOTION_ATTACK_RIGHT,
		MOTION_ATTACK_LEFT,
		MOTION_ATTACK_BOTHHAND,
		MOTION_MAX
	};
	enum E_Parts
	{// パーツ番号
		IDX_WAIST = 0,
		IDX_BODY,
	};
	// 構造体定義
	struct S_FragMotion
	{// モーションフラグ
		bool bAttack[ATTACK_MAX];	// 攻撃モーション
		bool bGuard;	// ガードモーション
	};
	
	CRikishi(int nPriority = 4);	// コンストラクタ
	~CRikishi();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual float Hit(E_Attack typeAttack,float fDamage);
	virtual void Attack(E_Attack typeAttack);
	virtual void HitAttack(E_Attack typeAttack) = 0;
	void Guard(void);	// ガード

	// 変数取得・設定関数
	void SetFragMotion(S_FragMotion fragMotion) { m_fragMotion = fragMotion; }
	S_FragMotion GetFragMotion(void) { return m_fragMotion; }

	// 静的メンバ関数
	static CRikishi *Create(void);

private:
	// メンバ関数
	void ManageMotion(void);	// モーション管理
	void Event(EVENT_INFO *pEventInfo) override;

	// メンバ変数
	S_FragMotion m_fragMotion;	// モーションフラグ
};

namespace Rikishi
{
const float DISTANCE_RIKISHI = 100.0f;	// 力士同士の距離
const float DAMAGE_ATTACK = 100.0f;	// 攻撃力
}

#endif