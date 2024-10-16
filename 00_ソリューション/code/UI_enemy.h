//*****************************************************
//
// 敵数表示UI[UI_enemy.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIENEMY_H_
#define _UIENEMY_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CUIEnemy : public CObject
{
public:

	CUIEnemy();	// コンストラクタ
	~CUIEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 変数取得・設定関数

	// 静的メンバ関数
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return m_pUIEnemy; }

private:
	// メンバ関数
	void ManageNumber(void);	// 数字の管理

	// メンバ変数
	CNumber *m_pNumber;	// 数字のポインタ
	CUI *m_pIcon;	// アイコンのポインタ

	// 静的メンバ変数
	static CUIEnemy *m_pUIEnemy;	// 自身のポインタ
};

#endif
