//*****************************************************
//
// エフェクト処理[effect.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEffect2D : public CPolygon2D
{
public:
	CEffect2D(int nPriority = 7);	// コンストラクタ
	~CEffect2D();	// デストラクタ

	static CEffect2D *Create(D3DXVECTOR3 pos,  float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;	// 寿命
	float m_fDecrease;	// 減少量
};

#endif