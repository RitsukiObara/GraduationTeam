//*****************************************************
//
// 氷の処理[ice.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ICE_H_
#define _ICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CFan3D;
class CIceState;
class CObjectX;

//*****************************************************
// クラスの定義
//*****************************************************
class CIce : public CGameObject
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_FLOWS,	// 流れてる状態
		STATE_MAX
	};
	enum E_Type
	{// 種類
		TYPE_NORMAL = 0,	// 通常
		TYPE_HARD,	// 硬い氷
		TYPE_MAX
	};

	CIce(int nPriority = 2);	// コンストラクタ
	~CIce();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateMesh(void);	// メッシュの生成
	void DeleteMesh(void);	// メッシュの破棄
	void SetTransform(float fRadius);	// トランスフォーム設定
	virtual bool IsCanPeck(void) { return true; };	// 突っつけるかの判定を返す
	void ChangeState(CIceState *pState);

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// 探索可能フラグ
	bool IsCanFind(void) { return m_bCanFind; }
	virtual void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// 破壊フラグ
	bool IsBreak(void) { return m_bBreak; }
	void EnablePeck(bool bPeck) { m_bPeck = bPeck; }	// 突っつかれたフラグ
	bool IsPeck(void) { return m_bPeck; }
	void EnableAliveStandIce(bool bPeck) { m_bAliveStandBlock = bPeck; }	// 立っている氷に辿り着いたフラグ
	bool IsAliveStandIce(void) { return m_bAliveStandBlock; }
	void EnableSink(bool bSink) { m_bSink = bSink; }	// 沈むフラグ
	bool IsSink(void) { return m_bSink; }
	CFan3D *GetFan3D(void) { return m_pUp; }	// 上側の扇ポリゴン取得

	// 静的メンバ関数
	static CIce *Create(E_Type type = E_Type::TYPE_NORMAL, E_State state = E_State::STATE_FLOWS);	// 生成処理
	static int GetNumAll(void) { return s_nNumAll; }	// 総数取得
	static std::vector<CIce*> GetInstance(void) { return m_Vector; }

private:
	// メンバ関数
	void Flows(void);	// 流れる処理
	void FollowWave(void);	// 波に追従する処理

	// メンバ変数
	E_State m_state;	// 状態
	bool m_bCanFind;	// 探索できるフラグ
	bool m_bBreak;	// 壊れるフラグ
	bool m_bPeck;	// 突っつかれたフラグ
	bool m_bAliveStandBlock;	// 立っているブロックに到達したフラグ
	bool m_bSink;	// 沈むフラグ
	CFan3D *m_pUp;	// 上側に貼る扇ポリゴン
	CMeshCylinder *m_pSide;	// サイドのシリンダー
	CIceState *m_pState;	// ステイトのポインタ

	// 静的メンバ変数
	static int s_nNumAll;	// 総数
	static std::vector<CIce*> m_Vector;	// 自身のポインタ
};

class CIceState
{// 氷の基底ステイト
public:
	CIceState() {};	// コンストラクタ
	~CIceState() {};	// デストラクタ

	virtual void Init(CIce *pIce) = 0;	// 初期化
	virtual void Uninit(CIce *pIce) = 0;	// 終了
	virtual void Update(CIce *pIce) = 0;	// 更新

private:
};

class CIceStaeteNormal : public CIceState
{// 氷の通常ステイト
public:
	CIceStaeteNormal() {};	// コンストラクタ
	~CIceStaeteNormal() {};	// デストラクタ

	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新

private:
};

class CIceStaeteBreak : public CIceState
{// 氷の崩壊ステイト
public:
	CIceStaeteBreak() : m_fTimerRepair(0.0f) {};	// コンストラクタ
	~CIceStaeteBreak() {};	// デストラクタ

	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新

private:
	// 定数定義
	static const int NUM_ICE_BREAK = 5;	// 壊れた氷の数
	
	// メンバ変数
	vector<CObjectX*> m_aPeaceIce;	// 氷の破片のポイント
	float m_fTimerRepair;	// 修復タイマー
};

class CIceStateFlow : public CIceState
{// 氷の流れステイト
public:
	CIceStateFlow() {};	// コンストラクタ
	~CIceStateFlow() {};	// デストラクタ

	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新

private:
	// 関数ポインタ型の定義
	typedef void (CIceStateFlow::*DirectionFunc)(void);

	// メンバ関数
	void CollideIce(void);	// 氷との判定
	void CheckUp(void);	// 上方向の確認
	void CheckRight(void);	// 右側の確認
	void CheckDown(void);	// 下方向の確認
	void CheckLeft(void);	// 左側の確認
};

class CIceStaeteSink : public CIceState
{// 氷の沈むステイト
public:
	CIceStaeteSink() {};	// コンストラクタ
	~CIceStaeteSink() {};	// デストラクタ

	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新

private:
};

#endif

