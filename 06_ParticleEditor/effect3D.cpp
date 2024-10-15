//*****************************************************
//
// ３Dエフェクト処理[effect3D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(7.0f)	// 移動速度

//=====================================================
// コンストラクタ
//=====================================================
CEffect3D::CEffect3D(int nPriority) : CObject3D(nPriority)
{
	m_nLife = 0;
	m_fDecreaseRadius = 0.0f;
	m_fDecreaseAlpha = 0;
	m_move = { 0.0f,0.0f,0.0f };
	m_bAdd = true;
	m_fGravity = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CEffect3D::~CEffect3D()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEffect3D::Init(void)
{
	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEffect3D::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEffect3D::Update(void)
{
	// 継承クラスの更新
	CObject3D::Update();

	D3DXVECTOR3 rot = universal::VecToRot(m_move);

	SetRot(rot);

	// 寿命減衰
	m_nLife--;

	// サイズ縮小
	SetSize(GetWidth() - m_fDecreaseRadius, GetHeight() - m_fDecreaseRadius);
	
	if (GetWidth() < 0.0f)
	{// 大きさの補正
		SetSize(0.0f, 0.0f);
	}

	// 重力加算
	m_move.y -= m_fGravity;

	// 位置更新
	SetPosition(GetPosition() + m_move);

	// 色減少
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g, GetColor().b, GetColor().a - m_fDecreaseAlpha));

	if (m_nLife < 0)
	{// 自分の削除
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEffect3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bAdd)
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//Zテストを無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	// 継承クラスの描画
	CObject3D::Draw();

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================
// 生成処理
//=====================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXCOLOR col, D3DXVECTOR3 move, float fGravity, bool bAdd,float fDecrease)
{
	CEffect3D *pEffect3D = NULL;

	if (pEffect3D == NULL)
	{// インスタンス生成
		pEffect3D = new CEffect3D;

		if (pEffect3D->GetID() == -1)
		{
			delete pEffect3D;
			pEffect3D = NULL;

			return NULL;
		}

		if (pEffect3D != NULL)
		{
			// 初期化処理
			pEffect3D->Init();
			pEffect3D->SetSize(fRadius, fRadius);

			// 位置設定
			pEffect3D->SetPosition(pos);

			pEffect3D->SetColor(col);

			// テクスチャの読込
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->m_fDecreaseRadius = fDecrease;

			pEffect3D->m_fDecreaseAlpha = 1.0f / (float)nLife;

			pEffect3D->m_fGravity = fGravity;

			pEffect3D->m_bAdd = bAdd;
		}
	}
	
	return pEffect3D;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CEffect3D::Load(void)
{
	return S_OK;
}

//=====================================================
// テクスチャ破棄
//=====================================================
void CEffect3D::Unload(void)
{

}