//*****************************************************
//
// メッシュフィールドの処理[meshfield.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"
#include "iceManager.h"
#include "ocean.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* TEX_FILE = "data\\TEXTURE\\BG\\map_sea.png";	// テクスチャのファイル
const float LENGTH = 300.0f;				// メッシュの一辺の長さ
const int MESH_U = 254;						// 横のブロック数
const int MESH_V = 254;						// 縦のブロック数
const int SPLIT_TEX = 4;					// テクスチャ分割数
const float ANGLE_SLIP = 0.7f;				// 坂を滑る角度

// 海流関連
const float OCEAN_SPEED = 80.0f;			// 海流の速度
const float OCEAN_SPEED_UP = 0.01875f;		// 波の速度増加量（構造変更前0.3fの16分の1）
const float OCEAN_SPEED_DOWN = 0.03125f;	// 波の速度減少量（構造変更前0.5fの16分の1）
const float OCEAN_SPEED_MAX = 5.0f;			// 波の速度最大値（構造変更前80.0fの16分の1）
const float OCEAN_SPEED_MULTIPLY = 5.0f;	// 氷の流れる速度->波の速度にする際の速度倍率

const float LIMIT_SCROLL = 100.0f;	// スクロールの限界
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CMeshField *CMeshField::m_pMeshField = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CMeshField::CMeshField(int nPriority) : CObject3D(nPriority)
{
	ZeroMemory(&m_MeshField, sizeof(m_MeshField));
	m_pIdxBuff = nullptr;
	m_nIdxTexture = -1;
	m_nIdxTextureOverRay = -1;
	m_pVtxBuff = nullptr;
	m_fLengthMesh = 0.0f;
	m_nDivNumU = 0;
	m_nDivNumV = 0;
	m_fOceanSpeed = 0.0f;
	m_fAcceleOcean = 0.0f;
	m_fMaxSpeed = 0.0f;
	m_eOcean_Speed_State = OCEAN_STATE_NONE;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CMeshField::~CMeshField()
{

}

//=====================================================
// 生成処理
//=====================================================
CMeshField *CMeshField::Create(void)
{
	if (m_pMeshField == nullptr)
	{
		m_pMeshField = new CMeshField;

		if (m_pMeshField != nullptr)
		{
			m_pMeshField->Init();
		}
	}

	return m_pMeshField;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeshField::Init(void)
{
	CObject3D::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 値の初期設定
	m_fLengthMesh = LENGTH;
	m_nDivNumU = MESH_U;
	m_nDivNumV = MESH_V;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_fOceanSpeed = OCEAN_SPEED;
	m_fMaxSpeed = OCEAN_SPEED_MAX;
	m_fAcceleOcean = OCEAN_SPEED_UP;

	// 読込処理
	Load("data\\BYNARY\\field00.bin");

	m_nDivTex = SPLIT_TEX;

	// テクスチャの読込
	m_nIdxTexture = CTexture::GetInstance()->Regist(TEX_FILE);

	// 頂点数計算
	m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

	if (m_pVtxBuff == nullptr)
	{
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	int nCntVtx = 0;

	for (nCountV = 0;nCountV < m_nDivNumV + 1; nCountV++)
	{// 頂点座標の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			int nIdxVtx = nCountV * (m_nDivNumU + 1) + nCountU;

			// 頂点座標
			pVtx[nIdxVtx].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nIdxVtx].pos.z = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;

			// テクスチャ座標
			pVtx[nIdxVtx].tex = D3DXVECTOR2
			(
			    ((float)m_nDivTex / (float)m_nDivNumU) * nCountU,
				((float)m_nDivTex / (float)m_nDivNumU) * nCountV
			);

			pVtx[nIdxVtx].tex2 = pVtx[nIdxVtx].tex;
		}
	}

	for (int nCnt = 0;nCnt < m_MeshField.nNumVtx;nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// 法線の設定
	SetNormal();

	m_MeshField.nNumIdx = (m_nDivNumU + 1) * (m_nDivNumV + 1) + ((m_nDivNumV + 1) - 2) * (m_nDivNumU + 1) + (((m_nDivNumV + 1) - 2) * 2);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshField.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	// インデックスバッファへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCount = 0; nCount < m_MeshField.nNumIdx / 2; nCount++)
	{// インデックス決定
		if (
			nCount % ((m_nDivNumU + 1) + (m_nDivNumU + 2) * (nCount / (m_nDivNumU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((m_nDivNumU + 1) * 2) + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = m_nDivNumU + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (m_nDivNumU + 2))) + (m_nDivNumU + 1);
		}
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	//EnableWire(true);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshField::Uninit(void)
{
	m_pMeshField = nullptr;

	if (m_pVtxBuff != nullptr)
	{// 頂点バッファポインタの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// インデックスバッファポインタの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshField::Update(void)
{
	//Edit();
}

//=====================================================
// エディットの処理
//=====================================================
void CMeshField::Edit(void)
{

}

//=====================================================
// 法線の設定
//=====================================================
void CMeshField::SetNormal(void)
{
	D3DXVECTOR3 vec1, vec2, nor1, nor2, nor3, nor4, nor5, nor6, nor7;
	int nCountU, nCountV, nCntVtx = 0;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	for (nCountV = 0; nCountV < m_nDivNumV + 1; nCountV++)
	{// 法線の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++, nCntVtx++)
		{
			if (nCntVtx == 0)
			{// 最初の場合================================================
			 // 下と右の法線を求め、足して割る
				// 左下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_MeshField.nNumVtx - 1)
			{// 最後の場合================================================
			 // 左と上の法線を求め、足して割る
			 // 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx - 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 2].pos - pVtx[nCntVtx - m_nDivNumU - 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2) * 0.5f;
			}
			else if (nCntVtx == m_nDivNumU + 1)
			{// 右上の面一つ================================================
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 法線を求める
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx == m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// 左下の面一つ================================================
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 法線を求める
				pVtx[nCntVtx].nor = nor1;
			}
			else if (nCntVtx < m_nDivNumU)
			{// 上の辺で面が三つ================================================
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor1, &vec1, &vec2);

				D3DXVec3Normalize(&nor1, &nor1);

				// 右の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor2, &vec1, &vec2);

				D3DXVec3Normalize(&nor2, &nor2);

				// 隣の法線をもらう
				nor3 = pVtx[nCntVtx - 1].nor;

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else if (nCntVtx % m_nDivNumU + 1 == 0)
			{// 左の辺で面が三つ================================================
			 // 上の法線をもらう
				nor1 = pVtx[nCntVtx - m_nDivNumU - 1].nor;
				// 右の法線は上と同じ
				nor2 = nor1;

				// 下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor3, &vec1, &vec2);

				D3DXVec3Normalize(&nor3, &nor3);

				// 右下の法線を求める
				nor4 = (nor3 + nor2) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4) / 4;
			}
			else if (nCntVtx == m_nDivNumU + nCountV * (m_nDivNumU + 1))
			{// 右の辺で面三つ================================================
			 // 左上の法線をもらう
				nor1 = pVtx[nCntVtx - m_nDivNumU * nCountV - 2].nor;
				// 上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU * nCountV - 1].nor;
				// 左の法線をもらう
				nor3 = pVtx[nCntVtx - 1].nor;

				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - 1].pos - pVtx[nCntVtx + m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				// 下の法線を求める
				nor6 = (nor4 + nor5) / 2;

				// 法線を求める
				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor6) / 4;
			}
			else if (nCntVtx > m_nDivNumU * m_nDivNumV + m_nDivNumU)
			{// 下の辺で面三つ================================================
			 // 左の法線をもらう
				nor1 = pVtx[nCntVtx - 1].nor;
				// 左上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;

				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor3 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3) / 3;
			}
			else
			{// 面が六つの場合================================================
			 // 左の法線をもらう
				nor1 = pVtx[nCntVtx - 1].nor;
				// 左上の法線をもらう
				nor2 = pVtx[nCntVtx - m_nDivNumU - 2].nor;
				// 上の法線をもらう
				nor3 = pVtx[nCntVtx - m_nDivNumU - 1].nor;

				// 右上の四角形の法線を求める
				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;
				vec2 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx - m_nDivNumU].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx - m_nDivNumU - 1].pos - pVtx[nCntVtx].pos;
				vec2 = pVtx[nCntVtx + 1].pos - pVtx[nCntVtx].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor6 = (nor5 + nor4) / 2;

				// 左下の四角形の法線を求める
				// 右上の法線を求める
				vec1 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + 1].pos;
				vec2 = pVtx[nCntVtx].pos - pVtx[nCntVtx + 1].pos;

				D3DXVec3Cross(&nor4, &vec1, &vec2);

				D3DXVec3Normalize(&nor4, &nor4);

				// 左下の法線を求める
				vec1 = pVtx[nCntVtx].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;
				vec2 = pVtx[nCntVtx + m_nDivNumU + 2].pos - pVtx[nCntVtx + m_nDivNumU + 1].pos;

				D3DXVec3Cross(&nor5, &vec1, &vec2);

				D3DXVec3Normalize(&nor5, &nor5);

				nor7 = (nor4 + nor5) / 2;

				pVtx[nCntVtx].nor = (nor1 + nor2 + nor3 + nor4 + nor5 + nor7) / 6;
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 法線のリセット処理
//=====================================================
void CMeshField::ResetNormal(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点法線の設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 高さの取得
//=====================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor)
{
	if (m_pVtxBuff == nullptr)
	{
		return pos.y;
	}

	// 変数宣言
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, nor, vec6;
	D3DXVECTOR3 vecDiff;
	int nRu, nRd, nLu, nLd;
	float fHeight = pos.y;
	int nCntV, nCntU;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nCntU = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntV = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	// 頂点番号計算
	nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
	nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
	nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
	nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU)+nCntU;

	if (pos.x < -m_fLengthMesh * m_nDivNumU / 2 || 
		pos.x > m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z < -m_fLengthMesh * m_nDivNumU / 2 ||
		pos.z > m_fLengthMesh * m_nDivNumU / 2)
	{
		return 0.0f;
	}

	// 差分を取得
	vecDiff = pVtx[nLd].pos - pos;

	// 左上へのベクトル算出
	vec1 = pVtx[nLu].pos - pVtx[nLd].pos;
	// 右下へのベクトル算出
	vec2 = pVtx[nRd].pos - pVtx[nLd].pos;

	// 右下へのベクトル算出
	vec3 = pVtx[nRd].pos - pVtx[nRu].pos;
	// 左上へのベクトル算出
	vec4 = pVtx[nLu].pos - pVtx[nRu].pos;

	// 中心のベクトル算出
	vec5 = pVtx[nLu].pos - pVtx[nRd].pos;

	if ((vec5.z * (pos.x - pVtx[nLu].pos.x)) - (vec5.x * (pos.z - pVtx[nLu].pos.z)) < 0)
	{// 左下のポリゴンの上にいる
	 // 法線の算出
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// 法線正規化
		D3DXVec3Normalize(&nor, &nor);

		// 内積計算
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nLd].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nLd].pos.z) * nor.z };

		if (nor.y != 0)
		{// 高さの取得
			fHeight = (vec.x + vec.z + pVtx[nLd].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}
	else
	{// 右上のポリゴンの上にいる
		// 法線の算出
		D3DXVec3Cross(&nor, &vec3, &vec4);

		// 法線正規化
		D3DXVec3Normalize(&nor, &nor);

		// 内積計算
		D3DXVECTOR3 vec = { -(pos.x - pVtx[nRu].pos.x) * nor.x,0.0f,-(pos.z - pVtx[nRu].pos.z) * nor.z };

		if (nor.y != 0)
		{// 高さの取得
			fHeight = (vec.x + vec.z + pVtx[nRu].pos.y * nor.y) / nor.y;

			if (pNor != nullptr)
			{
				*pNor = nor;
			}
		}
	}

	if (pMove != nullptr)
	{
		if (nor.y * nor.y < ANGLE_SLIP * ANGLE_SLIP)
		{
			*pMove += nor * 0.5f;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=====================================================
// へこむ処理
//=====================================================
void CMeshField::Dent(D3DXVECTOR3 pos,float fRadius,float fDepth)
{
	// 計算用変数
	D3DXVECTOR3 vecDiff;
	float fLength;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 差分の長さを取得
		vecDiff = pos - (pVtx[nCnt].pos + GetPosition());
		fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

		if (fLength < fRadius)
		{
			float fRate = fLength / fRadius * D3DX_PI;

			fRate = cosf(fRate);

			pVtx[nCnt].pos.y += fDepth - (fDepth * (fLength / fRadius));

			// 頂点位置にエフェクトを出すよ
			CEffect3D::Create(pVtx[nCnt].pos + GetPosition(), 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// レイで当たっているかのチェック
//=====================================================
bool CMeshField::RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn)
{
	int nCntV, nCntU;
	int nCntVStart, nCntUStart;
	int nCntVEnd, nCntUEnd;
	int nLd, nLu, nRd, nRu;
	int nTemp;
	float fLength = 0.0f;
	BOOL bLd,bRu;
	D3DXVECTOR3 vecDiff = vecDir;

	bool bHit = false;
	pos.y += 20;

	// 計算する範囲の設定
	nCntUStart = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x) / (int)m_fLengthMesh;
	nCntVStart = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z) / (int)m_fLengthMesh;

	nCntUEnd = (int)(m_fLengthMesh * m_nDivNumU / 2 + pos.x + vecDir.x) / (int)m_fLengthMesh;
	nCntVEnd = (int)(m_fLengthMesh * m_nDivNumV / 2 - pos.z - vecDir.z) / (int)m_fLengthMesh;

	if (nCntUEnd < nCntUStart)
	{
		nTemp = nCntUEnd;

		nCntUEnd = nCntUStart;

		nCntUStart = nTemp;
	}

	if (nCntVEnd < nCntVStart)
	{
		nTemp = nCntVEnd;

		nCntVEnd = nCntVStart;

		nCntVStart = nTemp;
	}

	// 向きの正規化
	D3DXVec3Normalize(&vecDir, &vecDir);

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = nCntVStart; nCntV < nCntVEnd; nCntV++)
	{// 全ポリゴンのチェック
		for (nCntU = nCntUStart; nCntU < nCntUEnd; nCntU++)
		{
			// 頂点番号計算
			nLd = (m_nDivNumU + 1) * (nCntV + 1) + nCntU;
			nLu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU + 1) + nCntU;
			nRd = (m_nDivNumU + 1) * (nCntV + 1) + 1 + nCntU;
			nRu = (m_nDivNumU + 1) * (nCntV + 1) - (m_nDivNumU) + nCntU;

			bLd = D3DXIntersectTri(&pVtx[nLd].pos, &pVtx[nLu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);
			bRu = D3DXIntersectTri(&pVtx[nLu].pos, &pVtx[nRu].pos, &pVtx[nRd].pos, &pos, &vecDir, nullptr, nullptr, &fLength);

			// 交差しているかチェック
			if (bLd || bRu)
			{
				if (D3DXVec3Length(&vecDiff) > fLength)
				{
					return true;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return bHit;
}

//=====================================================
// 色の変更
//=====================================================
void CMeshField::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の変更
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// テクスチャ分割数設定
//=====================================================
void CMeshField::SetDivTex(int nDivTex)
{
	m_nDivTex = nDivTex;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountV = 0; nCountV < m_nDivNumV + 1; nCountV++)
	{// 頂点座標の設定
		for (int nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			// 頂点座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;

			// テクスチャ座標
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex = D3DXVECTOR2
			(
				((float)m_nDivTex / (float)m_nDivNumU) * nCountU,
				((float)m_nDivTex / (float)m_nDivNumU) * nCountV
			);

			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex2 = pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 高さのリセット
//=====================================================
void CMeshField::Reset(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の変更
	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		// 頂点カラーの設定
		pVtx[nCnt].pos.y = 0.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 波打つ処理
//=====================================================
void CMeshField::Wave(float fRot)
{
	CIceManager* pIceManager = CIceManager::GetInstance();
	if (pIceManager == nullptr)
		return;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	COcean::E_Stream nOceanRot = pIceManager->GetDirStream();	// 海流の向きを取得
	COcean::E_Stream nOceanRotNext = pIceManager->GetDirStreamNext();	// 次の海流の向きを取得
	float oceanSpeed = pIceManager->GetOceanLevel();	// 氷の流れる速度取得

	// 海流の速度が上がっている状態の時
	if (m_eOcean_Speed_State == OCEAN_STATE_UP)
	{
		oceanSpeed += m_fAcceleOcean;

		if (nOceanRot != nOceanRotNext)
		{
			nOceanRot = nOceanRotNext;
			pIceManager->SetDirStream(nOceanRot);	//現状の海流の向きを次の向きに更新
		}
	}

	// 海流の速度が下がっている状態の時
	if (m_eOcean_Speed_State == OCEAN_STATE_DOWN)
	{
		oceanSpeed -= m_fAcceleOcean * 2;
	}

	if (oceanSpeed < 0.0f)
	{
		m_eOcean_Speed_State = OCEAN_STATE_UP;
	}

	if (oceanSpeed > m_fMaxSpeed)
	{
		m_eOcean_Speed_State = OCEAN_STATE_NONE;
	}

	// 氷の流れる速度設定
	pIceManager->SetOceanLevel(oceanSpeed);

	for (nCountV = 0; nCountV < m_nDivNumV + 1; nCountV++)
	{// 頂点座標の設定
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			//	矢印が海流の向きに流れる処理
			if (nOceanRot == COcean::STREAM_UP)
			{// 海流が上向き
				pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y = sinf(fRot) * oceanSpeed * OCEAN_SPEED_MULTIPLY;
			}

			if (nOceanRot == COcean::STREAM_RIGHT)
			{// 海流が右向き
				pVtx[nCountU * (m_nDivNumV + 1) + nCountV].pos.y = sinf(fRot) * oceanSpeed * OCEAN_SPEED_MULTIPLY;
			}

			if (nOceanRot == COcean::STREAM_DOWN)
			{// 海流が下向き
				pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y = sinf(fRot) * oceanSpeed * OCEAN_SPEED_MULTIPLY;
			}

			if (nOceanRot == COcean::STREAM_LEFT)
			{// 海流が左向き
				pVtx[nCountU * (m_nDivNumV + 1) + nCountV].pos.y = sinf(fRot) * oceanSpeed * OCEAN_SPEED_MULTIPLY;
			}
		}

		if (nOceanRot == COcean::STREAM_UP ||
			nOceanRot == COcean::STREAM_LEFT)
		{// 海流が上向き
			fRot += 1.0f;
		}

		if (nOceanRot == COcean::STREAM_DOWN ||
			nOceanRot == COcean::STREAM_RIGHT)
		{// 海流が下向き
			fRot -= 1.0f;
		}

		universal::LimitRot(&fRot);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// スクロール処理
//=====================================================
void CMeshField::Scroll(int nIdx, D3DXVECTOR2 vec)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nIdx == 0)
	{
		// テクスチャ座標に加算
		for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
			pVtx[nCnt].tex += vec;
	}
	else if (nIdx == 1)
	{
		// テクスチャ座標に加算
		for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
			pVtx[nCnt].tex2 += vec;
	}
	else
		assert(false);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// スクロールの限界確認
	CheckScroll();
}

//=====================================================
// スクロールの限界チェック
//=====================================================
void CMeshField::CheckScroll(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//-----------------------------
	// 1番目のテクスチャ座標
	//-----------------------------
	bool bLimit = false;

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		if (pVtx[nCnt].tex.x * pVtx[nCnt].tex.x > LIMIT_SCROLL * LIMIT_SCROLL)
			bLimit = true;

		if (pVtx[nCnt].tex.y * pVtx[nCnt].tex.y > LIMIT_SCROLL * LIMIT_SCROLL)
			bLimit = true;
	}

	if (bLimit)
	{
		for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
		{
			if (pVtx[nCnt].tex.x > LIMIT_SCROLL)
				pVtx[nCnt].tex.x -= LIMIT_SCROLL;

			if (pVtx[nCnt].tex.x < -LIMIT_SCROLL)
				pVtx[nCnt].tex.x += LIMIT_SCROLL;

			if (pVtx[nCnt].tex.y > LIMIT_SCROLL)
				pVtx[nCnt].tex.y -= LIMIT_SCROLL;

			if (pVtx[nCnt].tex.y < -LIMIT_SCROLL)
				pVtx[nCnt].tex.y += LIMIT_SCROLL;
		}
	}

	//-----------------------------
	// 2番目のテクスチャ座標
	//-----------------------------
	bLimit = false;

	for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
	{
		if (pVtx[nCnt].tex2.x * pVtx[nCnt].tex2.x > LIMIT_SCROLL * LIMIT_SCROLL)
			bLimit = true;

		if (pVtx[nCnt].tex2.y * pVtx[nCnt].tex2.y > LIMIT_SCROLL * LIMIT_SCROLL)
			bLimit = true;
	}

	if (bLimit)
	{
		for (int nCnt = 0; nCnt < m_MeshField.nNumVtx; nCnt++)
		{
			if (pVtx[nCnt].tex2.x > LIMIT_SCROLL)
				pVtx[nCnt].tex2.x -= LIMIT_SCROLL;

			if (pVtx[nCnt].tex2.x < -LIMIT_SCROLL)
				pVtx[nCnt].tex2.x += LIMIT_SCROLL;

			if (pVtx[nCnt].tex2.y > LIMIT_SCROLL)
				pVtx[nCnt].tex2.y -= LIMIT_SCROLL;

			if (pVtx[nCnt].tex2.y < -LIMIT_SCROLL)
				pVtx[nCnt].tex2.y += LIMIT_SCROLL;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CMeshField::Draw(void)
{
	CObject3D::Draw();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	// 1枚目のテクスチャ設定
	LPDIRECT3DTEXTURE9 pBaseTexture = CTexture::GetInstance()->GetAddress(m_nIdxTexture);
	pDevice->SetTexture(0, pBaseTexture);
	
	// 2枚目のテクスチャ設定
	LPDIRECT3DTEXTURE9 pTexture2 = CTexture::GetInstance()->GetAddress(m_nIdxTextureOverRay);

	if (pTexture2 != nullptr)
	{
		pDevice->SetTexture(1, pTexture2);

		// テクスチャステージの設定（2枚目のテクスチャ）
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);	// 2枚目のテクスチャの色
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);	// 現在の色（1枚目）
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);		// 色を加算
		pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);			// 2番目のUVを使用
	}

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive (D3DPT_TRIANGLESTRIP, 0, 0, m_MeshField.nNumVtx, 0, m_MeshField.nNumIdx - 2);

	// 設定を元に戻す
	pDevice->SetTexture(1, NULL); // 2枚目のテクスチャを解除
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);		// 色を加算
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//=====================================================
// 読込処理
//=====================================================
void CMeshField::Load(std::string path)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen((const char*)&path[0], "rb");

	if (pFile != nullptr)
	{
		fread(&m_nDivNumU, sizeof(int), 1, pFile);
		fread(&m_nDivNumV, sizeof(int), 1, pFile);
		fread(&m_nDivTex, sizeof(int), 1, pFile);
		fread(&m_fLengthMesh, sizeof(float), 1, pFile);

		// 頂点情報のポインタ
		VERTEX_3D *pVtx;

		m_MeshField.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

		if (m_pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshField.nNumVtx * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				nullptr);
		}

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < m_MeshField.nNumVtx; i++)
		{
			D3DXVECTOR3 temp;

			// 頂点の位置情報を読込
			fread(&pVtx[0].pos, sizeof(D3DXVECTOR3), 1, pFile);
			fread(&temp, sizeof(D3DXVECTOR3), 1, pFile);
			fread(&temp, sizeof(D3DXVECTOR3), 1, pFile);

			pVtx++;
		}

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=====================================================
// 保存処理
//=====================================================
void CMeshField::Save(std::string path)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen((const char*)&path[0], "wb");

	if (pFile != nullptr)
	{
		fwrite(&m_nDivNumU, sizeof(int), 1, pFile);
		fwrite(&m_nDivNumV, sizeof(int), 1, pFile);
		fwrite(&m_nDivTex, sizeof(int), 1, pFile);
		fwrite(&m_fLengthMesh, sizeof(float), 1, pFile);

		// 頂点情報のポインタ
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < m_MeshField.nNumVtx; i++)
		{
			// 頂点の位置情報を読込
			fwrite(&pVtx[0].pos, sizeof(D3DXVECTOR3), 1, pFile);

			pVtx++;
		}

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}
}