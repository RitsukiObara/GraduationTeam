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
	namespace Stand
	{
		const int MOTION_COUNT = 60 * 3;	// モーションをする時間
		const vector<CNPCPenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_NECKSHAKE,	// 首振り
			CNPCPenguin::MOTION_STOMACH,		// 腹ベタァ
			CNPCPenguin::MOTION_UNYO,		// 首うにょん
			CNPCPenguin::MOTION_WINGPTPT		// 羽パタ
		};
	}
	namespace Move
	{
		const int MOTION_COUNT = 60 * 5 + 20;	// モーションをする時間
		const int FALL_CHANCE = 18;			// コケ確率（値：％）
		const float MOVE_SPEED = 2.5f;		// 移動速度
	}
	namespace ShakeHead
	{
		const int MOTION_COUNT = 60 * 2;	// モーションをする時間
	}
	namespace Stomach
	{
		const int MOTION_COUNT = 60 * 3;	// モーションをする時間
	}
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

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguin::Uninit(void)
{
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

	if (m_pCollisionSphere != nullptr)
	{// 球の判定の追従
		D3DXVECTOR3 pos = GetPosition();

		m_pCollisionSphere->SetPosition(pos);

		// ブロック判定
		m_pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// キャラの位置反映
		SetPosition(pos);
	}

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
