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

//*****************************************************
// マクロ定義
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// 項目の幅
#define RESULT_HEIGHT	(0.05f)	// 項目の高さ
#define MOVE_FACT	(0.15f)	// 移動速度
#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	//const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// クリアロゴのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
COceanFlowUI* COceanFlowUI::m_pOceanFlowUI = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
COceanFlowUI::COceanFlowUI()
{
	m_state = STATE_NONE;
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
	if (m_pOceanFlowUI == nullptr)
	{
		m_pOceanFlowUI = new COceanFlowUI;

		if (m_pOceanFlowUI != nullptr)
		{
			m_pOceanFlowUI->Init();
		}
	}

	return m_pOceanFlowUI;
}

//====================================================
// 初期化処理
//====================================================
HRESULT COceanFlowUI::Init(void)
{
	m_pArrow = CObjectX::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//	矢印モデルの初期化
	if (m_pArrow != nullptr)
	{
		m_pArrow->Init();

		m_pArrow->BindModel(CModel::Load("data\\MODEL\\other\\Arrow001.x"));
	}

	CIceManager::GetInstance()->GetDirStream();

	m_state = STATE_IN;

	return S_OK;
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

	m_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void COceanFlowUI::Update(void)
{
	OceanFlowKeep = CIceManager::GetInstance()->GetDirStream();
	D3DXVECTOR3 Rot = m_pArrow->GetRotation();

	if (OceanFlowKeep == CIceManager::STREAM_UP)
	{
		universal::FactingRot(&Rot.y, D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_RIGHT)
	{
		universal::FactingRot(&Rot.y, D3DX_PI, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_DOWN)
	{
		universal::FactingRot(&Rot.y, -D3DX_PI * 0.5f, 0.02f);
	}

	if (OceanFlowKeep == CIceManager::STREAM_LEFT)
	{
		universal::FactingRot(&Rot.y, 0.0f, 0.02f);
	}

	m_pArrow->SetRotation(Rot);

	// 状態管理
	OceanState();
}

//====================================================
// 状態管理
//====================================================
void COceanFlowUI::OceanState(void)
{

}

//====================================================
// 描画処理
//====================================================
void COceanFlowUI::Draw(void)
{

}