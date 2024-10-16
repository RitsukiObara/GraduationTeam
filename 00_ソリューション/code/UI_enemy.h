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
	void AddEnemy(void);	// 敵の追加
	void DeleteEnemy(void);	// 敵の削除

	// 変数取得・設定関数

	// 静的メンバ関数
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return s_pUIEnemy; }

private:
	// メンバ関数
	void Debug(void);	// デバッグ処理

	// メンバ変数
	vector<CUI*> m_apIcon;	// アイコンの配列

	// 静的メンバ変数
	static CUIEnemy *s_pUIEnemy;	// 自身のポインタ
};

#endif
