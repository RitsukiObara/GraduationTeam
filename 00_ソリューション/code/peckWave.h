//*****************************************************
//
// つっつき波紋の処理[peckWave.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PECKWAVE_H_
#define _PECKWAVE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// クラス定義
//*****************************************************
class CPeckWave : public CPolygon3D
{
public:
	CPeckWave(int nPriority = 5);	// コンストラクタ
	~CPeckWave();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPeckWave *Create(D3DXVECTOR3 pos,D3DXCOLOR col);

private:
};

#endif