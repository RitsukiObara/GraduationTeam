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
#define MOVE_SPEED					(1.0f)										// 移動速度
#define MESH_LENGTH					(800.0f)									// メッシュの一辺の長さ
#define MESH_U						(254)											// 横のブロック数
#define MESH_V						(254)											// 縦のブロック数
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
	float OceanFlowLevel = CIceManager::GetInstance()->GetOceanLevel();	//	海流レベルの取得

	CMeshField::Update();
	//OceanCycleTimer();

	m_fSpeed += 0.007f * OceanFlowLevel;

	//OceanRotState();

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
	int OceanFlowKeep = CIceManager::GetInstance()->GetDirStream();
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
	float OceanCycleTimer = CGame::GetInstance()->GetTimeSecond();	 // 現在のタイムを取得

	if (OceanCycleTimer <= 111.0f &&
		OceanCycleTimer >= 110.0f)
	{
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		pIceManager->SetDirStreamNext(pIceManager->E_Stream::STREAM_DOWN);	// 海流の向きを下にする
		//universal::RandRange();
	}

	if (OceanCycleTimer <= 101.0f &&
		OceanCycleTimer >= 100.0f)
	{
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// 海流の速度を下げる
		pIceManager->SetDirStreamNext(pIceManager->E_Stream::STREAM_LEFT);	// 海流の向きを下にする
	}

}