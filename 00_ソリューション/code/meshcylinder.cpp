//*****************************************************
//
// メッシュシリンダーの処理[meshcylinder.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "debugproc.h"
#include "universal.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TEX_PATH		"data\\TEXTURE\\EFFECT\\boost.png"				// テクスチャファイル名

//=====================================================
// コンストラクタ
//=====================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject3D(nPriority)
{
	ZeroMemory(&m_meshCylinder, sizeof(m_meshCylinder));
	m_pIdxBuff = nullptr;
	m_pVtxBuff = nullptr;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=====================================================
// 生成処理
//=====================================================
CMeshCylinder *CMeshCylinder::Create(int nMeshU,int nMeshV,int nTexU,int nTexV)
{
	CMeshCylinder *pMeshCylinder = nullptr;

	if (pMeshCylinder == nullptr)
	{
		pMeshCylinder = new CMeshCylinder;

		if (pMeshCylinder != nullptr)
		{
			pMeshCylinder->m_meshCylinder.nMeshU = nMeshU;
			pMeshCylinder->m_meshCylinder.nMeshV = nMeshV;
			pMeshCylinder->m_meshCylinder.nTexU = nTexU;
			pMeshCylinder->m_meshCylinder.nTexV = nTexV;

			pMeshCylinder->Init();
		}
	}

	return pMeshCylinder;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeshCylinder::Init(void)
{
	m_meshCylinder.fRadius = meshCylinder::MESH_RADIUS;
	m_meshCylinder.fHeight = meshCylinder::MESH_HEIGHT;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	
	m_meshCylinder.nTexU = meshCylinder::SPLIT_TEX_U;
	m_meshCylinder.nTexV = meshCylinder::SPLIT_TEX_V;
	m_meshCylinder.fRadius = m_meshCylinder.fRadius;
	int nMeshU = m_meshCylinder.nMeshU;
	int nMeshV = m_meshCylinder.nMeshV;
	int nTexU = m_meshCylinder.nTexU;
	int nTexV = m_meshCylinder.nTexV;
	float fRadius = m_meshCylinder.fRadius;
	float fHeight = m_meshCylinder.fHeight;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nMeshU + 1) * (nMeshV + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((nMeshU + 1) * (nMeshV + 1) + ((nMeshV + 1) - 2) * (nMeshU + 1) + (((nMeshV + 1) - 2) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファへのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//最大頂点数計算
	m_meshCylinder.nNumVtx = (nMeshU + 1) * (nMeshV + 1);

	//計算用変数
	float fRot;
	D3DXVECTOR3 VecRot;

	//頂点情報==================================================================================
	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//頂点座標の設定
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//角度算出
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

			//テクスチャ座標
			pVtx[nCountV * (nMeshU + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)nTexU / nMeshU) * nCountU,
				((float)nTexV / nMeshV) * nCountV
			);

			VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z
			);

			//ベクトル正規化
			D3DXVec3Normalize(&VecRot, &VecRot);

			//法線ベクトルの設定
			pVtx[nCountV * (nMeshU + 1) + nCountU].nor = VecRot;
		}
	}

	for (int nCnt = 0; nCnt < m_meshCylinder.nNumVtx; nCnt++)
	{
		//頂点カラーの設定
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//インデックス==================================================================================
	m_meshCylinder.nNumIdx = (nMeshU + 1) * (nMeshV + 1) + ((nMeshV + 1) - 2) * (nMeshU + 1) + (((nMeshV + 1) - 2) * 2);

	for (int nCount = 0; nCount < m_meshCylinder.nNumIdx / 2; nCount++)
	{//インデックス決定
		if (
			nCount % ((nMeshU + 1) + (nMeshU + 2) * (nCount / (nMeshU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((nMeshU + 1) * 2) + (nMeshU + 1) * (nCount / (nMeshU + 2));

			pIdx[nCount * 2] = nMeshU + (nMeshU + 1) * (nCount / (nMeshU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (nMeshU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (nMeshU + 2))) + (nMeshU + 1);
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshCylinder::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{//頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{//インデックスバッファポインタの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshCylinder::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();
}

//=====================================================
// 頂点反映
//=====================================================
void CMeshCylinder::SetVtx(void)
{
	int nMeshU = m_meshCylinder.nMeshU;
	int nMeshV = m_meshCylinder.nMeshV;
	int nTexU = m_meshCylinder.nTexU;
	int nTexV = m_meshCylinder.nTexV;
	float fRadius = m_meshCylinder.fRadius;
	float fHeight = m_meshCylinder.fHeight;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//頂点座標の設定
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//角度算出
			float fRot = nCountU * (D3DX_PI / nMeshU) * 2;

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = (float)sin(fRot) * fRadius;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.y = (nMeshV - nCountV) * fHeight;
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * fRadius;

			//テクスチャ座標
			pVtx[nCountV * (nMeshU + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)nTexU / nMeshU) * nCountU,
				((float)nTexV / nMeshV) * nCountV
			);

			D3DXVECTOR3 VecRot = D3DXVECTOR3
			(
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x,
				0.0f,
				pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z
			);

			//ベクトル正規化
			D3DXVec3Normalize(&VecRot, &VecRot);

			//法線ベクトルの設定
			pVtx[nCountV * (nMeshU + 1) + nCountU].nor = VecRot;
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 色設定
//=====================================================
void CMeshCylinder::SetCol(D3DXCOLOR col)
{
	if (m_pVtxBuff == nullptr)
		return;

	m_col = col;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_meshCylinder.nNumVtx; nCnt++)
	{
		//頂点カラーの設定
		//pVtx[nCnt].col = m_col;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CMeshCylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 継承クラスの描画
	CObject3D::Draw();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ取得
	CTexture *pTextureManager = CTexture::GetInstance();

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (pTextureManager != nullptr)
	{
		pTexture = pTextureManager->GetAddress(m_nIdxTexture);
	}

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_meshCylinder.nNumVtx,								//頂点数
		0,													//最初のインデックス
		m_meshCylinder.nNumIdx - 2							//ポリゴン数
	);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// 描画のみ行う
//=====================================================
void CMeshCylinder::JustDraw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ取得
	CTexture *pTextureManager = CTexture::GetInstance();

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (pTextureManager != nullptr)
	{
		pTexture = pTextureManager->GetAddress(m_nIdxTexture);
	}

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_meshCylinder.nNumVtx,								//頂点数
		0,													//最初のインデックス
		m_meshCylinder.nNumIdx - 2							//ポリゴン数
	);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}