//*****************************************************
//
// 背景氷の処理[BG_Ice.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "BG_Ice.h"
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
	const char* MODEL[CBgIce::TYPE_MAX] = { "data\\MODEL\\block\\Drift_ice.x","data\\MODEL\\block\\Drift_ice_small.x" };
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//====================================================
// コンストラクタ
//====================================================
CBgIce::CBgIce()
{
	m_type = TYPE_BIG;
}

//====================================================
// デストラクタ
//====================================================
CBgIce::~CBgIce()
{

}

//====================================================
// 生成処理
//====================================================
CBgIce* CBgIce::Create(D3DXVECTOR3 pos,TYPE type)
{
	CBgIce* pBgIce = new CBgIce;

	pBgIce->SetPosition(pos);

	pBgIce->Init();

	return pBgIce;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CBgIce::Init(void)
{
	CObjectX::Init();

	CObjectX::BindModel(CModel::Load(MODEL[m_type]));

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CBgIce::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CBgIce::Update(void)
{
	CObjectX::Update();
}

//====================================================
// 描画処理
//====================================================
void CBgIce::Draw(void)
{
	CObjectX::Draw();
}