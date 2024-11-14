//*****************************************************
//
// つっつき線の処理[peckLine.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PECKLINE_H_
#define _PECKLINE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// クラス定義
//*****************************************************
class CPeckLine : public CObject
{
public:
	CPeckLine();	// コンストラクタ
	~CPeckLine();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPeckLine *Create(void);

private:
	// メンバ関数
	
	// メンバ変数
};

#endif