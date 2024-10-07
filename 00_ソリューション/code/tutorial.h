//*****************************************************
//
// チュートリアル処理[tutorial.h]
// Author:藤田勇一
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CBlink2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorial : public CScene
{
public:
	CTutorial();	// コンストラクタ
	~CTutorial();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// メンバ関数
	void UpdateBlinkUI(void);	// 終了操作の点滅更新

	// メンバ変数
	CUI *m_pManual;			// 説明情報
	CBlink2D *m_pControl;	// 終了操作情報
	float m_fCurTime;		// 現在の待機時間
};

#endif