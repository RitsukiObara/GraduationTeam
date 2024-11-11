//*****************************************************
//
// ステージ選択ペンギン[slectStagePenguin.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStagePenguin.h"
#include "inputManager.h"
#include "collision.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
const float SCALE_BODY = 2.5f;	// 体のスケール

const float FACT_DECREASE_MOVE = 0.9f;	// 移動量の減衰係数
const float ADD_MOVE = 4.0f;	// 移動の追加量

const float RADIUS_COLLISION = 200.0f;	// 球の判定の半径
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pInputMgr(nullptr), m_pClsnSphere(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CSelectStagePenguin::~CSelectStagePenguin()
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectStagePenguin *CSelectStagePenguin::Create(void)
{
	CSelectStagePenguin *pSelectStagePenguin = nullptr;

	pSelectStagePenguin = new CSelectStagePenguin;

	if (pSelectStagePenguin != nullptr)
	{
		pSelectStagePenguin->Init();
	}

	return pSelectStagePenguin;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStagePenguin::Init(void)
{
	// 入力マネージャー生成
	m_pInputMgr = CInputManager::Create();

	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// スケールの設定
	SetScale(SCALE_BODY);

	// 球の判定生成
	m_pClsnSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pClsnSphere != nullptr)
	{
		m_pClsnSphere->SetRadius(RADIUS_COLLISION);
		m_pClsnSphere->SetPosition(GetPosition());
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStagePenguin::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStagePenguin::Update(void)
{
	// 入力処理
	Input();

	// 移動量分移動
	AddPosition(m_move);

	// 移動量の減衰
	m_move *= FACT_DECREASE_MOVE;

	if (m_pClsnSphere != nullptr)
	{// 球の判定の追従
		D3DXVECTOR3 pos = GetPosition();

		m_pClsnSphere->SetPosition(pos);

		// ブロック判定
		m_pClsnSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);

		// キャラの位置反映
		SetPosition(pos);
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 入力
//=====================================================
void CSelectStagePenguin::Input(void)
{
	// 移動入力
	// 目標方向の設定
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	D3DXVec3Normalize(&axisMove, &axisMove);
	m_move += axisMove * ADD_MOVE;
}

//=====================================================
// デバッグ処理
//=====================================================
void CSelectStagePenguin::Debug(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}