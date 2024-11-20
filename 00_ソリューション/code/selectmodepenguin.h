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
		MOTION_UNYO,	// 首うね
		MOTION_CANNOTPECK,
		MOTION_MULTIAPPEAR,
		MOTION_WINGPTPT,	// 羽根パタパタ
		MOTION_STOMACH,	// 腹ベタ
		MOTION_FALL,
		MOTION_MAX
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
	void MotionState(void);	// モーション状態

private:

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	CCollisionSphere *m_pCollisionSphere;	// 球の判定
	ISelectModePenguinState* m_pState;	// 動きステート

	int nCntMove;	// 移動のカウント
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
	void Uninit(void) = 0;
	void Update(CSelectModePenguin* pPenguin) = 0;
};

//****************************************
//		↓↓↓↓↓ここからステート追加↓↓↓↓↓
//****************************************
//****************************************
// 待機ステート
//****************************************
class CSelectModePenguinState_Stand : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stand(){ m_nCounter = 0; }
	~CSelectModePenguinState_Stand(){}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 歩きステート
//****************************************
class CSelectModePenguinState_Move : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Move() { m_nCounter = 0; }
	~CSelectModePenguinState_Move() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 転びステート
//****************************************
class CSelectModePenguinState_Fall : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Fall() {}
	~CSelectModePenguinState_Fall() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// 首振りステート
//****************************************
class CSelectModePenguinState_ShakeHead : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_ShakeHead() { m_nCounter = 0; }
	~CSelectModePenguinState_ShakeHead() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 腹ベタステート
//****************************************
class CSelectModePenguinState_Stomach : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stomach() { m_nCounter = 0; }
	~CSelectModePenguinState_Stomach() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// うにょんステート
//****************************************
class CSelectModePenguinState_UNYO : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_UNYO() {}
	~CSelectModePenguinState_UNYO() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// 羽パタパタステート
//****************************************
class CSelectModePenguinState_WingPTPT : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_WingPTPT() {}
	~CSelectModePenguinState_WingPTPT() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Uninit(void);
	void Update(CSelectModePenguin* pPenguin);
};

#endif