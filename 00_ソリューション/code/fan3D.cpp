//*****************************************************
//
// 3D円の処理[fan3D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan3D.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float SWING_WIDTH = 10.0f;  // sinカーブの振れ幅
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFan3D::CFan3D(int nPriority) : CFan(nPriority)
{
	m_bBillboard = false;
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));
	m_nStartOffsetCount = 0;
	m_nIdxTextureOverRay = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CFan3D::~CFan3D()
{

}

//=====================================================
// 生成処理
//=====================================================
CFan3D *CFan3D::Create(int nPriority, int nNumVtx)
{
	CFan3D *pFan3D = nullptr;

	// インスタンス生成
	pFan3D = new CFan3D(nPriority);

	if (pFan3D != nullptr)
	{
		pFan3D->SetNumVtx(nNumVtx);

		// 初期化処理
		pFan3D->Init();
	}

	return pFan3D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan3D::Init(void)
{
	CFan::Init();

	Add3D();

	// 親マトリックスリセット
	ResetMtxParent();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		LPDIRECT3DVERTEXBUFFER9 *pVtxBuff = GetVtxBuff();
		int nNumVtx = GetNumVtx();

		if (*pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// 頂点設定
	SetVtx();

	EnableLighting(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan3D::Uninit(void)
{
	Remove3D();

	CFan::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CFan3D::Update(void)
{

}

//=====================================================
// 親マトリックスのリセット
//=====================================================
void CFan3D::ResetMtxParent(void)
{
	D3DXMatrixIdentity(&m_mtxParent);
}

//=====================================================
// 頂点位置設定
//=====================================================
void CFan3D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 中心の頂点の設定
		pVtx[0].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };
		pVtx[0].tex2 = D3DXVECTOR2{ 0.5f,0.5f };
		pVtx[0].col = GetCol();

		for (int i = 1;i < nNumVtx + 2;i++)
		{// 円周の頂点の設定
			float fAngle = (fAngleMax * fRateAngle) * ((float)(i - 1) / (float)nNumVtx);

			universal::LimitRot(&fAngle);
			
			pVtx[i].pos =
			{
				sinf(fAngle) * fRadius,
				0.0f,
				cosf(fAngle) * fRadius,
			};

			float f = 0.1f * sinf((float)i * 0.01f);

			D3DXVECTOR2 tex =
			{
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;
			pVtx[i].tex2 = tex;

			pVtx[i].col = GetCol();
		}

		// 頂点バッファのアンロック
		pVtxBuff->Unlock();
	}
}

//=====================================================
// 色設定
//=====================================================
void CFan3D::SetCol(D3DXCOLOR col)
{
	CFan::SetCol(col);

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	int nNumVtx = GetNumVtx();

	if (pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = col;
		}

		// 頂点バッファのアンロック
		pVtxBuff->Unlock();
	}
}

//=====================================================
// テクスチャ座標設定処理
//=====================================================
void CFan3D::SetTex(float fScrollX, float fScrollY)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < nNumVtx + 2; i++)
		{
			float fTexY = fScrollY * sinf((float)(m_nStartOffsetCount + i) / SWING_WIDTH);

			pVtx[i].tex.x = pVtx[i].tex.x + fScrollX;
			pVtx[i].tex.y = pVtx[i].tex.y + fTexY;
		}

		m_nStartOffsetCount++;
		
		// 頂点バッファのアンロック
		pVtxBuff->Unlock();
	}
}

//=====================================================
// 描画
//=====================================================
void CFan3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		//ワールドマトリックス初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{// ビルボード描画
			DrawBillboard();
		}
		else
		{// 通常の描画
			DrawNormal();
		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// カリングを無効化
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// 1枚目のテクスチャ設定
		int nIdxTexture1 = GetIdxTexture();
		LPDIRECT3DTEXTURE9 pBaseTexture = CTexture::GetInstance()->GetAddress(nIdxTexture1);
		pDevice->SetTexture(0, pBaseTexture);

		// 2枚目のテクスチャ設定
		LPDIRECT3DTEXTURE9 pTexture2 = CTexture::GetInstance()->GetAddress(m_nIdxTextureOverRay);
		pDevice->SetTexture(1, pTexture2);

		// テクスチャステージの設定（2枚目のテクスチャ）
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);	// 2枚目のテクスチャの色
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);	// 現在の色（1枚目）
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);		// 色を加算
		pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);			// 2番目のUVを使用

		// 描画
		CFan::Draw();

		// 設定を元に戻す
		pDevice->SetTexture(1, NULL); // 2枚目のテクスチャを解除
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);		// 色を加算
		pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);

		// カリングを有効化
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//=====================================================
// 通常描画
//=====================================================
void CFan3D::DrawNormal(void)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXVECTOR3 rot = GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親マトリックスをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// ビルボード描画
//=====================================================
void CFan3D::DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに向ける
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}