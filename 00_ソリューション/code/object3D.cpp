//*****************************************************
//
// 3Dオブジェクト[object3D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"
#include "renderer.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject3D::CObject3D(int nPriority) : CGameObject(nPriority), m_mtxWorld(), m_mtxParent(), m_scale()
{

}

//=====================================================
// デストラクタ
//=====================================================
CObject3D::~CObject3D()
{

}

//=====================================================
// 生成処理
//=====================================================
CObject3D *CObject3D::Create(void)
{
	CObject3D *pObject3D = new CObject3D;

	if (pObject3D != nullptr)
		pObject3D->Init();

	return pObject3D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CObject3D::Init(void)
{
	// 親マトリックスの初期化
	ResetMtxParent();

	// スケール初期化
	m_scale = { 1.0f,1.0f,1.0f };

	// 3D描画に追加
	Add3D();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CObject3D::Uninit(void)
{
	Remove3D();

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CObject3D::Update(void)
{

}

//=====================================================
// 描画
//=====================================================
void CObject3D::Draw(void)
{
	// マトリックスの計算
	CulcMatrix();
}

//=====================================================
// マトリックスの計算
//=====================================================
void CObject3D::CulcMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXVECTOR3 scale = GetScale();  // スケールを取得する関数
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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
// 親マトリックスのリセット
//=====================================================
void CObject3D::ResetMtxParent(void)
{
	D3DXMatrixIdentity(&m_mtxParent);
}

//=====================================================
// 前方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetForward(void)
{
	return D3DXVECTOR3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
}

//=====================================================
// 上方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetUp(void)
{
	return D3DXVECTOR3(m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23);
}

//=====================================================
// 右方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetRight(void)
{
	return D3DXVECTOR3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
}