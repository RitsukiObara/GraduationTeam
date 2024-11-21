//*****************************************************
//
// リザルトのアザラシ[resultseal.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _RESULTSEAL_H_
#define _RESULTSEAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CShadow;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// モード選択画面の遊ぶペンギンオブジェクトクラス
//****************************************
class CResultSeal : public CMotion
{
public:
	// モーション列挙
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK,		// 歩き
		MOTION_STARTJUMP,	// ジャンプ開始
		MOTION_STAYJUMP,	// ジャンプ中
		MOTION_LANDING,		// 着地
		MOTION_SWIM,		// 泳ぎ
		MOTION_TURN,		// 方向転換
		MOTION_MAX
	};

	CResultSeal(D3DXVECTOR3 pos, int nPriority = 4);	// コンストラクタ
	~CResultSeal();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CResultSeal* Create(D3DXVECTOR3 pos);	// 生成処理

private:
	// メンバ関数
	void Move(void);

	// メンバ変数
	CCollisionSphere *m_pCollisionSphere;	// 球の判定
	CShadow *m_pShadow;						// 影のポインタ
	int m_nCounterMove;						// グイッって動くまでのカウンタ
	int m_bMove;							// アザラシ動きます
	float m_fRotateSpeed;						// 移動速度
	const D3DXVECTOR3 c_centorPos;			// 回る中心位置
};

#endif