//*****************************************************
//
// ステージリザルトUIの処理[stageResultUI.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ocean_flow_UI.h"
#include "manager.h"
#include "objectX.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "UI.h"
#include "inputManager.h"
#include "iceManager.h"
#include "gameManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// 項目の幅
#define RESULT_HEIGHT	(0.05f)	// 項目の高さ
#define MOVE_FACT	(0.15f)	// 移動速度
#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値
#define MAX_OCEANLEVEL (7.00f)	// 海流最大レベル

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_SINGLE = { -1000.0f,200.0f,0.0f };	// シングルプレイ時の矢印の位置
const D3DXVECTOR3 POS_MULTI = { -1500.0f ,400.0f,700.0f };	// マルチプレイ時の矢印の位置

const string PATH_MODEL_SINGLE = "data\\MODEL\\other\\Arrow001.x";	// シングル時のモデルパス
const string PATH_MODEL_MULTI = "data\\MODEL\\other\\Arrow002.x";	// マルチ時のモデルパス

const string PATH_COMPASS_SINGLE = "data\\MODEL\\other\\Compass00.x";	// シングル時のコンパスモデルパス
const string PATH_COMPASS_MULTI = "data\\MODEL\\other\\Compass01.x";	// マルチ時のコンパスモデルパス

const float TIMEMAX_SHAKE = 5.0f;		// 矢印の揺れの最大時間
const float SPEED_SHAKE_INIT = 0.1f;	// 矢印の初期揺れ速度
const float MAGNITUDE_SHAKE = 100.0f;	// 揺れの規模
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
COceanFlowUI* COceanFlowUI::s_pOceanFlowUI = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
COceanFlowUI::COceanFlowUI()
{
	m_state = STATE_NONE;
	m_pDir = nullptr;
	m_pArrow = nullptr;
	m_pCompass = nullptr;
	m_fTimerShakeArrow = 0.0f;
	m_fSpeedShakeArrow = 0.0f;
}

//====================================================
// デストラクタ
//====================================================
COceanFlowUI::~COceanFlowUI()
{

}

//====================================================
// 生成処理
//====================================================
COceanFlowUI* COceanFlowUI::Create(void)
{
	if (s_pOceanFlowUI == nullptr)
	{
		s_pOceanFlowUI = new COceanFlowUI;

		if (s_pOceanFlowUI != nullptr)
		{
			s_pOceanFlowUI->Init();
		}
	}

	return s_pOceanFlowUI;
}

//====================================================
// 初期化処理
//====================================================
HRESULT COceanFlowUI::Init(void)
{
	// 方向用の空のオブジェクト生成
	CreateDir();

	// 矢印の生成
	CreateArrow();

	// コンパスの生成
	//CreateCompass();

	// 値の初期化
	m_state = STATE_IN;
	m_fSpeedShakeArrow = SPEED_SHAKE_INIT;

	return S_OK;
}

//====================================================
// 方向用の空のオブジェクト生成
//====================================================
void COceanFlowUI::CreateDir(void)
{
	if (m_pDir != nullptr)
		return;

	m_pDir = CObject3D::Create();

	// 位置の設定
	if (m_pDir != nullptr)
	{
		bool bMulti = gameManager::IsMulti();

		// モード毎の位置
		if (bMulti)
			m_pDir->SetPosition(POS_MULTI);
		else
			m_pDir->SetPosition(POS_SINGLE);
	}
}

//====================================================
// 矢印生成
//====================================================
void COceanFlowUI::CreateArrow(void)
{
	if (m_pArrow != nullptr)
		return;

	m_pArrow = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 矢印モデルの初期化
	if (m_pArrow != nullptr)
	{
		CGame::E_GameMode gamemode = CGame::GetInstance()->GetGameMode();

		// モード毎の矢印位置
		if (gamemode == CGame::MODE_SINGLE)
			m_pArrow->BindModel(CModel::Load(&PATH_MODEL_SINGLE[0]));
		else
			m_pArrow->BindModel(CModel::Load(&PATH_MODEL_MULTI[0]));
	}
}

//====================================================
// コンパス生成
//====================================================
void COceanFlowUI::CreateCompass(void)
{
	if (m_pCompass != nullptr)
		return;

	m_pCompass = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pCompass != nullptr)
	{// 初期化
		bool bMulti = gameManager::IsMulti();

		// モード毎の設定
		if (bMulti)
		{
			m_pCompass->SetPosition(POS_MULTI);
			m_pCompass->BindModel(CModel::Load(&PATH_COMPASS_MULTI[0]));
		}
		else
		{
			m_pCompass->SetPosition(POS_SINGLE);
			m_pCompass->BindModel(CModel::Load(&PATH_COMPASS_SINGLE[0]));
		}
	}
}

//====================================================
// 終了処理
//====================================================
void COceanFlowUI::Uninit(void)
{
	// メニュー項目の破棄
	if (m_pArrow != nullptr)
	{
		m_pArrow->Uninit();
		m_pArrow = nullptr;
	}

	s_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void COceanFlowUI::Update(void)
{
	// 海流の向き補正処理
	OceanRotState();

	// 海流レベル状態管理
	OceanLevelState();
}

//====================================================
// 海流の向き補正処理
//====================================================
void COceanFlowUI::OceanRotState(void)
{
	if (m_pDir == nullptr)
		return;

	int OceanFlowKeep = CIceManager::GetInstance()->GetDirStreamNext();
	D3DXVECTOR3 Rot = m_pDir->GetRotation();

	//	矢印が海流の向きに流れる処理
	if (OceanFlowKeep == COcean::STREAM_UP)
	{
		universal::FactingRot(&Rot.y, D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_RIGHT)
	{
		universal::FactingRot(&Rot.y, D3DX_PI, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_DOWN)
	{
		universal::FactingRot(&Rot.y, -D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == COcean::STREAM_LEFT)
	{
		universal::FactingRot(&Rot.y, 0.0f, 0.02f);
	}

	m_pDir->SetRotation(Rot);
}

//====================================================
// 海流レベルのUI処理
//====================================================
void COceanFlowUI::OceanLevelState(void)
{
	if (m_pArrow == nullptr || m_pDir == nullptr)
		return;

	if (CIceManager::GetInstance() == nullptr)
		return;

	float OceanFlowLevel = CIceManager::GetInstance()->GetOceanLevel();	//	海流レベルの取得
	D3DXCOLOR fEmissiveCol = m_pArrow->GetDeffuseeCol(0);	//	海流UIの色取得

	float Colorrate = OceanFlowLevel / MAX_OCEANLEVEL;	// 海流最大レベルとの割合

	// 目標の色に遷移
	fEmissiveCol = D3DXCOLOR(0.8f + (0.2f * Colorrate), 0.8f - (0.6f * Colorrate), 0.2f, 1.0f);

	m_pArrow->SetDeffuseCol(fEmissiveCol,0);

	// 矢印の前後移動を追加
	ShakeArrow(Colorrate);

	// 矢印の親子付け
	D3DXMATRIX mtx = m_pDir->GetMatrix();
	m_pArrow->SetMatrixParent(mtx);
}

//====================================================
// 矢印の揺れ
//====================================================
void COceanFlowUI::ShakeArrow(float fRate)
{
	if (m_pArrow == nullptr)
		return;

	// サインカーブ計算
	float fSin = universal::CalcSinWave(m_fTimerShakeArrow, TIMEMAX_SHAKE);

	float fShake = MAGNITUDE_SHAKE * fSin;

	m_pArrow->SetPosition(D3DXVECTOR3(fShake, 0.0f, 0.0f));

	m_fTimerShakeArrow += m_fSpeedShakeArrow * fRate;

	// タイマーを範囲内に収める
	m_fTimerShakeArrow = std::fmod(m_fTimerShakeArrow, TIMEMAX_SHAKE);
}

//====================================================
// 描画処理
//====================================================
void COceanFlowUI::Draw(void)
{

}