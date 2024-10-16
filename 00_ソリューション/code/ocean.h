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
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	float m_fRot;
};
#endif