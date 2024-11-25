//*****************************************************
//
// リザルトのアザラシ[resultseal.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultseal.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"
#include "shadow.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// ボディのパス
	const float SCALE_BODY = 1.8f;	// 体のスケール
	
	const float FACT_DECREASE_MOVE = 0.9f;	// 移動量の減衰係数
	const float ADD_MOVE = 4.0f;	// 移動の追加量
	
	const float RADIUS_COLLISION = 200.0f;	// 球の判定の半径

	const int MOVE_COUNT = 50;	// グイッと動くまでの時間
	const float ROTATE_SPEED = 0.008f * D3DX_PI;	// 回転量（これに慣性がかかる）
	const float ROTATE_COEF = 0.9f;				// 回転の慣性
}

//=====================================================
// コンストラクタ
//=====================================================
CResultSeal::CResultSeal(D3DXVECTOR3 pos, int nPriority) : CMotion(nPriority),c_centorPos(pos)
{
	m_pCollisionSphere = nullptr;
	m_nCounterMove = 0;
	m_bMove = false;
	m_fRotateSpeed = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CResultSeal::~CResultSeal()
{

}

//=====================================================
// 生成処理
//=====================================================
CResultSeal* CResultSeal::Create(D3DXVECTOR3 pos)
{
	CResultSeal *pNPCPenguin = nullptr;

	pNPCPenguin = new CResultSeal(pos);

	if (pNPCPenguin != nullptr)
	{
		pNPCPenguin->Init();
	}

	return pNPCPenguin;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CResultSeal::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// モーション初期設定
	SetMotion(MOTION_WALK);

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
void CResultSeal::Uninit(void)
{
	// 影削除
	Object::DeleteObject((CObject**)&m_pShadow);

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CResultSeal::Update(void)
{
	// モーション更新
	CMotion::Update();

	// 移動処理
	Move();

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
}

//=====================================================
// 移動処理
//=====================================================
void CResultSeal::Move(void)
{
	m_nCounterMove++;
	if (m_nCounterMove >= MOVE_COUNT)
	{
		m_nCounterMove = 0;
		m_fRotateSpeed = ROTATE_SPEED;
	}

	float length = D3DXVec3Length(&(c_centorPos - GetPosition()));
	D3DXVECTOR3 rot = GetRotation();

	// 移動向きを変える
	rot.y += m_fRotateSpeed;
	rot.y = fmodf(rot.y + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI;

	// 位置計算
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, GetPosition().y, 0.0f);
	pos.x = -sinf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.x;
	pos.z = -cosf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.z;

	// 位置向き設定
	SetPosition(pos);
	SetRotation(rot);

	// 回転に慣性
	m_fRotateSpeed = m_fRotateSpeed * ROTATE_COEF;
}

//=====================================================
// 描画処理
//=====================================================
void CResultSeal::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}
