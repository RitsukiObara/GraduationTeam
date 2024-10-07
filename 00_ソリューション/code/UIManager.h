//*****************************************************
//
// UIマネージャー[UIManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// 定数定義
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CUIManager : public CObject
{
public:

	CUIManager();	// コンストラクタ
	~CUIManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ReleaseGameUI(void);	// ゲームUIの一括解放

	// 変数取得・設定関数
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

	// 静的メンバ関数
	static CUIManager *Create(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }

private:
	// メンバ変数
	float m_fCntFrame;	// フレームのカウンター
	bool m_bDisp;	// 表示するかどうか

	// 静的メンバ変数
	static CUIManager *m_pUIManager;	// 自身のポインタ
};

#endif
