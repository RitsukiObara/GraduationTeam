//*****************************************************
//
// 氷の処理[ice.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ice.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_FLOWS = 1.0f;	// 流れる速度
const string PATH_TEX = "data\\TEXTURE\\UI\\ice.png";	// テクスチャパス
const float SIZE_INIT = 100.0f;	// 初期サイズ
const float HEIGHT_ICE = 50.0f;	// 氷の高さ
const int NUM_CORNER = 6;	// 角の数
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// 上側の初期向き
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CIce::s_nNumAll = 0;

//=====================================================
// コンストラクタ
//=====================================================
CIce::CIce(int nPriority) : CGameObject(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false), m_bAliveStandBlock(false),
m_pSide(nullptr),m_pUp(nullptr)
{
	s_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CIce *CIce::Create(E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		pIce = new CIce;

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// メッシュの生成
	CreateMesh();

	// トランスフォームの初期設定
	SetTransform(SIZE_INIT);

	return S_OK;
}

//=====================================================
// メッシュの生成
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(3, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			m_pUp->SetRotation(ROT_UP_INIT);
			m_pUp->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (m_pSide == nullptr)
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
}

//=====================================================
// 終了処理
//=====================================================
void CIce::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIce::Update(void)
{
	if (m_state == E_State::STATE_FLOWS)
	{// 流れる状態では移動を続ける
		Flows();
	}
}

//=====================================================
// 流れる処理
//=====================================================
void CIce::Flows(void)
{
	AddPosition(D3DXVECTOR3(SPEED_FLOWS, 0.0f, 0.0f));
}

//=====================================================
// 描画処理
//=====================================================
void CIce::Draw(void)
{

}

//=====================================================
// トランスフォーム設定
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// 上の扇ポリゴンの設定
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// サイドのシリンダーの設定
	m_pSide->SetRadius(fRadius);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);
}