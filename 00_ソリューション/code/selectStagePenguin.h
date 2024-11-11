//*****************************************************
//
// ステージ選択ペンギン[slectStagePenguin.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _SELECTSTAGEPENGUIN_H_
#define _SELECTSTAGEPENGUIN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CInputManager;
class CCollisionSphere;

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectStagePenguin : public CMotion
{
public:
	CSelectStagePenguin(int nPriority = 4);	// コンストラクタ
	~CSelectStagePenguin();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CSelectStagePenguin *Create(void);	// 生成処理

private:
	// メンバ関数
	void Input(void);	// 入力
	void InputMove(void);	// 移動入力
	void Debug(void);	// デバッグ処理

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	CInputManager *m_pInputMgr;	// 入力マネージャー
	CCollisionSphere *m_pClsnSphere;	// 球の判定
};

#endif