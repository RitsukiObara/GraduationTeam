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
	const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// クリアロゴのパス
	const char* FAIL_LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";	// 失敗ロゴのパス
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
	m_bSound = false;
	nCountMove = 0;
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
	//	矢印モデルの初期化
	if (m_apArrow != nullptr)
	{
		m_apArrow->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
		m_apArrow->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_apArrow->BindModel(CModel::Load("data\\MODEL\\other\\Arrow001.x"));
	}

	m_state = STATE_IN;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void COceanFlowUI::Uninit(void)
{
	// メニュー項目の破棄
	if (m_apArrow != nullptr)
	{
		Uninit();

		m_apArrow = nullptr;
	}

	m_pOceanFlowUI = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void COceanFlowUI::Update(void)
{
	// 状態管理
	ResultState();
}

//====================================================
// 状態管理
//====================================================
void COceanFlowUI::ResultState(void)
{

}

//====================================================
// 描画処理
//====================================================
void COceanFlowUI::Draw(void)
{

}