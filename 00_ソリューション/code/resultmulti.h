//*****************************************************
//
// マルチモードのリザルト処理[resultmulti.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _RESULT_MULTI_H_
#define _RESULT_MULTI_H_

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
class CResultMulti : public CScene
{
public:
	CResultMulti();		// コンストラクタ
	~CResultMulti();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// メンバ関数
	void SetUI(CUI** pUI, string path, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void InitNormal(int nIdxWinner);	// 通常の初期化
	void InitDraw(void);	// 引き分けの初期化

	// メンバ変数
	CUI *m_pWinnerNum;		// 勝者番号UI
	CUI *m_pWinnerText;		// 勝者テキストUI
	bool m_bDraw;			// 引き分けフラグ
};

#endif