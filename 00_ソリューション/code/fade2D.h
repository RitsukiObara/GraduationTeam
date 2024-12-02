//*****************************************************
//
// 2Dフェードの処理[fade2D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _FADE2D_H_
#define _FADE2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon2D;

//*****************************************************
// クラス定義
//*****************************************************
class CFade2D : public CObject
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,		// イン状態
		STATE_OUT,		// アウト状態
		STATE_MAX
	};

	CFade2D();	// コンストラクタ
	~CFade2D();	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void InitByState(E_State state);	// 状態での初期化

	// 変数取得・設定関数
	void SetTimeMax(float fTime) { m_fTimeMax = fTime; }	// 最大時間
	float GetTime(void) { return m_fTimeMax; }
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void EnableBouceIn(bool bValue) { m_bBouceIn = bValue; }
	void EnableBouceOut(bool bValue) { m_bBouceOut = bValue; }
	void SetPathNext(string sValue) { m_pathNext = sValue; }	// 次のテクスチャパス
	void SetSizeNext(D3DXVECTOR2 size) { m_sizeNext = size; }	// 次のサイズ

	// 静的メンバ関数
	static CFade2D *Create(CPolygon2D *pPolygon,float fTimeMax = 4.0f);
private:
	// メンバ変数
	E_State m_state;		// 状態
	float m_fTimer;			// タイマー
	float m_fTimeMax;		// 最大時間
	CPolygon2D *m_pPoygon;	// ポリゴンのポインタ
	bool m_bBouceIn;		// イン状態のバウンス
	bool m_bBouceOut;		// アウト状態のバウンス
	string m_pathNext;		// 次のテクスチャパス
	D3DXVECTOR2 m_sizeNext;	// 次のサイズ

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CFade2D::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif