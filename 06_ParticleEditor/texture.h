//*****************************************************
//
// テクスチャーの処理[texture.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_TEX	(56)
#define MAX_STRING	(256)

//*****************************************************
// クラスの定義
//*****************************************************
class CTexture
{
public:
	CTexture();	// コンストラクタ
	~CTexture();	// デストラクタ

	HRESULT Load(void);
	void Unload(void);
	int Regist(const char *pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];	// テクスチャの配列
	static int m_nNumAll;	// 総数
	char *m_apFilename[MAX_TEX];	// ファイル名の配列
};

#endif