//*****************************************************
//
// ステージ選択ペンギン[slectStagePenguin.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStagePenguin.h"
#include "inputManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
const float SCALE_BODY = 2.5f;	// 体のスケール
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_pInputMgr(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CSelectStagePenguin::~CSelectStagePenguin()
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectStagePenguin *CSelectStagePenguin::Create(void)
{
	CSelectStagePenguin *pSelectStagePenguin = nullptr;

	pSelectStagePenguin = new CSelectStagePenguin;

	if (pSelectStagePenguin != nullptr)
	{
		pSelectStagePenguin->Init();
	}

	return pSelectStagePenguin;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStagePenguin::Init(void)
{
	// 入力マネージャー生成
	m_pInputMgr = CInputManager::Create();

	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// スケールの設定
	SetScale(SCALE_BODY);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStagePenguin::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStagePenguin::Update(void)
{
	// 入力処理
	Input();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 入力
//=====================================================
void CSelectStagePenguin::Input(void)
{

}

//=====================================================
// デバッグ処理
//=====================================================
void CSelectStagePenguin::Debug(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}