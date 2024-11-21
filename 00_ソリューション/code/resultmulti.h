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
	CResultMulti();	// コンストラクタ
	~CResultMulti();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// メンバ関数

	// メンバ変数
	CUI *m_pWinner;		// 勝者UI
};

#endif