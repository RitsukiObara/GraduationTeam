//*****************************************************
//
// 地面の処理[field.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CField : public CObject3D
{
public:
	CField(); // コンストラクタ
	~CField();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField *Create(D3DXVECTOR2 tex = D3DXVECTOR2{ 1.0f,1.0f }, D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, float width = 3000.0f, float height = 3000.0f);

private:
};

#endif