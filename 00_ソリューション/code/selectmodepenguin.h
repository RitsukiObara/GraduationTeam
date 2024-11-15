//*****************************************************
//
// モード選択ペンギン[selectmodepenguin.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _SELECTMODEPENGUIN_H_
#define _SELECTMODEPENGUIN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectModePenguin : public CMotion
{
public:
	CSelectModePenguin(int nPriority = 4);	// コンストラクタ
	~CSelectModePenguin();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CSelectModePenguin* Create(void);	// 生成処理

private:
	// メンバ関数
	void Debug(void);	// デバッグ処理

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	CCollisionSphere *m_pClsnSphere;	// 球の判定
};

#endif