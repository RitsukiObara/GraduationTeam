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
#include "object3D.h"
#include "ocean.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CFan3D;
class CIceState;
class CObjectX;
class CCollisionSphere;

//*****************************************************
// クラスの定義
//*****************************************************
class CIce : public CObject3D
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
		TYPE_HARD,			// 硬い氷
		TYPE_HARDMULTI,		// マルチ用の硬い氷
		TYPE_MAX
	};
	enum E_TypeShake
	{// 揺れの種類
		SHAKE_NONE = 0,	// 何もしていない状態
		SHAKE_SINK_NORMAL,	// 通常の沈み
		SHAKE_SINK_BIG,	// 大きな沈み
		SHAKE_RETURN,	// 通常戻り
		SHAKE_RETURN_BIG,	// 大きな戻り
		SHAKE_MAX
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
	bool IsOnTopAnyObject(void);	// 何かしらが乗ってる判定
	void SetColor(D3DXCOLOR col);	// 色の設定
	void StartFlash(void);			// 光る処理の開始
	void MoveObjectOnIce(D3DXVECTOR3 vecMove);	// 乗ってるオブジェクトを動かす
	void CreateCollision(void);	// 判定の生成
	void DeleteCollision(void);	// 判定の破棄

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// 探索可能フラグ
	bool IsCanFind(void) { return m_bCanFind; }
	virtual void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// 破壊フラグ
	bool IsBreak(void) { return m_bBreak; }
	void EnablePeck(bool bPeck) { m_bPeck = bPeck; }	// 突っつかれたフラグ
	bool IsPeck(void) { return m_bPeck; }
	void EnableSink(bool bSink) { m_bSink = bSink; }	// 沈むフラグ
	bool IsSink(void) { return m_bSink; }
	void EnableStop(bool bStop) { m_bStop = bStop; }	// 止まるフラグ
	bool IsStop(void) { return m_bStop; }
	CFan3D *GetFan3D(void) { return m_pUp; }				// 上側の扇ポリゴン取得
	CMeshCylinder *GetMeshCyliner(void) { return m_pSide; }	// 側面のメッシュシリンダー取得
	void SetRippleFrag(COcean::E_Stream stream, bool bValue) { m_abRipleFrag[stream] = bValue; }	// 流れごとのフラグ取得
	bool GetRippleFrag(COcean::E_Stream stream) { return m_abRipleFrag[stream]; }
	void SetHeightOcean(float fValue) { m_fHeightOcean = fValue; }									// 波からの高さ
	float GetHeightOcean(void) { return m_fHeightOcean; }
	void GetOnTopObject(vector<CGameObject*> &rVector, float fRate = 1.0f);	// 上に乗ってるものの検出
	void SetTimerStartMove(float fTime);									// 流れ始めタイマー設定
	float GetTimerStartMove(void);

	// 静的メンバ関数
	static CIce *Create(E_Type type = E_Type::TYPE_NORMAL, E_State state = E_State::STATE_FLOWS,int nIdxV = -1, int nIdxH = -1);	// 生成処理
	static int GetNumAll(void) { return s_nNumAll; }	// 総数取得
	static std::vector<CIce*> GetInstance(void) { return s_Vector; }
	static CIce *GetNearestIce(D3DXVECTOR3 pos);	// 最も近い氷の取得

private:
	// メンバ関数
	void FollowWave(void);		// 波に追従する処理
	void SearchOnThis(void);	// 自身に乗ってるものの検出
	void Tilt(void);			// 揺れの処理
	void Ripples(void);			// さざ波の処理
	void SmallRipples(void);	// 小さいさざ波の処理

	void FollowMesh(void);		// メッシュの追従
	void FollowCollision(void);	// 判定の追従
	void Flash(void);			// きらりと光る処理

	// メンバ変数
	E_State m_state;		// 状態
	bool m_bCanFind;		// 探索できるフラグ
	bool m_bBreak;			// 壊れるフラグ
	bool m_bPeck;			// 突っつかれたフラグ
	bool m_bSink;			// 沈むフラグ
	bool m_bStop;			// 停止しているかどうか
	float m_fHeightOcean;	// 海からの高さ

	D3DXVECTOR3 m_rotDest;	// 目標の向き

	CFan3D *m_pUp;					// 上側に貼る扇ポリゴン
	CMeshCylinder *m_pSide;			// サイドのシリンダー
	CCollisionSphere *m_pCollision;	// 当たり判定

	CIceState *m_pState;	// ステイトのポインタ

	bool m_abRipleFrag[COcean::E_Stream::STREAM_MAX];	// 流れごとのフラグ
	float m_fTimerRipples;								// さざ波のタイマー
	float m_fSpawnTimeRipples;							// さざ波の制限時間

	float m_fTimerFlash;		// 光のタイマー
	float m_fTimeStartFlash;	// 光の開始までの時間
	int m_nCntAnimFlash;		// 光のアニメーションカウンター
	int m_nPaternAnim;			// アニメーションパターン

	// 静的メンバ変数
	static int s_nNumAll;	// 総数
	static std::vector<CIce*> s_Vector;	// 自身のポインタ
};

class CIceState
{// 氷の基底ステイト
public:
	CIceState() {};	// コンストラクタ
	~CIceState() {};	// デストラクタ

	virtual void Init(CIce *pIce) = 0;		// 初期化
	virtual void Uninit(CIce *pIce) = 0;	// 終了
	virtual void Update(CIce *pIce) = 0;	// 更新

	virtual void SetTimerStartMove(float fTime) {};	// 動き始めのタイマー
	virtual float GetTimerStartMove(void) { return 0.0f; }

private:
};

class CIceStaeteNormal : public CIceState
{// 氷の通常ステイト
public:
	CIceStaeteNormal() : m_nIdxDriftV(0), m_nIdxDriftH(0) {};	// コンストラクタ
	~CIceStaeteNormal() {};	// デストラクタ

	// メンバ関数
	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新

private:
	// メンバ関数
	bool CheckFailGetIndex(CIce *pIce);	// 番号取得に失敗しているかの確認
	void MoveToGrid(CIce *pIce);	// グリッドの位置に向かって移動する処理

	// メンバ変数
	int m_nIdxDriftV;	// 漂着してるグリッドの縦番号
	int m_nIdxDriftH;	// 漂着してるグリッドの横番号
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
	
	// メンバ関数
	void UpdateRepair(CIce *pIce);	// 修復する更新
	void FollowIce(CIce *pIce);	// 氷の追従

	// メンバ変数
	vector<CObjectX*> m_aPeaceIce;	// 氷の破片のポイント
	float m_fTimerRepair;	// 修復タイマー
};

class CIceStateFlow : public CIceState
{// 氷の流れステイト
public:
	CIceStateFlow() : m_nIdxDriftV(0), m_nIdxDriftH(0), m_fTimerStartMove(0.0f){};	// コンストラクタ
	~CIceStateFlow() {};	// デストラクタ

	void Init(CIce *pIce) override;	// 初期化
	void Uninit(CIce *pIce) override;	// 終了
	void Update(CIce *pIce) override;	// 更新
	void SetTimerStartMove(float fTime) override { m_fTimerStartMove = fTime; }	// 流れ始めタイマー設定
	float GetTimerStartMove(void) override { return m_fTimerStartMove; }

private:
	// 関数ポインタ型の定義
	typedef bool (CIceStateFlow::*DirectionFunc)(CIce *pIce,int nIdxV,int nIdxH, vector<CIce*> &rpHitIce);

	// メンバ関数
	void UpdateSearchIce(CIce *pIce);	// 氷を探してる時の更新
	void UpdateDriftIce(CIce *pIce);	// 漂着する時の更新
	void CollideIce(CIce *pIce);	// 氷との判定
	bool CheckUp(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// 上方向の確認
	bool CheckRight(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// 右側の確認
	bool CheckDown(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// 下方向の確認
	bool CheckLeft(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// 左側の確認
	void CollideOtherFlow(CIce *pIce);	// 他の流氷との判定

	// メンバー変数
	int m_nIdxDriftV;			// 漂着するグリッドの縦番号
	int m_nIdxDriftH;			// 漂着するグリッドの横番号
	float m_fTimerStartMove;	// 移動開始タイマー
	D3DXVECTOR3 m_vecStream;	// 流れのベクトル
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

