//*****************************************************
//
// 影の処理[shadow.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CShadow : public CPolygon3D
{
public:
	CShadow(int nPriority = 4);	// コンストラクタ
	~CShadow() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 静的メンバ関数
	static CShadow *Create(void);	// 生成処理

private:
	// メンバ関数
	bool CollideIce(void);		// 氷との判定
	void CollideOcean(void);	// 海との判定
};

#endif