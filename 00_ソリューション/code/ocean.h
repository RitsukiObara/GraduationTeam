//*****************************************************
//
// オーシャンの処理[ocean.h]
// Author:早川友貴
//
//*****************************************************

#ifndef _OCEAN_H_
#define _OCEAN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshfield.h"

//*****************************************************
// クラス定義
//*****************************************************
class COcean : public CMeshField
{
public:
	COcean();	// コンストラクタ
	~COcean();	// デストラクタ

	static COcean* Create(void);
	static COcean* GetInstance(void) { return m_pOcean; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	void OceanRotState(void); // 海流の向きとメッシュの向きを連動させる処理

	float m_fSpeed;	// 海流のスピード
	D3DXVECTOR3 m_fRot;	// 海流の向き
	static COcean* m_pOcean;	// 自身のポインタ

};
#endif