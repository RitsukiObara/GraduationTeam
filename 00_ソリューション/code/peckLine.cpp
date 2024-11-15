//*****************************************************
//
// つっつき線の処理[peckLine.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "peckLine.h"
#include "player.h"
#include "object3D.h"
#include "orbit.h"
#include "manager.h"
#include "effect3D.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
//--------------------------
// 軌跡の定数
//--------------------------
namespace orbit
{
const D3DXVECTOR3 OFFSET_LEFT	= { 10.0f,0.0f,0.0f };				// 左側のオフセット
const D3DXVECTOR3 OFFSET_RIGHT	= { -10.0f,0.0f,0.0f };				// 右側のオフセット
const int NUM_EDGE				= 40;								// 辺の数
const float MOVE_TIME			= 0.2f;								// 移動にかかる時間
const float WAIT_TIME			= 0.7f;								// 待機時間
const float FACT_PARABOLA		= 10.0f;							// 放物線の係数
const D3DXCOLOR COL_INIT		= D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);	// 軌跡の色
const float HEIGHT_PARABOLA		= 100.0f;							// 放物線の高さ
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPeckLine::FuncUpdateState CPeckLine::s_aFuncUpdateState[] =
{
	nullptr,				// 何もしていない状態の更新
	&CPeckLine::UpdateMove,	// 動いてる状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CPeckLine::CPeckLine() : m_pPlayer(nullptr), m_pPosOrbit(nullptr),m_pOrbit(nullptr), m_fTimer(0.0f), m_posDest(), m_state(E_State::STATE_NONE), m_fTimerWait(0.0f)
{

}

//====================================================
// デストラクタ
//====================================================
CPeckLine::~CPeckLine()
{

}

//====================================================
// 生成処理
//====================================================
CPeckLine *CPeckLine::Create(CPlayer *pPlayer)
{
	CPeckLine *pPeckLine = nullptr;

	pPeckLine = new CPeckLine;

	if (pPeckLine != nullptr)
	{
		pPeckLine->m_pPlayer = pPlayer;
		pPeckLine->Init();
	}

	return pPeckLine;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CPeckLine::Init(void)
{
	assert(m_pPlayer != nullptr);	// プレイヤー渡し忘れ用

	// オブジェクトの生成
	CreateObject();

	return S_OK;
}

//====================================================
// オブジェクト生成
//====================================================
void CPeckLine::CreateObject(void)
{
	//--------------------------
	// 軌跡先端用3Dオブジェクトの生成
	//--------------------------
	m_pPosOrbit = CObject3D::Create();

	if (m_pPosOrbit == nullptr)
		return;

	//--------------------------
	// 軌跡の生成
	//--------------------------
	D3DXMATRIX mtx = m_pPosOrbit->GetMatrix();
	m_pOrbit = COrbit::Create(mtx, orbit::OFFSET_LEFT, orbit::OFFSET_RIGHT, orbit::NUM_EDGE);

	if (m_pOrbit == nullptr)
		return;

	//m_pOrbit->EnableZtest(true);
}

//====================================================
// 終了処理
//====================================================
void CPeckLine::Uninit(void)
{
	Release();
}

//====================================================
// 更新処理
//====================================================
void CPeckLine::Update(void)
{
	// 状態ごとの更新
	if(s_aFuncUpdateState[m_state] != nullptr)
		(this->*(s_aFuncUpdateState[m_state]))();
}

//====================================================
// 移動開始
//====================================================
void CPeckLine::StartMove(D3DXVECTOR3 posDest)
{
	// 移動状態に設定
	m_state = E_State::STATE_MOVE;

	// 目標位置を設定
	m_posDest = posDest;

	if (m_pPosOrbit != nullptr)
	{
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posDest - posPlayer;

		float fRot = atan2f(vecDiff.x, vecDiff.z);

		m_pPosOrbit->SetRotation(D3DXVECTOR3(0.0f, fRot, 0.0f));
	}
}

//====================================================
// 移動状態での更新
//====================================================
void CPeckLine::UpdateMove(void)
{
	m_fTimerWait += CManager::GetDeltaTime();

	// 軌跡の追従
	FollowOrbit();

	if (m_fTimerWait < orbit::WAIT_TIME)
		return;

	m_fTimer += CManager::GetDeltaTime();

	// タイマーの割合を計算
	float fRate = m_fTimer / orbit::MOVE_TIME;

	// オブジェクトの位置を設定
	D3DXVECTOR3 posOwner = m_pPlayer->GetPosition();
	D3DXVECTOR3 posObject = universal::Lerp(posOwner, m_posDest, fRate);

	// 高さに放物線を追加
	float fParabola = universal::ParabolaY(fRate - 0.5f, orbit::FACT_PARABOLA);
	fParabola *= -orbit::HEIGHT_PARABOLA; 
	fParabola += orbit::HEIGHT_PARABOLA;
	posObject.y += fParabola;

	m_pPosOrbit->SetPosition(posObject);

	// 一定時間経過で移動終了
	if (m_fTimer > orbit::MOVE_TIME)
	{
		m_fTimerWait = 0.0f;
		m_fTimer = 0.0f;
	}
}

//====================================================
// 軌跡の追従
//====================================================
void CPeckLine::FollowOrbit(void)
{
	if (m_pOrbit == nullptr || m_pPosOrbit == nullptr)
		return;

	D3DXMATRIX mtx = m_pPosOrbit->GetMatrix();

	m_pOrbit->SetOffset(mtx);
	m_pOrbit->SetColor(orbit::COL_INIT);
}

//====================================================
// 移動終了
//====================================================
void CPeckLine::EndMove(void)
{
	// 何もしてない状態に設定
	m_state = E_State::STATE_NONE;

	// タイマーリセット
	m_fTimer = 0.0f;
	m_fTimerWait = orbit::WAIT_TIME;
}

//====================================================
// 描画処理
//====================================================
void CPeckLine::Draw(void)
{

}