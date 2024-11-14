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

//*****************************************************
// マクロ定義
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\BG\\field00.jpg"				// テクスチャファイル名
#define SPLIT_TEX					(10)										// テクスチャ分割数
#define CHENGE_LENGTH	(10000)	// 操作できる頂点までの距離
#define ANGLE_SLIP	(0.7f)	// 坂を滑る角度
#define CMP_LENGTH	(1000.0f)	// 判定する半径
#define MAX_ALBATROSS	(2)										// アホウドリ最大数
namespace
{
	const int OCEAN_ROT_CHANGE_TIME_DEFAULT = 10;	// デフォルトの海流向き変更時間
	const int OCEAN_ROT_CHANGE_TIME_DEGREE = 10;	// 海流向き変更時間ぶれ幅
}

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
	m_bRandState = false;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSetRotTime = 0;
	m_nExecRotChangeTime = 0;
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

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
	{ // 流氷マネージャーが NULL じゃない場合

		// 保存用変数に現在の海流を設定する
		m_nRandKeep = pIceManager->GetDirStream();
	}

	SetNextOceanRot();	// 最初に次の向き設定

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
		m_nRandNextKeep = universal::RandRange(CIceManager::E_Stream::STREAM_MAX, CIceManager::E_Stream::STREAM_UP);
	} while (m_nRandKeep == m_nRandNextKeep);	// 次の向きが変わるまで乱数を回す

	// 変更時間設定
	m_nExecRotChangeTime = OCEAN_ROT_CHANGE_TIME_DEFAULT + universal::RandRange(OCEAN_ROT_CHANGE_TIME_DEGREE, 0);
	m_nSetRotTime = pGame->GetTimeSecond();	 // 現在のタイムを取得
}

//====================================================
// 海流の向き変更時間か確認処理
//====================================================
void COcean::OceanChangeCheck(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();
	CGame* pGame = CGame::GetInstance();

	if (pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	CIceManager::E_Stream OceanFlow = pIceManager->GetDirStreamNext();

	int nNowTime = pGame->GetTimeSecond();	 // 現在のタイムを取得

	if (m_nSetRotTime - nNowTime >= m_nExecRotChangeTime)
	{// 変更時間になった
		pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// 海流の速度を下げる
		pIceManager->SetDirStreamNext((CIceManager::E_Stream)(m_nRandNextKeep));	// 海流の向きをランダムにする
		m_nRandKeep = m_nRandNextKeep;	// 現在の向きに設定
		SetNextOceanRot();	// 次の向き設定
		m_bRandState = false;	// ランダムの状態を false にする
	}

	if (m_nSetRotTime - nNowTime + 3 == m_nExecRotChangeTime)
	{
		if (m_bRandState == false)
		{
			for (int nCnt = 0; nCnt < MAX_ALBATROSS; nCnt++)
			{
				// アホウドリ生成
				CAlbatross::Create((CIceManager::E_Stream)(m_nRandNextKeep));
			}

			m_bRandState = true;
		}
	}
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
		if (m_bRandState == false)
		{
			m_bRandState = true;

			m_nRandKeep = m_nRandNextKeep;
		}

		// 現状と次の向きが同じとき数値をリセット
		if (m_nRandKeep == m_nRandNextKeep)
		{
			m_nRandNextKeep = universal::RandRange(CIceManager::E_Stream::STREAM_MAX, CIceManager::E_Stream::STREAM_UP);
		}

		// 現状と次の向きが同じじゃないとき
		if (m_nRandKeep != m_nRandNextKeep)
		{
			pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// 海流の速度を下げる
			pIceManager->SetDirStreamNext((CIceManager::E_Stream)(m_nRandNextKeep));	// 海流の向きをランダムにする
		}
	}
}
