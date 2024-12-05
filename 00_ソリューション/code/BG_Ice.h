//*****************************************************
//
// 背景氷の処理[BG_Ice.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _BGICE_H_
#define _BGICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラス定義
//*****************************************************
class CBgIce : public CObjectX
{
public:
	CBgIce();	// コンストラクタ
	~CBgIce();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CBgIce* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type);
	static void Load(char* pPath);

private:
	// 列挙型定義
	enum E_State
	{//背景氷の状態
		STATE_FLOW = 0,
		STATE_SINK,
		STATE_MAX
	};

	// メンバ関数
	void Move(void);		// 移動処理
	void Flow(void);		// 海面に沿わせる処理
	void RippleCheck(void);	// 波紋を出すかのチェック
	void CollideIce(void);	// 足場氷との判定
	void StartSink(void);	// 沈み始める処理

	// メンバ変数
	E_State m_state;		// 状態分け変数
	float fSpeedGravity;	// 沈んでいく速度
	float m_fSpeed;			// 移動速度
	int m_nRippleCount;		// 波紋出現カウンタ
	bool m_bInscreen;		// 画面内に入ったかチェックする変数
	int m_nOutScreen;		//スクリーン外にいる時間
};

//*****************************************************
// 定数定義
//*****************************************************
namespace bgice
{
	const int BGICE_MODEL = 3;	// 背景の氷総数
	const int RIPPLE_DEFAULT = 150;	// 波紋出現の基本時間
	const int RIPPLE_DEGREE = 90;	// 波紋出現時間のぶれ幅
}

#endif
