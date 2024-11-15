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
	// 構造体定義
	struct S_InfoLine
	{// 線の構造体
		CObject3D *pPosOrbit;	// 軌跡の先端用3Dオブジェクト
		D3DXVECTOR3 posInit;	// 初期位置
		D3DXVECTOR3 posDest;	// 目標位置
		COrbit *pOrbit;			// 軌跡
		float fTimer;			// タイマー

		S_InfoLine() : pPosOrbit(nullptr),posInit() ,posDest(), pOrbit(nullptr), fTimer(0.0f) {};	// コンストラクタ
	};

	// メンバ関数
	void CreateLine(void);	// オブジェクトの生成
	void UpdateMove(void);		// 移動状態の更新
	void UpdateAllLine(void);	// 全ての線の更新

	// メンバ変数
	CPlayer *m_pPlayer;					// プレイヤーのポインタ
	vector<S_InfoLine*> m_aInfoLine;	// 線の配列
	float m_fTimerWait;					// 待機タイマー
	E_State m_state;					// 状態
	D3DXVECTOR3 m_posDest;				// 目標位置

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CPeckLine::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif