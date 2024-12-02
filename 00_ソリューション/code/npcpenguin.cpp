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
#include "player.h"
#include "iceManager.h"
#include "ice.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace Skin
	{
		const vector<string> PATH_MODEL =
		{
			"data\\MOTION\\motionPenguin.txt",	// 自然体
			"data\\MOTION\\motionPenguin_Pilot.txt",	// パイロット
			"data\\MOTION\\motionPenguin_HeadBang.txt",	// ハチマキ
			"data\\MOTION\\motionPenguin_Pirate.txt",	// 海賊
			"data\\MOTION\\motionPenguin_SwimSuit.txt",	// 水着
		};
	}
	const string DEFAULT_PATH_MODEL = "data\\MOTION\\motionPenguin.txt";
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
CNPCPenguin* CNPCPenguin::Create(INPCPenguinState* pState, SKIN skin)
{
	CNPCPenguin *pNPCPenguin = nullptr;

	pNPCPenguin = new CNPCPenguin;

	if (pNPCPenguin != nullptr)
	{
		// 初期化前にスキン設定
		if (skin == SKIN::SKIN_NONE)
		{
			pNPCPenguin->Load((char*)&DEFAULT_PATH_MODEL[0]);
		}
		else
		{
			pNPCPenguin->Load((char*)&player::PATH_BODY[skin][0]);
		}

		// 初期化
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
	m_pShadow = CShadow::Create(3);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguin::Uninit(void)
{
	// 影・当たり判定削除
	Object::DeleteObject((CObject**)&m_pShadow);
	Object::DeleteObject((CObject**)&m_pCollisionSphere);

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

	// 氷に乗っているとき乗る処理
	FollowIce();

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
// 描画処理
//=====================================================
void CNPCPenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// 氷の追従
//=====================================================
void CNPCPenguin::FollowIce(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	int nGridV, nGridH;
	if (pIceMgr->GetIdxGridFromPosition(GetPosition(), &nGridV, &nGridH))
	{
		CIce* pIceStand = pIceMgr->GetGridIce(&nGridV, &nGridH);

		if (pIceStand != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			pos.y = pIceStand->GetPosition().y;
			SetPosition(pos);
		}
	}
}