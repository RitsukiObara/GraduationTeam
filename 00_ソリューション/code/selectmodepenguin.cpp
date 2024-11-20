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
	namespace Stand
	{
		const int MOTION_COUNT = 60 * 3;	// モーションをする時間
		const vector<CSelectModePenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CSelectModePenguin::MOTION_WALK,		// 移動
			CSelectModePenguin::MOTION_WALK,		// 移動
			CSelectModePenguin::MOTION_WALK,		// 移動
			CSelectModePenguin::MOTION_NECKSHAKE,	// 首振り
			CSelectModePenguin::MOTION_STOMACH,		// 腹ベタァ
			CSelectModePenguin::MOTION_UNYO,		// 首うにょん
			CSelectModePenguin::MOTION_WINGPTPT		// 羽パタ
		};
	}
	namespace Move
	{
		const int MOTION_COUNT = 60 * 6;	// モーションをする時間
		const int FALL_CHANCE = 15;			// 転ぶ確立（値：％）
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
CSelectModePenguin::CSelectModePenguin(int nPriority) : CMotion(nPriority)
{
	m_pCollisionSphere = nullptr;
	nCntMove = 0;
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
		m_pState->Uninit();
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
void CSelectModePenguin::MotionState(void)
{
	nCntMove++;

	if (nCntMove > 70)
	{
		int nRandNextMotion = universal::RandRange(CSelectModePenguin::MOTION::MOTION_WALK, CSelectModePenguin::MOTION::MOTION_WALK);


		nCntMove = 0;
	}

	//else
	//{
	//	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NEUTRAL);
	//}
}

//=====================================================
// 描画処理
//=====================================================
void CSelectModePenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//*****************************************************
// 立ってるだけステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_Stand::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_Stand::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_Stand::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stand::MOTION_COUNT)
	{// ステート時間終了
		// どのステートに遷移するか抽選
		CSelectModePenguin::MOTION rnd = Stand::NEXT_MOTION_PATTERN[(rand() % (int)Stand::NEXT_MOTION_PATTERN.size())];
		switch (rnd)
		{
		case CSelectModePenguin::MOTION_WALK:
			pPenguin->SetState(new CSelectModePenguinState_Move);
			break;
		case CSelectModePenguin::MOTION_NECKSHAKE:
			pPenguin->SetState(new CSelectModePenguinState_ShakeHead);
			break;
		case CSelectModePenguin::MOTION_STOMACH:
			pPenguin->SetState(new CSelectModePenguinState_Stomach);
			break;
		case CSelectModePenguin::MOTION_UNYO:
			pPenguin->SetState(new CSelectModePenguinState_UNYO);
			break;
		case CSelectModePenguin::MOTION_WINGPTPT:
			pPenguin->SetState(new CSelectModePenguinState_WingPTPT);
			break;
		default:
			m_nCounter = 0;
			break;
		}
	}
}

//*****************************************************
// 移動ステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_Move::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_Move::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_Move::Update(CSelectModePenguin* pPenguin)
{
	// 移動
	D3DXVECTOR3 rot = pPenguin->GetRotation();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	move.x = -sinf(rot.y) * Move::MOVE_SPEED;
	move.z = -cosf(rot.y) * Move::MOVE_SPEED;

	pPenguin->Translate(move);

	// 回る
	rot.y += 0.005f * D3DX_PI;
	pPenguin->SetRotation(rot);

	// ステート終了カウント
	m_nCounter++;
	if (m_nCounter >= Move::MOTION_COUNT)
	{// ステート時間終了
		// コケチャンス
		int rnd = rand() % 100;
		if (rnd < Move::FALL_CHANCE)
		{// コケ
			pPenguin->SetState(new CSelectModePenguinState_Fall);
		}
		else
		{// 普通に立つ
			pPenguin->SetState(new CSelectModePenguinState_Stand);
		}
	}
}

//*****************************************************
// 転びステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_Fall::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_FALL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_Fall::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_Fall::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// コケ終わった
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// 首振りステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_ShakeHead::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_NECKSHAKE);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_ShakeHead::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_ShakeHead::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= ShakeHead::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// 腹ベタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_Stomach::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_STOMACH);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_Stomach::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_Stomach::Update(CSelectModePenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stomach::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// 首うねステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_UNYO::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_UNYO);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_UNYO::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_UNYO::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 首うにょん終わった
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}

//*****************************************************
// 羽パタパタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectModePenguinState_WingPTPT::Init(CSelectModePenguin* pPenguin)
{
	pPenguin->SetMotion(CSelectModePenguin::MOTION::MOTION_WINGPTPT);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectModePenguinState_WingPTPT::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguinState_WingPTPT::Update(CSelectModePenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 羽パタ終わった
		pPenguin->SetState(new CSelectModePenguinState_Stand);
	}
}
