//*****************************************************
//
// Xファイルの処理[objectX.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "texture.h"
#include "model.h"

//====================================================
// コンストラクタ
//====================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_IdxModel = -1;
	m_fRadius = 0.0f;
	m_pModel = NULL;
}

//====================================================
// デストラクタ
//====================================================
CObjectX::~CObjectX()
{

}

//====================================================
//初期化処理
//====================================================
HRESULT CObjectX::Init(void)
{
	// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	return S_OK;
}

//====================================================
//終了処理
//====================================================
void CObjectX::Uninit(void)
{
	// 自身の破棄
	Release();
}

//====================================================
//更新処理
//====================================================
void CObjectX::Update(void)
{

}

//====================================================
// 描画処理
//====================================================
void CObjectX::Draw(void)
{
	if (m_pModel != NULL)
	{
		// マトリックスの設定
		SetMatrix();

		// レンダラーの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
		D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
		LPDIRECT3DTEXTURE9 pTexture;

		// 現在のマテリアル取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->dwNumMat; nCntMat++)
		{
			// マテリアル設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの取得
			pTexture = CManager::GetTexture()->GetAddress(m_pModel->pIdxTexture[nCntMat]);

			// テクスチャ設定
			pDevice->SetTexture(0, pTexture);

			// モデル（パーツ）描画
			m_pModel->pMesh->DrawSubset(nCntMat);
		}

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// マトリックス設定処理
//=====================================================
void CObjectX::SetMatrix(void)
{
	// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//変数宣言
	D3DXMATRIX mtxRot, mtxTrans;
	
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
//生成処理
//=====================================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{
		// インスタンス生成
		pObjectX = new CObjectX;

		pObjectX->m_pos = pos;
		pObjectX->m_rot = rot;

		// 初期化処理
		pObjectX->Init();
	}

	return pObjectX;
}

//=====================================================
// モデルの最長頂点取得
//=====================================================
void CObjectX::SetRadius(void)
{
	//計算用変数
	int nNumVtx;			//頂点数
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ
	float fLength = 0.0f;
	D3DXVECTOR3 vtxMin = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vtxMax = { 0.0f,0.0f,0.0f };

	//頂点数の取得
	nNumVtx = m_pModel->pMesh->GetNumVertices();

	//フォーマットサイズ入手
	dwSizeFVF = D3DXGetFVFVertexSize(m_pModel->pMesh->GetFVF());

	//頂点バッファのロック
	m_pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x > vtxMax.x)
		{//Xの最大値
			vtxMax.x = vtx.x;
		}
		else if (vtx.x < vtxMin.x)
		{//Xの最小値
			vtxMin.x = vtx.x;
		}

		if (vtx.z > vtxMax.z)
		{//Zの最大値
			vtxMax.z = vtx.z;
		}
		else if (vtx.z < vtxMin.z)
		{//Zの最小値
			vtxMin.z = vtx.z;
		}

		if (vtx.y > vtxMax.y)
		{//Yの最大値
			vtxMax.y = vtx.y;
		}
		else if (vtx.y < vtxMin.y)
		{//Yの最小値
			vtxMin.y = vtx.y;
		}

		if (D3DXVec3Length(&vtx) > fLength)
		{
			fLength = D3DXVec3Length(&vtx);
		}

		pVtxBuff += dwSizeFVF;
	}

	//最大・最小頂点設定
	m_vtxMax = vtxMax;
	m_vtxMin = vtxMin;

	//頂点バッファのアンロック
	m_pModel->pMesh->UnlockVertexBuffer();

	// 長さを代入
	m_fRadius = fLength;
}

//=====================================================
// モデルの割り当て
//=====================================================
void CObjectX::BindModel(int nIdx)
{
	// モデル割り当て
	m_pModel = CModel::GetModel(nIdx);

	// 半径設定
	SetRadius();
}
