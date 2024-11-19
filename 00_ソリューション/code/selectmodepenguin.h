//*****************************************************
//
// モード選択ペンギン[selectmodepenguin.h]
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
__interface ISelectModePenguinState;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// モード選択画面の遊ぶペンギンオブジェクトクラス
//****************************************
class CSelectModePenguin : public CMotion
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
		MOTION_UNYO,
		MOTION_CANNOTPECK,
		MOTION_MAX
	};

	// 構造体定義
	struct S_FragMotion
	{// モーションフラグの構造体
		bool bWalk;		// 歩行
		bool bPeck;		// 突っつき
		bool bJump;		// ジャンプ
	};

	CSelectModePenguin(int nPriority = 4);	// コンストラクタ
	~CSelectModePenguin();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CSelectModePenguin* Create(ISelectModePenguinState* pState = nullptr);	// 生成処理

	// 設定
	void SetState(ISelectModePenguinState* pState);
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// 入力可能フラグ

private:
	void ManageMotion(void);			// モーションの管理
	void EndJump(void);				// ジャンプの終了

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	CCollisionSphere *m_pCollisionSphere;	// 球の判定
	ISelectModePenguinState* m_pState;	// 動きステート
	S_FragMotion m_fragMotion;	// モーションフラグ
	bool m_bEnableInput;	// 入力可能フラグ
};

//****************************************
// 動きステート
//****************************************
//----------------------------------------
// ペンギンの動きステート基底インターフェース
//----------------------------------------
__interface ISelectModePenguinState
{
	HRESULT Init(CSelectModePenguin *pPenguin) = 0;
	void Update(CSelectModePenguin* pPenguin) = 0;
};

//****************************************
//		↓↓↓↓↓ここからステート追加↓↓↓↓↓
//****************************************
//****************************************
// 立ってるだけステート
//****************************************
class CSelectModePenguinState_Stand : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stand(){}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

#endif