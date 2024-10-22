//*****************************************************
//
// 背景氷の処理[BG_Ice.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _BG_ICE_H_
#define _BG_ICE_H_

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

	static CBgIce* Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

#endif
