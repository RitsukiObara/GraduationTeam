//*****************************************************
//
// 3Dフェードの処理[fade3D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _FADE3D_H_
#define _FADE3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラス定義
//*****************************************************
class CFade3D : public CObject
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

	CFade3D();	// コンストラクタ
	~CFade3D();	// デストラクタ

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

	// 静的メンバ関数
	static CFade3D *Create(CPolygon3D *pPolygon,float fTimeMax = 4.0f);
private:
	// メンバ変数
	E_State m_state;		// 状態
	float m_fTimer;			// タイマー
	float m_fTimeMax;		// 最大時間
	CPolygon3D *m_pPoygon;	// ポリゴンのポインタ
	bool m_bBouceIn;		// イン状態のバウンス
	bool m_bBouceOut;		// アウト状態のバウンス
	string m_pathNext;		// 次のテクスチャパス

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CFade3D::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
};

#endif