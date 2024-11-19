//*****************************************************
//
// ゲームマネージャー[gameManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gameManager.h"
#include "UIManager.h"
#include "timer.h"
#include "ocean.h"
#include "BG_Ice.h"
#include "camera.h"
#include "cameraState.h"
#include "iceManager.h"
#include "UI_ready.h"
#include "ocean_flow_UI.h"
#include "flowIceFactory.h"
#include "gameManagerSingle.h"
#include "gameManagerMulti.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEMP\\mode.txt";	// モードのパス
const string PATH_IDXMAP = "data\\TEMP\\mapIdx.bin";	// マップ番号のパス
}

//=====================================================
// コンストラクタ
//=====================================================
CGameManager::CGameManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CGameManager *CGameManager::Create(CGame::E_GameMode mode)
{
	CGameManager* pGameManager = nullptr;

	switch (mode)
	{
	case CGame::MODE_SINGLE:
		pGameManager = new CGameManagerSingle;
		break;
	case CGame::MODE_MULTI:
		pGameManager = new CGameManagerMulti;
		break;
	default:
		assert(false);
		break;
	}

	if (pGameManager == nullptr)
		return nullptr;

	pGameManager->Init();

	return pGameManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGameManager::Init(void)
{
	// UIマネージャーの追加
	CUIManager::Create();

	//海の生成
	COcean::Create();

#if 0
	//ゲームスタート告知UI
	CUIready::Create();
#endif

	// カメラのステイト設定
	Camera::ChangeState(new CFollowPlayer);

	// 矢印モデルの生成
	COceanFlowUI::Create();

	// 流氷ファクトリーの生成
	CFlowIceFct::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGameManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CGameManager::Update(void)
{
	// ゲームの状態管理
	ManageGameState();
}

//=====================================================
// ゲームの状態管理
//=====================================================
void CGameManager::ManageGameState(void)
{
	CGame::E_State state = CGame::GetState();

	switch (state)
	{
	case CGame::STATE_READY:

		break;
	case CGame::STATE_START:
		UpdateStart();
		break;
	case CGame::STATE_NORMAL:
		UpdateNormal();
		break;
	case CGame::STATE_RESULT:
		UpdateResult();
		break;
	case CGame::STATE_END:
		UpdateEnd();
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================================
// 開始状態の更新
//=====================================================
void CGameManager::UpdateStart(void)
{

}

//=====================================================
// 通常状態の更新
//=====================================================
void CGameManager::UpdateNormal(void)
{

}

//=====================================================
// リザルト状態の更新
//=====================================================
void CGameManager::UpdateResult(void)
{

}

//=====================================================
// 終了状態の更新
//=====================================================
void CGameManager::UpdateEnd(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CGameManager::Draw(void)
{

}

namespace gameManager
{
// モード保存
void SaveMode(CGame::E_GameMode mode, vector<bool> abPlayerEnter)
{
	std::ofstream file(PATH_TEX);

	if (file.is_open())
	{
		file << "MODE = " << (int)mode << '\n';			// モード

		for(bool bEnter : abPlayerEnter)
			file << "PLAYER = " << bEnter << '\n';	// エンターフラグ

		file.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}
}

// モード読込
void LoadMode(CGame::E_GameMode *pMode, vector<bool> &rbPlayerEnter)
{
	std::ifstream file(PATH_TEX);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "MODE")
			{// モード
				int nMode;
				iss >> temp >> nMode;
				*pMode = (CGame::E_GameMode)nMode;
			}

			if (key == "PLAYER")
			{// 人数
				int nData;
				iss >> temp >> nData;

				rbPlayerEnter.push_back(nData);
			}

			if (file.eof())
			{// 読み込み終了
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}
}

// マップ番号保存
void SaveIdxMap(int nIdx)
{
	// ファイルを開く
	std::ofstream outputFile(PATH_IDXMAP, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// データの保存
	outputFile.write(reinterpret_cast<char*>(&nIdx), sizeof(int));

	outputFile.close();
}

// マップ番号読込
int LoadIdxMap(void)
{
	// ファイルを開く
	std::ifstream inputFile(PATH_IDXMAP, std::ios::binary);

	if (!inputFile.is_open())
		assert(false);

	int nIdx;

	inputFile.read(reinterpret_cast<char*>(&nIdx), sizeof(int));

	inputFile.close();

	return nIdx;
}
}