//*****************************************************
//
// 影の処理[shadow.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shadow.h"
#include "texture.h"
#include "iceManager.h"
#include "ocean.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const float SIZE_INIT = 100.0f;									// 初期のサイズ
const D3DXCOLOR COL_INIT = { 0.0f,0.0f,0.0f,1.0f };				// 初期色
const string PATH_TEX = "data\\TEXTURE\\EFFECT\\effect000.png";	// テクスチャパス

const float RATE_COLLIDE_ICE = 1.0f;	// 氷との判定の割合
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CShadow::CShadow(int nPriority) : CPolygon3D(nPriority)
{

}

//=====================================================
// 生成処理
//=====================================================
CShadow *CShadow::Create(void)
{
	CShadow *pShadow = new CShadow;

	if (pShadow != nullptr)
		pShadow->Init();

	return pShadow;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShadow::Init(void)
{
	// 継承クラスの初期化
	CPolygon3D::Init();

	// テクスチャ設定
	int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	// サイズ設定
	SetSize(SIZE_INIT, SIZE_INIT);
	SetVtx();

	// 前面に出す
	EnableZtest(true);

	// 色設定
	SetColor(COL_INIT);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CPolygon3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CShadow::Update(void)
{
	// 継承クラスの更新
	CPolygon3D::Update();
}

//=====================================================
// 氷との判定
//=====================================================
bool CShadow::CollideIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// 一番近い氷の取得
	D3DXVECTOR3 pos = GetPosition();
	CIce *pIce = pIceMgr->GetNearestIce(pos);

	if (pIce == nullptr)
		return false;

	// 氷の上に立っていなければ処理を偽を返す
	if (!pIceMgr->IsInIce(pos, pIce, RATE_COLLIDE_ICE))
		return false;

	pos.y = pIce->GetPosition().y;

	CPolygon3D::SetPosition(pos);

	return true;
}

//=====================================================
// 海との判定
//=====================================================
void CShadow::CollideOcean(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
		return;

	// 海の高さに合わせる
	D3DXVECTOR3 pos = GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	CPolygon3D::SetPosition(pos);
}

//=====================================================
// 位置の設定
//=====================================================
void CShadow::SetPosition(D3DXVECTOR3 pos)
{
	CPolygon3D::SetPosition(pos);

	if (!CollideIce())	// 氷に当たらなかったら海との判定を行う
		CollideOcean();
}

//=====================================================
// 描画処理
//=====================================================
void CShadow::Draw(void)
{
	// 継承クラスの描画
	CPolygon3D::Draw();
}