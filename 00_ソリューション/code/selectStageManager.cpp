//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStageManager.h"
#include "skybox.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";	// テキストパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CSelectStageManager::CSelectStageManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectStageManager *CSelectStageManager::Create(void)
{
	CSelectStageManager *pManager = nullptr;

	pManager = new CSelectStageManager;

	if (pManager != nullptr)
	{
		// 初期化処理
		pManager->Init();
	}

	return pManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStageManager::Init(void)
{
	// スカイボックスの生成
	CSkybox::Create();

	// カメラのステイト設定
	Camera::ChangeState(new CCameraStateSelectStage);

	// 読込
	Load();

	// ステージの設置
	SetStage();

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CSelectStageManager::Load(void)
{
	std::ifstream file(PATH_TEXT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// ステージ情報読込開始
				// ステージ情報の生成
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				m_aInfoStage.push_back(pInfoStage);
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

//=====================================================
// ステージの読込
//=====================================================
void CSelectStageManager::LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage)
{
	if (pInfoStage == nullptr)
		return;

	while (std::getline(file, str))
	{// 読込開始
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "END_SETSTAGE")
		{// 終了
			break;
		}

		if (key == "MODEL")
		{// パス
			iss >> str >> pInfoStage->pathModel;
		}

		if (key == "POS")
		{// 位置
			iss >> str >> pInfoStage->pos.x >> pInfoStage->pos.y >> pInfoStage->pos.z;
		}
	}
}

//=====================================================
// ステージの設置
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (m_aInfoStage.empty())
		return;

	for (S_InfoStage *pInfoStage : m_aInfoStage)
	{
		// ステージのXモデルの設置
		CObjectX *pObjectX = CObjectX::Create();

		int nIdxModel = CModel::Load(&pInfoStage->pathModel[0]);
		pObjectX->BindModel(nIdxModel);

		pObjectX->SetPosition(pInfoStage->pos);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStageManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStageManager::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CSelectStageManager::Draw(void)
{

}