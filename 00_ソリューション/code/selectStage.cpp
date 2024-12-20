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
#include "ice.h"
#include "sound.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	const char* BG_ICE = { "data\\TEXT\\selectstageIce.txt" };		// 氷配置テキスト
	const float RADIUS_ICE = 400.0f;	// 氷の半径
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

	// 氷の読み込み
	Load(BG_ICE);

	// BGM再生
	Sound::Play(CSound::LABEL::LABEL_BGM_SELECTSTAGE);
	
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
		return;

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

//=====================================================
// 読込処理
//=====================================================
void CSelectStage::Load(const char* pPath)
{
	//変数宣言
	char cTemp[MAX_STRING];

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	//ファイルから読み込む
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while (true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SET") == 0)
			{//キースタート
				while (strcmp(cTemp, "END_SET") != 0)
				{//終わりまでキー設定

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//位置取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						{
							(void)fscanf(pFile, "%f", &pos[nCntPos]);
						}
					}

					if (strcmp(cTemp, "ROT") == 0)
					{//向き取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntRot = 0; nCntRot < 3; nCntRot++)
						{
							(void)fscanf(pFile, "%f", &rot[nCntRot]);
						}
					}
				}

				// 氷の生成
				CIce* pIce = CIce::Create(CIce::TYPE_NORMAL, CIce::STATE_NORMAL);
				if (pIce == nullptr)
					return;

				// 位置セット
				pIce->SetPosition(pos);

				// 向きセット
				pIce->SetRotation(rot);

				// サイズセット
				pIce->SetTransform(RADIUS_ICE);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
				break;
		}//while
	}//file
	else
	{
		assert(("SelectStageIce読み込みに失敗", false));
	}

	fclose(pFile);
}