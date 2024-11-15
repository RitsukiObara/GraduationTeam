//*****************************************************
//
// つっつき線の処理[peckLine.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PECKLINE_H_
#define _PECKLINE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CObject3D;
class COrbit;

//*****************************************************
// クラス定義
//*****************************************************
class CPeckLine : public CObject
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何もしていない状態
		STATE_MOVE,		// 移動状態
		STATE_MAX
	};

	CPeckLine();	// コンストラクタ
	~CPeckLine();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void StartMove(D3DXVECTOR3 posDest);	// 移動開始
	void EndMove(void);						// 移動終了

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }

	// 静的メンバ関数
	static CPeckLine *Create(CPlayer *pPlayer);

private:
	// メンバ関数
	void CreateObject(void);	// オブジェクトの生成
	void UpdateMove(void);		// 移動状態の更新
	void FollowOrbit(void);		// 軌跡の追従

	// メンバ変数
	CPlayer *m_pPlayer;		// 持ち主のプレイヤー
	CObject3D *m_pPosOrbit;	// 軌跡の先端用3Dオブジェクト
	D3DXVECTOR3 m_posDest;	// 目標位置
	COrbit *m_pOrbit;		// 軌跡
	float m_fTimer;			// タイマー
	float m_fTimerWait;		// 待機タイマー
	E_State m_state;		// 状態

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CPeckLine::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif