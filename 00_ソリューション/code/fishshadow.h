//*****************************************************
//
// 魚影の処理[fishshadow.h]
// Author:若木一真
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

#include "enemy.h"

//*****************************************************
// クラス定義
//*****************************************************
class CFishShadow : public CPolygon3D
{
public:
	CFishShadow();	// コンストラクタ
	~CFishShadow();	// デストラクタ

	static CFishShadow* Create(CEnemy::TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif