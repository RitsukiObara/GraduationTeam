//*****************************************************
//
// テクスチャの管理[texture.cpp]
// Author:��山桃也
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
CTexture *CTexture::m_pTexture = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTexture::CTexture()
{
	m_pTexture = this;

	ZeroMemory(&m_apFilename[0], sizeof(m_apFilename));
	ZeroMemory(&m_apTexture[0], sizeof(m_apTexture));
}

//=====================================================
// デストラクタ
//=====================================================
CTexture::~CTexture()
{
	m_pTexture = nullptr;
}

//=====================================================
// 生成処理
//=====================================================
CTexture *CTexture::Create(void)
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture;

		if (m_pTexture != nullptr)
		{
			m_pTexture->Load();
		}
	}

	return m_pTexture;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CTexture::Load(void)
{
	// ファイル名の設定
	char *apFileName[MAX_TEX] = 
	{
		"data\\TEXTURE\\BG\\sky01.jpg",
	};

	for ( int nCntTex = 0;nCntTex < MAX_TEX;nCntTex++)
	{
		if (apFileName[nCntTex] != nullptr)
		{// ファイル名があったらテクスチャの読込
			// デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

			// テクスチャの読込
			D3DXCreateTextureFromFile
			(pDevice, apFileName[nCntTex],&m_apTexture[nCntTex]);

			// ファイル名保存
			strcpy(m_apFilename[nCntTex], apFileName[nCntTex]);

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
		if (m_apTexture[nCntTex] != nullptr)
		{
			if (m_apTexture[nCntTex] != nullptr)
			{
				m_apTexture[nCntTex]->Release();
				m_apTexture[nCntTex] = nullptr;
				m_nNumAll--;
			}
		}
	}
}

//=====================================================
// 個別読込
//=====================================================
int CTexture::Regist(const char *pFileName)
{
	if (pFileName == nullptr)
	{
		return -1;
	}

	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (m_apFilename[nCntTex] != nullptr)
		{
			if (strcmp(&m_apFilename[nCntTex][0], &pFileName[0]) == 0)
			{// ファイル名が同じ場合、現在の番号を返す
 				return nCntTex;
			}
		}
	}

	// 新しくテクスチャを読み込む場合↓

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// テクスチャの読込
	if (D3DXCreateTextureFromFile
	(pDevice, pFileName, &m_apTexture[m_nNumAll]) == E_FAIL)
		return -1;

	// ファイル名の保存
	strcpy(m_apFilename[m_nNumAll], pFileName);

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
		return nullptr;
	}
	else
	{
		return m_apTexture[nIdx];
	}
}

//=====================================================
// 全てを小原にする
//=====================================================
void CTexture::ChangeAllToObara(void)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	LPDIRECT3DTEXTURE9 pTex;

	// テクスチャの読込
	if (D3DXCreateTextureFromFile
	(pDevice, "data\\TEXTURE\\MATERIAL\\obaraface.png", &pTex) == E_FAIL)
		return;

	for (int i = 0; i < m_nNumAll; i++)
	{
		if(m_apTexture[i] != nullptr)
			m_apTexture[i] = pTex;
	}
}

namespace Texture
{
int GetIdx(const char *pFileName)
{
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture == nullptr)
		return -1;

	int nIdx = pTexture->Regist(pFileName);

	return nIdx;
}

LPDIRECT3DTEXTURE9 GetTexture(int nIdx)
{// テクスチャ取得
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture == nullptr)
		return nullptr;

	return pTexture->GetAddress(nIdx);
}
}