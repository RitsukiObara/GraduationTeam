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

const float RATE_RADIUS_HEIGHT = 0.15f;		// 高さによる半径の変化幅
const float RATE_DENSITY_HEIGHT = 0.002f;	// 高さによる濃さの変化幅
const float MINDENSITY = 0.1f;				// 濃さの最低値
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
CShadow *CShadow::Create(int nPriority)
{
	CShadow *pShadow = new CShadow(nPriority);

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
	m_fRadiusInitial = SIZE_INIT;
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

	SetRadiusHeight(pos, GetPosition());
}

//=====================================================
// 高さによる半径の設定
//=====================================================
void CShadow::SetRadiusHeight(D3DXVECTOR3 pos, D3DXVECTOR3 posNew)
{
	float fHeightDiff = pos.y - posNew.y;
	float fRadius = SIZE_INIT;

	// 高さによる半径の計算
	fRadius += fHeightDiff * RATE_RADIUS_HEIGHT;

	// 半径反映
	SetSize(fRadius, fRadius);
	SetVtx();

	// 濃さの計算
	float fDensity = 1.0f;
	fDensity -= fHeightDiff * RATE_DENSITY_HEIGHT;

	universal::LimitValuefloat(&fDensity, 1.0f, MINDENSITY);

	SetAlpha(fDensity);
}

//=====================================================
// 描画処理
//=====================================================
void CShadow::Draw(void)
{
	// 継承クラスの描画
	CPolygon3D::Draw();
}