//*****************************************************
//
// オーシャン処理[ocean.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"
#include "ocean.h"
#include "iceManager.h"
#include "timer.h"
#include "universal.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\BG\\field00.jpg"				// テクスチャファイル名
#define SPLIT_TEX					(10)										// テクスチャ分割数
#define CHENGE_LENGTH	(10000)	// 操作できる頂点までの距離
#define ANGLE_SLIP	(0.7f)	// 坂を滑る角度
#define CMP_LENGTH	(1000.0f)	// 判定する半径

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
COcean::COcean()
{
	m_fSpeed = 0.0f;
	m_nRandKeep = 0;
	m_nRandNextKeep = 0;
	m_nRandState = false;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// デストラクタ
//=====================================================
COcean::~COcean()
{

}

//=====================================================
// 生成処理
//=====================================================
COcean* COcean::Create(void)
{
	if (m_pOcean == nullptr)
	{
		m_pOcean = new COcean;

		if (m_pOcean != nullptr)
		{
			m_pOcean->Init();
		}
	}

	return m_pOcean;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT COcean::Init(void)
{
	CMeshField::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void COcean::Uninit(void)
{
	m_pOcean = nullptr;

	CMeshField::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void COcean::Update(void)
{
	float OceanFlowLevel = 0.0f;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if(pIceMgr != nullptr)
		OceanFlowLevel = pIceMgr->GetOceanLevel();	//	海流レベルの取得

	CMeshField::Update();

	m_fSpeed += 0.007f * OceanFlowLevel;

	//OceanRotState();
	OceanCycleTimer();

	universal::LimitRot(&m_fSpeed);

	CMeshField::Wave(m_fSpeed);
}

//=====================================================
// 描画処理
//=====================================================
void COcean::Draw(void)
{
	CMeshField::Draw();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//====================================================
// 海流の向きとメッシュの向きを連動させる処理
//====================================================
void COcean::OceanRotState(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int OceanFlowKeep = pIceManager->GetDirStream();
	m_fRot = CMeshField::GetRotation();

	//	矢印が海流の向きに流れる処理
	if (OceanFlowKeep == CIceManager::STREAM_UP)
	{
		m_fRot.y = 0.0f;
	}

	if (OceanFlowKeep == CIceManager::STREAM_RIGHT)
	{
		m_fRot.y = D3DX_PI * 0.5f;
	}

	if (OceanFlowKeep == CIceManager::STREAM_DOWN)
	{
		m_fRot.y = D3DX_PI;
	}

	if (OceanFlowKeep == CIceManager::STREAM_LEFT)
	{
		m_fRot.y = -D3DX_PI * 0.5f;
	}

	CMeshField::SetRotation(m_fRot);
}

//====================================================
// 海流周期を時間で管理する処理
//====================================================
void COcean::OceanCycleTimer(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();

	CGame *pGame = CGame::GetInstance();

	if (pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	int OceanCycleTimer = pGame->GetTimeSecond();	 // 現在のタイムを取得

	// 10の倍数の時に入る
	if (OceanCycleTimer % 10 == 0)
	{
		if (m_nRandState == false)
		{
			m_nRandState = true;

			m_nRandKeep = m_nRandNextKeep;
		}

		// 現状と次の向きが同じとき数値をリセット
		if (m_nRandKeep == m_nRandNextKeep)
		{
			m_nRandNextKeep = universal::RandRange(pIceManager->E_Stream::STREAM_MAX, pIceManager->E_Stream::STREAM_UP);
		}

		// 現状と次の向きが同じじゃないとき
		if (m_nRandKeep != m_nRandNextKeep)
		{
			pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
			pIceManager->SetDirStreamNext((CIceManager::E_Stream)(m_nRandNextKeep));	// 海流の向きをランダムにする
		}
	}
}