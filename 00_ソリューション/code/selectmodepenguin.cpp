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
CSelectModePenguin::CSelectModePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pClsnSphere(nullptr)
{

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
CSelectModePenguin* CSelectModePenguin::Create(void)
{
	CSelectModePenguin *pSelectStagePenguin = nullptr;

	pSelectStagePenguin = new CSelectModePenguin;

	if (pSelectStagePenguin != nullptr)
	{
		pSelectStagePenguin->Init();
	}

	return pSelectStagePenguin;
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
void CSelectModePenguin::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectModePenguin::Update(void)
{
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
// デバッグ処理
//=====================================================
void CSelectModePenguin::Debug(void)
{
	CDebugProc::GetInstance()->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}

//=====================================================
// 描画処理
//=====================================================
void CSelectModePenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}