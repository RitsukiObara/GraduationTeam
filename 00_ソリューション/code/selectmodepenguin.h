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
__interface ISelectModePenguinState;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// モード選択画面の遊ぶペンギンオブジェクトクラス
//****************************************
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
	static CSelectModePenguin* Create(ISelectModePenguinState* pState = nullptr);	// 生成処理

	// 設定
	void SetState(ISelectModePenguinState* pState);

private:
	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	CCollisionSphere *m_pCollisionSphere;	// 球の判定
	ISelectModePenguinState* m_pState;	// 動きステート
};

//****************************************
// 動きステート
//****************************************
//----------------------------------------
// ペンギンの動きステート基底インターフェース
//----------------------------------------
__interface ISelectModePenguinState
{
	HRESULT Init(CSelectModePenguin *pPenguin) = 0;
	void Update(CSelectModePenguin* pPenguin) = 0;
};

//****************************************
//		↓↓↓↓↓ここからステート追加↓↓↓↓↓
//****************************************
//****************************************
// 立ってるだけステート
//****************************************
class CSelectModePenguinState_Stand : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stand(){}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

#endif