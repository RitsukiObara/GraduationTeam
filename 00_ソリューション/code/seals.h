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
	// 状態列挙
	typedef enum
	{
		STATE_STOP = 0,	// 停止状態
		STATE_MOVE,		// 移動状態
		STATE_ATTACK,	// 攻撃状態
		STATE_DRIFT,	// 流れる状態
		STATE_MAX
	}STATE;

	// 関数ポインタ型の定義
	typedef bool (CSeals::*UpdateState)(void);

	CSeals(int nPriority = 4);	// コンストラクタ
	~CSeals();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void UpdateStop(void);		// 停止状態の更新
	void UpdateMove(void);		// 移動状態の更新
	void UpdateAttack(void);	// 攻撃状態の更新
	void UpdateDrift(void);		// 漂流状態の更新

	// 静的メンバ関数
	static CSeals* Create(void);

private:
	// アザラシの状態
	STATE m_State;
};

#endif
