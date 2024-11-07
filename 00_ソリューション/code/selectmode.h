//*****************************************************
//
// モード選択処理[selectmode.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CSelectMode : public CScene
{
public:
	enum MODE
	{
		MODE_SINGLE = 0,
		MODE_PARTY,
		//MODE_OPTION,
		MODE_MAX
	};

	CSelectMode();	// コンストラクタ
	~CSelectMode();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// メンバ関数
	void ChangeSelectMode(int move);	// モード移動処理

	// メンバ変数
	CUI *m_apModeUI[MODE_MAX];	// モードUI
	CUI *m_pManual;				// 説明情報
	float m_fCurTime;			// 現在の待機時間
	MODE m_selectMode;			// 選択しているモード
};

#endif