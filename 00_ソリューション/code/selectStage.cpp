//*****************************************************
//
// ゲーム処理[SelectStage.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStage.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "skybox.h"
#include "selectStageManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const char* PATH_SelectStage_ROAD = "data\\MAP\\road00.bin";	// ゲームメッシュロードのパス
const int NUM_LIGHT = 3;	// ライトの数
const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ライトのデフォルト色
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ライトの色が変わる速度
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CSelectStage::CSelectStage()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStage::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	// ステージ選択管理クラスの生成
	CSelectStageManager::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStage::Uninit(void)
{
	// オブジェクト全棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStage::Update(void)
{
	// シーンの更新
	CScene::Update();

	// カメラ更新
	UpdateCamera();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// カメラの更新
//=====================================================
void CSelectStage::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// デバッグ処理
//=====================================================
void CSelectStage::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CSelectStage::Draw(void)
{

}