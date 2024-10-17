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
#include "ocean.h"
#include "iceHard.h"
#include "objectX.h"
#include "iceManager.h"
#include "manager.h"
#include "particle.h"
#include "model.h"

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

const float TIME_REPAIR_ICE = 10.0f;	// 氷の修復にかかる時間
const int MAX_SCALE = 50; // スケールの最大値
const int MIN_SCALE = 20; // スケールの最小値
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_small.x";	// 破片氷のモデルパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::m_Vector = {};	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIce::CIce(int nPriority) : CGameObject(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false), m_bAliveStandBlock(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr)
{
	s_nNumAll++;
	m_Vector.push_back(this);
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
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		default:
			assert(false);
			break;
		}

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

	ChangeState(new CIceStaeteNormal);

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

	SetTransform(Grid::SIZE);
}

//=====================================================
// メッシュの破棄
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// 終了処理
//=====================================================
void CIce::Uninit(void)
{
	// メッシュの削除
	DeleteMesh();

	for (auto itr = m_Vector.begin(); itr < m_Vector.end(); itr++)
	{
		//削除対象じゃない場合
		if (*itr != this)
		{
			continue;
		}
		//Vectorから削除
		m_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIce::Update(void)
{
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (COcean::GetInstance() == nullptr)
	{
		return;
	}

	// 海と一緒に氷を動かす処理
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = COcean::GetInstance()->GetHeight(pos,&move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
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

//=====================================================
// ステイトの変更
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// 通常ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{

}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{

}

//*******************************************************************************
// 崩壊ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// 氷のメッシュを削除
	pIce->DeleteMesh();
	
	// ここで氷破壊時のエフェクトを発生
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// 氷破片を生成
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// モデルの割り当て
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// 位置をランダム設定
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// スケールをランダム設定
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// 回転
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// 配列に入れる
		m_aPeaceIce.push_back(pPeace);
	}

	// カウンターを初期化
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// タイマーを減らす
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// 一定時間経過で、氷を修復する
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}
}

//*******************************************************************************
// 流れるステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteFlow::Init(CIce *pIce)
{

}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteFlow::Update(CIce *pIce)
{
	pIce->AddPosition(D3DXVECTOR3(-SPEED_FLOWS, 0.0f, 0.0f));
}

//*******************************************************************************
// 沈むステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}