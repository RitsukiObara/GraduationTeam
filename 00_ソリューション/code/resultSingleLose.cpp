//*****************************************************
//
// シングル負けリザルトの処理[resultSingleLose.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingleLose.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace scoreCaption
{
const string PATH_TEX[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// キャプションのパス
	"data\\TEXTURE\\UI\\Restart.png",
	"data\\TEXTURE\\UI\\Resume.png",
};
const float TIME_APPER = 2.0f;	// 出現にかかる時間
const float WIDTH = 0.16f;	// 幅
const float HEIGHT = 0.1f;	// 高さ
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.3f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

namespace scoreNumber
{
const float SIZE_INIT = 0.03f;	// サイズ
const float HEIGHT_INIT = -0.1f;
const D3DXVECTOR3 POS_INIT[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] =
{// 初期位置
	{ 0.2f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.4f;	// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}

namespace score
{
const int NUM_PLACE[CResultSingleLose::E_ScoreCaption::CAPTION_MAX] = { 5, 2 };	// スコアの桁数
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultSingleLose::FuncUpdateState CResultSingleLose::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,									// 何もしない更新
	nullptr,									// カメラ移動の更新
	nullptr,									// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingleLose::CResultSingleLose() : m_fTimer(0.0f)
{

}

//====================================================
// デストラクタ
//====================================================
CResultSingleLose::~CResultSingleLose()
{

}

//====================================================
// 初期化処理
//====================================================
HRESULT CResultSingleLose::Init(void)
{
	// 親クラスの初期化
	CResultSingle::Init();

	// 2Dオブジェクトの生成
	Create2D();

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultSingleLose::Create2D(void)
{

}

//====================================================
// 終了処理
//====================================================
void CResultSingleLose::Uninit(void)
{
	CResultSingle::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CResultSingleLose::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// 親クラスの更新
	CResultSingle::Update();
}

//====================================================
// 描画処理
//====================================================
void CResultSingleLose::Draw(void)
{
	// 親クラスの描画
	CResultSingle::Draw();
}