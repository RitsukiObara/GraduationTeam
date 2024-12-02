//*****************************************************
//
// プレイヤー操作なしペンギン[npcpenguin.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _SELECTMODEPENGUIN_H_
#define _SELECTMODEPENGUIN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CShadow;
__interface INPCPenguinState;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// モード選択画面の遊ぶペンギンオブジェクトクラス
//****************************************
class CNPCPenguin : public CMotion
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
		MOTION_VICTORY,
		MOTION_FAIL,
		MOTION_GUTS,
		MOTION_NECKSHAKE,	
		MOTION_UNYO,	// 首うね
		MOTION_CANNOTPECK,
		MOTION_MULTIAPPEAR,
		MOTION_WINGPTPT,	// 羽根パタパタ
		MOTION_STOMACH,	// 腹ベタ
		MOTION_FALL,
		MOTION_MAX
	};
	// ペンギンスキン列挙
	enum SKIN
	{
		SKIN_PILOT = 0,		// パイロット
		SKIN_HEADBANG,		// ハチマキペンギン
		SKIN_PIRATE,		// 海賊
		SKIN_SWIMSUIT,		// 水着（いるん？）
		SKIN_NONE,			// 自然体
		SKIN_MAX
	};

	CNPCPenguin(int nPriority = 4);	// コンストラクタ
	~CNPCPenguin();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CNPCPenguin* Create(INPCPenguinState* pState = nullptr, SKIN skin = SKIN_NONE);	// 生成処理

	// 設定
	void SetState(INPCPenguinState* pState);

private:
	// メンバ関数
	void FollowIce(void);

	// メンバ変数
	CCollisionSphere *m_pCollisionSphere;	// 球の判定
	INPCPenguinState *m_pState;				// 動きステート
	CShadow *m_pShadow;						// 影のポインタ
};

//****************************************
// 動きステート
//****************************************
//----------------------------------------
// ペンギンの動きステート基底インターフェース
//----------------------------------------
__interface INPCPenguinState
{
	HRESULT Init(CNPCPenguin *pPenguin) = 0;
	void Uninit(void) = 0;
	void Update(CNPCPenguin* pPenguin) = 0;
};

#endif