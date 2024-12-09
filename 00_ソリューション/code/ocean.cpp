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
#include "albatross.h"
#include "BG_Ice.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float OCEAN_ROT_CHANGE_TIME_DEFAULT = 20.0f;	// デフォルトの海流向き変更時間
const int OCEAN_ROT_CHANGE_TIME_DEGREE = 5;			// 海流向き変更時間ぶれ幅
const float FLOW_LEVEL_MULTIPLY = 0.008f;			// 海流の速度の倍率
const float ALBATROSS_SPAWN_TIME = 3.0f;			// アホウドリ出現時間
const int MAX_ALBATROSS = 1;						// アホウドリ最大数
const int MAX_RANGE_LEFT = -1800;					// ランダムレンジ左最大数
const int MAX_RANGE_RIGHT = 1500;					// ランダムレンジ右最大数
const int MAX_RANGE_UP = 1200;						// ランダムレンジ上最大数
const int MAX_RANGE_DOWN = -1500;					// ランダムレンジ下最大数
const float Z_UP = 1500.0f;							// Z方向上方向
const float Z_DOWN = -1500.0f;						// Z方向下方向
const float X_LEFT = -2500.0f;						// X方向上方向
const float X_RIGHT = 2500.0f;						// X方向下方向
const int BGICE_CREATE_CNT = 40;					// 背景氷が生成されるフレーム数
const float OCEANLEVEL_DEFAULT = 3.0f;				// デフォルトの海流の強さ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
COcean::COcean(int nPriority) : CMeshField(nPriority)
{
	m_fSpeed = 0.0f;
	m_nRandKeep = 0;
	m_nRandNextKeep = 0;
	m_bRandState = false;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fProgressTime = 0.0f;
	m_fRotChangeTime = 0.0f;
	m_nBgiceCnt = 0;
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
	m_nBgiceCnt = 0;
	
	CMeshField::Init();

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
	{ // 流氷マネージャーが NULL じゃない場合

		// 保存用変数に現在の海流を設定する
		m_nRandKeep = pIceManager->GetDirStream();
	}

	SetNextOceanRot();	// 最初に次の向き設定

	// 法線のリセット処理
	CMeshField::ResetNormal();

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
	float OceanFlowLevel = OCEANLEVEL_DEFAULT;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if(pIceMgr != nullptr)
		OceanFlowLevel = pIceMgr->GetOceanLevel();	//	海流レベルの取得

	CMeshField::Update();

	m_fSpeed += FLOW_LEVEL_MULTIPLY * OceanFlowLevel;

	BgIceRotState();
	//OceanCycleTimer();
	OceanChangeCheck();

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
// 次の海流の向き設定処理
//====================================================
void COcean::SetNextOceanRot(void)
{
	CGame* pGame = CGame::GetInstance();
	if (pGame == nullptr)
		return;

	// 向き変更
	do
	{
		m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
	} while (m_nRandKeep == m_nRandNextKeep);	// 次の向きが変わるまで乱数を回す

	// 変更時間設定
	m_fRotChangeTime = OCEAN_ROT_CHANGE_TIME_DEFAULT + (float)universal::RandRange(OCEAN_ROT_CHANGE_TIME_DEGREE, 0);
	m_fProgressTime = 0.0f;
}

//====================================================
// 海流の向き変更時間か確認処理
//====================================================
void COcean::OceanChangeCheck(void)
{
	CManager* pManager = CManager::GetInstance();
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();
	CGame* pGame = CGame::GetInstance();

	if (pManager == nullptr || pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	COcean::E_Stream OceanFlow = pIceManager->GetDirStreamNext();

	m_fProgressTime += pManager->GetDeltaTime();
	if (m_fProgressTime >= m_fRotChangeTime)
	{// 変更時間になった
		pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// 海流の速度を下げる
		pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// 海流の向きをランダムにする
		m_nRandKeep = m_nRandNextKeep;	// 現在の向きに設定
		SetNextOceanRot();	// 次の向き設定
		m_bRandState = false;	// ランダムの状態を false にする
	}

	if (m_fProgressTime >= m_fRotChangeTime - ALBATROSS_SPAWN_TIME)
	{
		if (m_bRandState == false)
		{
			for (int nCnt = 0; nCnt < MAX_ALBATROSS; nCnt++)
			{
				// アホウドリ生成
				CAlbatross::Create((COcean::E_Stream)(m_nRandNextKeep));
			}

			m_bRandState = true;
		}
	}
}

//====================================================
// 海流の向きとメッシュの向きを連動させる処理
//====================================================
void COcean::BgIceRotState(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int OceanFlowKeep = pIceManager->GetDirStream();
	m_fRot = CMeshField::GetRotation();

	//	矢印が海流の向きに流れる処理
	if (OceanFlowKeep == COcean::STREAM_UP)
	{
		//下から背景氷を出す処理
		BgIceSetPosDown();
	}

	if (OceanFlowKeep == COcean::STREAM_DOWN)
	{
		//上から背景氷を出す処理
		BgIceSetPosUp();
	}

	if (OceanFlowKeep == COcean::STREAM_LEFT)
	{
		//下から背景氷を出す処理
		BgIceSetPosR();
	}

	if (OceanFlowKeep == COcean::STREAM_RIGHT)
	{
		//上から背景氷を出す処理
		BgIceSetPosL();
	}
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
		if (m_bRandState == false)
		{
			m_bRandState = true;

			m_nRandKeep = m_nRandNextKeep;
		}

		// 現状と次の向きが同じとき数値をリセット
		if (m_nRandKeep == m_nRandNextKeep)
		{
			m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
		}

		// 現状と次の向きが同じじゃないとき
		if (m_nRandKeep != m_nRandNextKeep)
		{
			pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// 海流の速度を下げる
			pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// 海流の向きをランダムにする
		}
	}
}

//====================================================
// 上方向から背景氷が出てくる処理
//====================================================
void COcean::BgIceSetPosUp(void)
{
	float posX = (float)universal::RandRange(MAX_RANGE_LEFT, MAX_RANGE_RIGHT);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT)
	{
		// 背景氷のロード
		CBgIce::Create(D3DXVECTOR3(posX, 0.0f, Z_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// 下方向から背景氷が出てくる処理
//====================================================
void COcean::BgIceSetPosDown(void)
{
	float posX = (float)universal::RandRange(MAX_RANGE_LEFT, MAX_RANGE_RIGHT);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT)
	{
		// 背景氷のロード
		CBgIce::Create(D3DXVECTOR3(posX, 0.0f, Z_DOWN), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// 左方向から背景氷が出てくる処理
//====================================================
void COcean::BgIceSetPosL(void)
{
	float posZ = (float)universal::RandRange(MAX_RANGE_UP, MAX_RANGE_DOWN);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT)
	{
		// 背景氷のロード
		CBgIce::Create(D3DXVECTOR3(X_LEFT, 0.0f, posZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// 右方向から背景氷が出てくる処理
//====================================================
void COcean::BgIceSetPosR(void)
{
	float posZ = (float)universal::RandRange(MAX_RANGE_UP, MAX_RANGE_DOWN);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT)
	{
		// 背景氷のロード
		CBgIce::Create(D3DXVECTOR3(X_RIGHT, 0.0f, posZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

