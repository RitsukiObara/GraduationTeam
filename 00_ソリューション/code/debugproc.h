//==========================================================
//
// デバッグ表示処理 [debugproc.h]
// Author ��山桃也 
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

#include "main.h"

//**********************************************************
//マクロ定義
//**********************************************************
#define MAX_DEBUGSTRING	(2048)	//デバッグ表示の最大文字数

//**********************************************************
//クラスの定義
//**********************************************************
class CDebugProc
{
public:	// 誰でもアクセス可能

	CDebugProc();	// コンストラクタ
	~CDebugProc();	// デストラクタ

	static CDebugProc *Create(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const char *fmt, ...);
	static CDebugProc *GetInstance(void) { return m_pDebugProc; }

private:	// 自分だけがアクセス可能
	static LPD3DXFONT m_pFont;	//フォントへのポインタ
	char m_aStr[MAX_DEBUGSTRING];	//デバッグ表示用の文字列
	bool m_bDisp;		//デバッグ表示のON/OFF

	static CDebugProc *m_pDebugProc;	// 自身のポインタ
};

#endif