//*****************************************************
//
// テクスチャーの処理[texture.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CTexture::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CTexture::CTexture()
{
	ZeroMemory(&m_apFilename[0], sizeof(m_apFilename));
	ZeroMemory(&m_apTexture[0], sizeof(m_apTexture));
}

//=====================================================
// デストラクタ
//=====================================================
CTexture::~CTexture()
{

}

//=====================================================
// 読込処理
//=====================================================
HRESULT CTexture::Load(void)
{
	// ファイル名の設定
	char *apFileName[MAX_TEX] = 
	{
		"data\\TEXTURE\\CHARACTER\\enemy000.png"
	};

	for ( int nCntTex = 0;nCntTex < MAX_TEX;nCntTex++)
	{
		if (apFileName[nCntTex] != NULL)
		{// ファイル名があったらテクスチャの読込
			// レンダラーの取得
			LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

			// テクスチャの読込
			D3DXCreateTextureFromFile
			(pDevice, apFileName[nCntTex],&m_apTexture[nCntTex]);

			// ファイル名保存
			m_apFilename[nCntTex] = apFileName[nCntTex];

			// 総数カウントアップ
			m_nNumAll++;
		}
	}

	return S_OK;
}

//=====================================================
// 全破棄処理
//=====================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			if (m_apTexture[nCntTex] != NULL)
			{
				m_apTexture[nCntTex]->Release();
				m_apTexture[nCntTex] = NULL;
			}
		}
	}
}

//=====================================================
// 個別読込
//=====================================================
int CTexture::Regist(const char *pFileName)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (m_apFilename[nCntTex] != NULL)
		{
			if (strcmp(&m_apFilename[nCntTex][0], &pFileName[0]) == 0)
			{// ファイル名が同じ場合、現在の番号を返す
				return nCntTex;
			}
		}
	}

	// 新しくテクスチャを読み込む場合↓

	// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile
	(pDevice, pFileName, &m_apTexture[m_nNumAll]);

	// ファイル名の保存
	m_apFilename[m_nNumAll] = (char*)pFileName;

	// 現在の番号を保存
	int nIdx = m_nNumAll;

	// 総数カウントアップ
	m_nNumAll++;

	return nIdx;
}

//=====================================================
// アドレス取得処理
//=====================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx == -1)
	{
		return NULL;
	}
	else
	{
		return m_apTexture[nIdx];
	}
}