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
#include "rikishi.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CRikishi
{
public:
	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float Hit(E_Attack typeAttack, float fDamage) override;

	// 静的メンバ関数
	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }

private:
	// メンバ関数
	void Input(void);	// 入力
	void Attack(E_Attack typeAttack) override;
	void HitAttack(E_Attack typeAttack) override;

	// メンバ変数
	float m_aTimerAttack[CRikishi::ATTACK_MAX];	// 攻撃カウンター
	CPolygon3D *m_pGuide;	// ガイド表示

	// 静的メンバ変数
	static CPlayer *m_pPlayer;	// 自身のポインタ
};

#endif