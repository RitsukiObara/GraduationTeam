//*****************************************************
//
// 雪処理[snow.h]
// Author:早川友貴
//
//*****************************************************

#ifndef _SNOW_H_
#define _SNOW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSnow : public CPolygon3D
{
public:
	CSnow(int nPriority = 4);	// コンストラクタ
	~CSnow();	// デストラクタ

	static CSnow* Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Move(void);

private:
	int m_nLife;	// 寿命
	D3DXVECTOR3 m_move;
};

#endif