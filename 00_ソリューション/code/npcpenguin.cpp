//*****************************************************
//
// プレイヤー操作なしペンギン[npcpenguin.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguin.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"
#include "shadow.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
	const float SCALE_BODY = 1.8f;	// 体のスケール
	
	const float FACT_DECREASE_MOVE = 0.9f;	// 移動量の減衰係数
	const float ADD_MOVE = 4.0f;	// 移動の追加量
	
	const float RADIUS_COLLISION = 200.0f;	// 球の判定の半径
}

//=====================================================
// コンストラクタ
//=====================================================
CNPCPenguin::CNPCPenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
	nCntMove = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CNPCPenguin::~CNPCPenguin()
{

}

//=====================================================
// 生成処理
//=====================================================
CNPCPenguin* CNPCPenguin::Create(INPCPenguinState* pState)
{
	CNPCPenguin *pNPCPenguin = nullptr;

	pNPCPenguin = new CNPCPenguin;

	if (pNPCPenguin != nullptr)
	{
		pNPCPenguin->Init();
		if (pState != nullptr)
		{
			pNPCPenguin->SetState(pState);
		}
	}

	return pNPCPenguin;
}

//=====================================================
// ステート設定処理
//=====================================================
void CNPCPenguin::SetState(INPCPenguinState* pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit();
		m_pState = nullptr;
	}
	
	m_pState = pState;
	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguin::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// スケールの設定
	SetScale(SCALE_BODY);

	// 球の判定生成
	m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pCollisionSphere != nullptr)
	{
		m_pCollisionSphere->SetRadius(RADIUS_COLLISION);
		m_pCollisionSphere->SetPosition(GetPosition());
	}

	// 影の生成
	m_pShadow = CShadow::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguin::Uninit(void)
{
	// 影削除
	Object::DeleteObject((CObject**)&m_pShadow);

	if (m_pState != nullptr)
	{
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguin::Update(void)
{
	// モーション更新
	CMotion::Update();

	D3DXVECTOR3 pos = GetPosition();

	if (m_pCollisionSphere != nullptr)
	{// 球の判定の追従
		m_pCollisionSphere->SetPosition(pos);

		// ブロック判定
		m_pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// キャラの位置反映
		SetPosition(pos);
	}

	// 影の追従
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(pos);

	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//=====================================================
// モーション状態
//=====================================================
void CNPCPenguin::MotionState(void)
{
	nCntMove++;

	if (nCntMove > 70)
	{
		int nRandNextMotion = universal::RandRange(CNPCPenguin::MOTION::MOTION_WALK, CNPCPenguin::MOTION::MOTION_WALK);


		nCntMove = 0;
	}

	//else
	//{
	//	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);
	//}
}

//=====================================================
// 描画処理
//=====================================================
void CNPCPenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}
