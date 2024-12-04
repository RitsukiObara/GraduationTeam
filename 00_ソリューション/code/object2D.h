//*****************************************************
//
// 2Dオブジェクト[object2D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CObject2D	: public CGameObject
{
public:
	CObject2D(int nPriority = 6);	// コンストラクタ
	~CObject2D();	//	デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void Draw(void);	// 描画
private:
};

#endif