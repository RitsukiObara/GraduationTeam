//*****************************************************
//
// モード選択ペンギン[selectmodepenguin.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectmodepenguin.h"
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
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectModePenguin::CSelectModePenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// デストラクタ
//=====================================================
CSelectModePenguin::~CSelectModePenguin()
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectModePenguin* CSelectModePenguin::Create(ISelectModePenguinState* pState)
{
	CSelectModePenguin *pSelectModePenguin = nullptr;

	pSelectModePenguin = new CSelectModePenguin;

	if (pSelectModePenguin != nullptr)
	{
		pSelectModePenguin->Init();
		if (pState != nullptr)
		{
			pSelectModePenguin->SetState(pState);
		}
	}

	return pSelectModePenguin;
}

//=====================================================
// ステート設定処理
//=====================================================
void CSelectModePenguin::SetState(ISelectModePenguinState* pState)
{
	m_pState = pState;
	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguin::Init(void)
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
void CSelectModePenguin::Uninit(void)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguin::Update(void)
{
	// 移動量分移動
	Translate(m_move);

	// 移動量の減衰
	m_move *= FACT_DECREASE_MOVE;

	// モーションの管理
	ManageMotion();

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
// 描画処理
//=====================================================
void CSelectModePenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// ジャンプの終了
//=====================================================
void CSelectModePenguin::EndJump(void)
{
	// ジャンプモーションフラグを折る
	m_fragMotion.bJump = false;

	// 入力を有効化
	EnableInput(true);
}

//=====================================================
// モーションの管理
//=====================================================
void CSelectModePenguin::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bJump)
	{// ジャンプ中
		if (nMotion == MOTION::MOTION_LANDING)
		{// 着地モーション終了でジャンプ終了
			if (bFinish)
				EndJump();
		}
		else if (nMotion == MOTION::MOTION_STARTJUMP)
		{// ジャンプ開始モーション終了からの遷移
			if (bFinish)
			{
				SetMotion(MOTION::MOTION_STAYJUMP);
			}
		}
		else if (nMotion != MOTION::MOTION_STARTJUMP)	// ジャンプ開始モーションの開始
			SetMotion(MOTION::MOTION_STARTJUMP);
	}
	else if ((nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK) && !bFinish)
	{

	}
	else if (m_fragMotion.bWalk)
	{// 歩きモーションフラグ有効
		if (nMotion != MOTION::MOTION_WALK)
			SetMotion(MOTION::MOTION_WALK);
	}
	else
	{// 何もフラグが立っていない状態
		if (nMotion != MOTION::MOTION_NEUTRAL)
			SetMotion(MOTION::MOTION_NEUTRAL);
	}
}

//*****************************************************
// 立ってるだけステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_Stand::Init(CSelectModePenguin* pPenguin)
{
	return S_OK;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_Stand::Update(CSelectModePenguin* pPenguin)
{
}
