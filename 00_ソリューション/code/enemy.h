//*****************************************************
//
// 敵の処理[enemy.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// インクルード
//*****************************************************
#include "rikishi.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemy : public CRikishi
{
public:
	CEnemy(int nPriority = 4);	// コンストラクタ
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	float Hit(E_Attack typeAttack, float fDamage) override;	// ヒット

	// 静的メンバ関数
	static CEnemy *Create(void);
	static CEnemy *GetInstance(void) { return m_pEnemy; }

private:
	// メンバ関数
	void Debug(void);	// デバッグ
	void Attack(E_Attack typeAttack) override;	// 攻撃
	void UpdatePosition(void);	// 位置更新
	void UpdateAttack(void);	// 攻撃更新
	void RandomAttack(void);	// 攻撃ランダム生成
	void HitAttack(E_Attack typeAttack) override;

	// 静的メンバ変数
	static CEnemy *m_pEnemy;	// 自身のポインタ

	// メンバ変数
	int m_aAtkNum[3];		// 各攻撃の確率
	int m_nMaxAtkStart;		// 攻撃開始値の最大値
	float m_fLastAtkTime;	// 前回攻撃からの経過時間
};

#endif