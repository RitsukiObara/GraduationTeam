//*****************************************************
//
// 魚影の処理[fishshadow.cpp]
// Author:若木一真　髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fishshadow.h"
#include "texture.h"
#include "manager.h"
#include "iceManager.h"
#include "enemy.h"
#include "particle.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const char* PATH_TEX = "data\\TEXTURE\\enemy\\FishShadow.png";	// テクスチャパス
const float TIME_FADEIN = 1.0f;									// フェードインにかかる時間
const float TIME_FADEOUT = 1.0f;								// フェードアウトにかかる時間
const D3DXCOLOR COL_INIT = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 初期色
const float WIDTH = 70.0f;										// 幅
const float HEIGHT = 140.0f;									// 高さ
const float DIST_APPER = 500.0f;								// 出現する距離
const float RATE_DEST = 0.7f;									// 目標位置の割合

const int FRAME_BUBBLE = 5;										// 泡を出す頻度

//-----------------------------------
// アニメーションの定数定義
//-----------------------------------
namespace anim
{
const int PATERN_ANIM = 5;	// アニメーションパターン
const int FRAME_ANIM = 5;	// アニメーションが切り替わるフレーム数
}
}

//====================================================
// コンストラクタ
//====================================================
CFishShadow::CFishShadow(int nPriority) : CAnim3D(nPriority), m_fTimerVanish(0.0f), m_nCntBubble(0)
{

}

//====================================================
// デストラクタ
//====================================================
CFishShadow::~CFishShadow()
{

}

//====================================================
// 生成処理
//====================================================
CFishShadow* CFishShadow::Create(int nPatern)
{
	CFishShadow* pFishShadow = nullptr;

	pFishShadow = new CFishShadow;

	if (pFishShadow != nullptr)
	{
		pFishShadow->Init();
		pFishShadow->InitSpawn(nPatern);
	}

	return pFishShadow;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CFishShadow::Init(void)
{
	// 継承クラスの初期化
	CAnim3D::Init();

	// テクスチャ設定
	int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	// 前面に出す
	EnableZtest(true);

	// 色初期設定
	SetColor(COL_INIT);

	// サイズ設定
	SetSize(0.0f, 0.0f);

	// アニメーション設定
	SetNumAnim(anim::PATERN_ANIM);
	SetSpeedAnim(anim::FRAME_ANIM);
	EnableLoop(true);

	return S_OK;
}

//====================================================
// スポーンの初期化
//====================================================
void CFishShadow::InitSpawn(int nPatern)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	//--------------------------------
	// 目標位置の設定
	//--------------------------------
	D3DXVECTOR3 posDest = { 0.0f,0.0f,0.0f };

	switch (nPatern)	// パターンごとのグリッド位置を取得
	{
	case CEnemy::E_Spawn::SPAWN_RU:
		posDest = pIceMgr->GetRightUpGrid().pos;
		break;
	case CEnemy::E_Spawn::SPAWN_LU:
		posDest = pIceMgr->GetLeftUpGrid().pos;
		break;
	case CEnemy::E_Spawn::SPAWN_RD:
		posDest = pIceMgr->GetRightDownGrid().pos;
		break;
	case CEnemy::E_Spawn::SPAWN_LD:
		posDest = pIceMgr->GetLeftDownGrid().pos;
		break;
	default:
		break;
	}

	m_posDest = posDest * RATE_DEST;

	//--------------------------------
	// 初期位置の設定
	//--------------------------------
	D3DXVECTOR3 posInit = m_posDest;
	D3DXVECTOR3 vecAdd;

	// 目標位置からさらに遠くする
	D3DXVec3Normalize(&vecAdd, &m_posDest);
	vecAdd *= DIST_APPER;

	posInit += vecAdd;

	m_posInit = posInit;

	SetPosition(posInit);

	//--------------------------------
	// 向きの設定
	//--------------------------------
	float fAngle = atan2f(-vecAdd.x, -vecAdd.z);

	SetRotation(D3DXVECTOR3(0.0f, fAngle, 0.0f));
}

//====================================================
// 終了処理
//====================================================
void CFishShadow::Uninit(void)
{
	// 継承クラスの終了
	CAnim3D::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CFishShadow::Update(void)
{
	// 継承クラスの更新
	CAnim3D::Update();

	if (m_fTimerVanish >= fishshadow::TIME_VANISH)
	{
		Uninit();
		return;
	}

	// タイマー加算
	m_fTimerVanish += CManager::GetDeltaTime();

	if (m_fTimerVanish <= TIME_FADEIN)	// フェードイン状態
		UpdateFadeIn();
	else if (m_fTimerVanish >= fishshadow::TIME_VANISH - TIME_FADEOUT)	// フェードアウト状態
		UpdateFadeOut();

	// 移動処理
	Move();

	m_nCntBubble++;

	if (m_nCntBubble == FRAME_BUBBLE)
	{
		// パーティクルを出す
		CParticle::Create(GetPosition(), CParticle::TYPE::TYPE_BUBBLE_SINK);

		m_nCntBubble = 0;
	}
}

//====================================================
// フェードイン状態
//====================================================
void CFishShadow::UpdateFadeIn(void)
{
	// タイマーのイージング
	float fTime = m_fTimerVanish / TIME_FADEIN;
	float fRate = easing::EaseOutExpo(fTime);

	SetAlpha(fRate);
	SetVtx();
}

//====================================================
// フェードアウト状態
//====================================================
void CFishShadow::UpdateFadeOut(void)
{
	// タイマーのイージング
	float fTimer = TIME_FADEOUT - (fishshadow::TIME_VANISH - m_fTimerVanish);

	float fTime = fTimer / TIME_FADEIN;
	float fRate = easing::EaseOutExpo(fTime);

	SetAlpha(1.0f - fRate);
	SetVtx();
}

//====================================================
// 移動状態
//====================================================
void CFishShadow::Move(void)
{
	D3DXVECTOR3 vecDiff = m_posDest - m_posInit;

	// タイマーのイージング
	float fTime = m_fTimerVanish / fishshadow::TIME_VANISH;
	float fRate = easing::EaseOutExpo(fTime);

	D3DXVECTOR3 pos = m_posInit + vecDiff * fRate;

	SetPosition(pos);

	// サイズ設定
	SetSize(WIDTH * fRate,HEIGHT * fRate);
}

//====================================================
// 描画処理
//====================================================
void CFishShadow::Draw(void)
{
	// 継承クラスの描画
	CAnim3D::Draw();
}