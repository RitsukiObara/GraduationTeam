//*****************************************************
//
// テクスチャの管理[texture.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_TEX	(128)
#define MAX_STRING	(256)

namespace Texture
{
int GetIdx(const char *pFileName);	// テクスチャ番号の取得
LPDIRECT3DTEXTURE9 GetTexture(int nIdx);	// テクスチャの取得
}

//*****************************************************
// クラスの定義
//*****************************************************
class CTexture
{
public:
	CTexture();	// コンストラクタ
	~CTexture();	// デストラクタ
	
	static CTexture *Create(void);
	HRESULT Load(void);
	void Unload(void);
	int Regist(const char *pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	static int GetNumAll(void) { return m_nNumAll; }
	static CTexture *GetInstance(void) { return m_pTexture; }
	void ChangeAllToObara(void);	// 全てを小原にする

private:
	// メンバ関数

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];	// テクスチャの配列
	static int m_nNumAll;						// 総数
	char m_apFilename[MAX_TEX][MAX_STRING];		// ファイル名の配列
	static CTexture *m_pTexture;				// 自身のポインタ
};

#endif