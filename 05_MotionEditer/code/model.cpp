//*****************************************************
//
// モデルの処理[model.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <stdio.h>

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
char CModel::m_aPath[NUM_MODEL][256] = {};
CModel::Model *CModel::m_apModel[NUM_MODEL] = {};

//=====================================================
// コンストラクタ
//=====================================================
CModel::CModel()
{

}

//=====================================================
// デストラクタ
//=====================================================
CModel::~CModel()
{

}

//=====================================================
// 全初期化処理
//=====================================================
void CModel::Init(int nIdx)
{
	
}

//=====================================================
// 読込処理
//=====================================================
int CModel::Load(char *pPath)
{
	int nIdxModel = -1;

	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{// 既に同じモデルがあるかの確認(パスで確認)
		if (strcmp(&m_aPath[nCntModel][0], pPath)  == 0)
		{
			return nCntModel;
		}
	}

	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] == NULL)
		{
			// レンダラーの取得
			LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
			D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

			m_apModel[nCntModel] = new Model;

			// モデル情報の初期化
			ZeroMemory(m_apModel[nCntModel],sizeof(Model));

			// Xファイル読み込み
			D3DXLoadMeshFromX(pPath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apModel[nCntModel]->pBuffMat,
				NULL,
				&m_apModel[nCntModel]->dwNumMat,
				&m_apModel[nCntModel]->pMesh);

			// 必要数分テクスチャのポインタを確保する
			m_apModel[nCntModel]->pIdxTexture = new int[m_apModel[nCntModel]->dwNumMat];

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)m_apModel[nCntModel]->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_apModel[nCntModel]->dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// テクスチャが存在する
					// テクスチャ番号取得
					m_apModel[nCntModel]->pIdxTexture[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
				}
				else
				{
					m_apModel[nCntModel]->pIdxTexture[nCntMat] = -1;
				}
			}

			// 番号保存
			nIdxModel = nCntModel;

			// パス保存
			strcpy(&m_aPath[nCntModel][0], pPath);

			break;
		}
	}

	return nIdxModel;
}

//=====================================================
// 全破棄処理
//=====================================================
void CModel:: Unload(void)
{
	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			if (m_apModel[nCntModel]->pMesh != NULL)
			{//メッシュポインタの破棄
				m_apModel[nCntModel]->pMesh->Release();
				m_apModel[nCntModel]->pMesh = NULL;
			}

			if (m_apModel[nCntModel]->pBuffMat != NULL)
			{//頂点バッファポインタの破棄
				m_apModel[nCntModel]->pBuffMat->Release();
				m_apModel[nCntModel]->pBuffMat = NULL;
			}
			
			if (m_apModel[nCntModel]->pIdxTexture != NULL)
			{//テクスチャポインタの破棄
				delete m_apModel[nCntModel]->pIdxTexture;
				m_apModel[nCntModel]->pIdxTexture = NULL;
			}

			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}
}