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
#include "UI.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;
class CIcon;

//*****************************************************
// クラスの定義
//*****************************************************
class CUIEnemy : public CObject
{// 敵表示UI
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
	void ManageIcon(void);	// アイコンの管理
	void Debug(void);	// デバッグ処理

	// メンバ変数
	vector<CIcon*> m_apIcon;	// アイコンの配列

	// 静的メンバ変数
	static CUIEnemy *s_pUIEnemy;	// 自身のポインタ
};

class CIcon : public CUI
{// 敵表示アイコン
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_FALL,	// 落下状態
		STATE_MAX
	};

	CIcon() : CUI(), m_state(E_State::STATE_NONE) {};	// コンストラクタ
	~CIcon() {};	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void) { CUI::Uninit(); };
	void Update(void);
	void Draw(void) { CUI::Draw(); };
	void StartFall(void);

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }

	// 静的メンバ関数
	static CIcon *Create(void);

private:
	// メンバ関数
	void UpdateFall(void);	// 落下時の更新

	// メンバ変数
	E_State m_state;	// 状態
	D3DXVECTOR3 m_move;	// 移動量
};

#endif
