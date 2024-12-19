//*****************************************************
//
// ステージ選択ペンギン[slectStagePenguin.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStagePenguin.h"
#include "inputManager.h"
#include "collision.h"
#include "debugproc.h"
#include "ice.h"
#include "MyEffekseer.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
const float SCALE_BODY = 2.5f;		// 体のスケール
const float RADIUS_ICE = 400.0f;	// 氷の半径
const D3DXVECTOR3 POS_INIT = { 0.0f,50.0f,0.0f };	// 初期位置

const float FACT_DECREASE_MOVE = 0.9f;	// 移動量の減衰係数
const float ADD_MOVE = 4.0f;	// 移動の追加量

const float RADIUS_COLLISION = 200.0f;	// 球の判定の半径

const float FACT_ROT = 0.2f;	// 回転係数

const int RIPPLE_CNT = 15;		// 波紋出現秒数

const float LINE_MOVE = 0.9f;	// 移動してる判断のしきい値
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_move(), m_pInputMgr(nullptr), m_pClsnSphere(nullptr), m_bInput(false), m_pIce(nullptr), m_nRippleCnt(0)
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

	// スケールの設定
	SetScale(SCALE_BODY);

	// 初期位置設定
	SetPosition(POS_INIT);

	// 球の判定生成
	m_pClsnSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pClsnSphere != nullptr)
	{
		m_pClsnSphere->SetRadius(RADIUS_COLLISION);
		m_pClsnSphere->SetPosition(GetPosition());
	}

	// 入力フラグを立てておく
	m_bInput = true;

	// 足場の氷の生成
	m_pIce = CIce::Create();
	if (m_pIce != nullptr)
		m_pIce->SetTransform(RADIUS_ICE);

	// 波紋が出るまでのカウント
	m_nRippleCnt = 0;

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
	if(m_bInput)
		Input();	// 入力処理

	// 移動量分移動
	Translate(m_move);

	// 移動量の減衰
	m_move *= FACT_DECREASE_MOVE;

	if (m_pClsnSphere != nullptr)
	{// 球の判定の追従
		D3DXVECTOR3 pos = GetPosition();

		m_pClsnSphere->SetPosition(pos);

		// ブロック判定
		m_pClsnSphere->PushCollision(&pos, CCollision::TAG::TAG_BLOCK);
		
		pos.y = POS_INIT.y;

		// キャラの位置反映
		SetPosition(pos);
	}

	D3DXVECTOR3 rot = GetRotation();
	float fSpeed = D3DXVec3Length(&m_move);
	if (fSpeed > LINE_MOVE)
	{
		// 目標の向きに補正する
		float fRotDest = atan2f(-m_move.x, -m_move.z);

		universal::FactingRot(&rot.y, fRotDest, FACT_ROT);
		SetRotation(rot);
	}

	if (m_pIce != nullptr)
	{// 氷の追従
		D3DXVECTOR3 posIce = m_pIce->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		m_pIce->SetPosition(D3DXVECTOR3(pos.x + m_move.x, posIce.y, pos.z + m_move.z));
		m_pIce->SetRotation(rot);

		// 高さを氷と同じにする
		SetPosition(D3DXVECTOR3(pos.x, posIce.y, pos.z));
	}

	// 波紋出現カウントを加算
	m_nRippleCnt++;

	if (m_nRippleCnt >= RIPPLE_CNT)
	{
		// プレイヤーに合わせて波紋を出す
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE_RIPPLE, m_pIce->GetPosition(), m_pIce->GetRotation(), D3DXVECTOR3(100.0f, 100.0f, 100.0f));

		m_nRippleCnt = 0;
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
// エンター入力の検出
//=====================================================
bool CSelectStagePenguin::IsEnter(void)
{
	if (m_pInputMgr == nullptr)
		return false;

	return m_pInputMgr->GetTrigger(CInputManager::BUTTON_ENTER);
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