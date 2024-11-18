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
#include "peckWave.h"
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
const int NUM_EDGE				= 20;								// 辺の数
const float MOVE_TIME			= 0.05f;							// 移動にかかる時間
const float WAIT_TIME			= 0.5f;								// 待機時間
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
CPeckLine::CPeckLine() : m_posDest(), m_state(E_State::STATE_NONE), m_fTimerWait(0.0f), m_col()
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
CPeckLine *CPeckLine::Create(D3DXCOLOR col, D3DXVECTOR3 pos)
{
	CPeckLine *pPeckLine = nullptr;

	pPeckLine = new CPeckLine;

	if (pPeckLine != nullptr)
	{
		pPeckLine->SetPosition(pos);
		pPeckLine->Init();
		pPeckLine->m_col = col;
	}

	return pPeckLine;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CPeckLine::Init(void)
{
	return S_OK;
}

//====================================================
// 線の生成
//====================================================
void CPeckLine::CreateLine(void)
{
	S_InfoLine *pInfo = new S_InfoLine;
	if (pInfo == nullptr)
		return;

	pInfo->posDest = m_posDest;
	pInfo->posInit = GetPosition();

	//---------------------------------
	// 軌跡先端用3Dオブジェクトの生成
	//---------------------------------
	pInfo->pPosOrbit = CObject3D::Create();

	if (pInfo->pPosOrbit == nullptr)
		return;

	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 vecDiff = m_posDest - posPlayer;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	pInfo->pPosOrbit->SetRotation(D3DXVECTOR3(0.0f, fRot, 0.0f));
	pInfo->pPosOrbit->SetPosition(posPlayer);

	pInfo->pPosOrbit->Draw();

	//---------------------------------
	// 軌跡の生成
	//---------------------------------
	D3DXMATRIX mtx = pInfo->pPosOrbit->GetMatrix();
	pInfo->pOrbit = COrbit::Create(mtx, orbit::OFFSET_LEFT, orbit::OFFSET_RIGHT, orbit::NUM_EDGE);

	if (pInfo->pOrbit == nullptr)
		return;

	// プレイヤーのIDに合わせた色に設定
	pInfo->pOrbit->SetColor(m_col);

	// 配列に追加
	m_aInfoLine.push_back(pInfo);
}

//====================================================
// 終了処理
//====================================================
void CPeckLine::Uninit(void)
{
	for (S_InfoLine *pInfo : m_aInfoLine)
	{
		// 自身の終了
		pInfo->pPosOrbit->Uninit();
		pInfo->pPosOrbit = nullptr;

		pInfo->pOrbit->Uninit();
		pInfo->pOrbit = nullptr;
		
		delete pInfo;
	}

	m_aInfoLine.clear();

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

	// 全ての線の更新
	UpdateAllLine();
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
}

//====================================================
// 移動状態での更新
//====================================================
void CPeckLine::UpdateMove(void)
{
	m_fTimerWait += CManager::GetDeltaTime();

	if (m_fTimerWait > orbit::WAIT_TIME)
	{// 時間経過で線を生成
		CreateLine();

		m_fTimerWait = 0.0f;
	}
}

//====================================================
// 全ての線の更新
//====================================================
void CPeckLine::UpdateAllLine(void)
{
	for (auto it = m_aInfoLine.begin(); it != m_aInfoLine.end(); /* no increment here */)
	{
		S_InfoLine* pInfo = *it;

		if (pInfo == nullptr || pInfo->pOrbit == nullptr || pInfo->pPosOrbit == nullptr)
		{
			it = m_aInfoLine.erase(it); // 無効な要素をリストから削除
			continue;
		}

		//---------------------------------
		// 位置の更新
		//---------------------------------
		// 一定時間経過で移動終了
		if (pInfo->fTimer > orbit::MOVE_TIME)
		{
			pInfo->fTimer = 0.0f;

			// 波紋エフェクトの生成
			CPeckWave::Create(pInfo->posDest, pInfo->pOrbit->GetColor(0));

			// 軌跡を独立させる
			pInfo->pOrbit->SetEnd(true);

			// 自身の終了処理
			pInfo->pPosOrbit->Uninit();
			pInfo->pPosOrbit = nullptr;

			pInfo->pOrbit = nullptr;

			delete pInfo;
			pInfo = nullptr;

			it = m_aInfoLine.erase(it); // 削除した要素をリストから安全に削除
			continue;
		}

		pInfo->fTimer += CManager::GetDeltaTime();

		// タイマーの割合を計算
		float fRate = pInfo->fTimer / orbit::MOVE_TIME;

		// オブジェクトの位置を設定
		D3DXVECTOR3 posOwner = pInfo->posInit;
		D3DXVECTOR3 posObject = universal::Lerp(posOwner, pInfo->posDest, fRate);

		// 高さに放物線を追加
		float fParabola = universal::ParabolaY(fRate - 0.5f, orbit::FACT_PARABOLA);
		fParabola *= -orbit::HEIGHT_PARABOLA;
		fParabola += orbit::HEIGHT_PARABOLA * 2;
		posObject.y += fParabola;

		pInfo->pPosOrbit->SetPosition(posObject);

		//---------------------------------
		// 軌跡の追従
		//---------------------------------
		D3DXMATRIX mtx = pInfo->pPosOrbit->GetMatrix();
		pInfo->pOrbit->SetOffset(mtx);

		++it; // ループを進める
	}
}

//====================================================
// 移動終了
//====================================================
void CPeckLine::EndMove(void)
{
	// 何もしてない状態に設定
	m_state = E_State::STATE_NONE;

	// タイマーリセット
	m_fTimerWait = orbit::WAIT_TIME;
}

//====================================================
// 描画処理
//====================================================
void CPeckLine::Draw(void)
{

}