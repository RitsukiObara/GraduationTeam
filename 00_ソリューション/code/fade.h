//*****************************************************
//
// フェード処理[fade.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "polygon2D.h"
#include "scene.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFade
{
public:
	typedef enum
	{//フェードの状態
		FADE_NONE = 0,				//何もしていない状態
		FADE_IN,					//フェードイン状態
		FADE_OUT,					//フェードアウト状態
		FADE_MAX
	}FADE;
	typedef enum
	{// モード
		MODE_NORMAL = 0,	// 通常
		MODE_WHITEOUT,	// ホワイトアウト
		MODE_MAX
	}MODE;

	CFade();	// コンストラクタ
	~CFade();	// デストラクタ
	
	static CFade *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CScene::MODE modeNext,bool bTrans = true);
	FADE GetState(void);
	void SetColor(D3DXCOLOR col) { m_col = col; }
	static CFade *GetInstance(void) { return m_pFade; }

private:
	bool m_bTrans;	// 遷移するかどうか
	FADE m_fade;	//フェードの状態
	CScene::MODE m_modeNext;	//次の画面
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXCOLOR m_col;	// 色
	static CFade *m_pFade;	// 自身のポインタ
};

#endif
