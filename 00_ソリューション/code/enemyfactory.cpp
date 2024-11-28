//*****************************************************
//
// 敵ファクトリー[enemyfactory.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemyfactory.h"
#include "manager.h"
#include "enemy.h"
#include "game.h"
#include "fishshadow.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyFct::CEnemyFct(int nPriority) : CObject(nPriority), m_fTimerSpawn(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyFct::~CEnemyFct()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemyFct *CEnemyFct::Create(void)
{
	CEnemyFct *pFct = nullptr;

	pFct = new CEnemyFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyFct::Init(void)
{
	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CEnemyFct::Load(string path)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETENEMY")
			{// 敵情報読込
				S_InfoEnemy *pInfoEnemy = new S_InfoEnemy;

				LoadInfo(file, temp, pInfoEnemy);

				m_apInfoEnemy.push_back(pInfoEnemy);
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

	// 敵の設定
	SetEnemy();
}

//=====================================================
// 情報の読込
//=====================================================
void CEnemyFct::LoadInfo(std::ifstream& file, string str, S_InfoEnemy *pInfoEnemy)
{
	while (std::getline(file, str))
	{// 読込開始
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "DELAY")
		{// ディレイ
			iss >> str >> pInfoEnemy->fDelaySpawn;
		}

		if (key == "TYPE")
		{// 種類
			iss >> str >> pInfoEnemy->nType;
		}

		if (key == "SPAWN")
		{// スポーンパターン
			iss >> str >> pInfoEnemy->nPaternSpawn;
		}

		if (key == "END_SETENEMY")
		{// 終了
			break;
		}
	}
}

//=====================================================
// 敵の設定
//=====================================================
void CEnemyFct::SetEnemy(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	for (S_InfoEnemy *pInfoEnemy : m_apInfoEnemy)
	{
		pGame->AddEnemy(pInfoEnemy->nType);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyFct::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyFct::Update(void)
{
	// 経過時間加算
	float fTimeOld = m_fTimerSpawn;
	m_fTimerSpawn += CManager::GetDeltaTime();

	for (S_InfoEnemy *pInfo : m_apInfoEnemy)
	{
		if (pInfo->fDelaySpawn > fTimeOld && pInfo->fDelaySpawn <= m_fTimerSpawn)
		{// スポーンディレイの何秒前に魚影をだす
			CFishShadow::Create(CEnemy::TYPE::TYPE_SEALS, D3DXVECTOR3(0.5f, 0.5f, 0.0f));
		}


		if (pInfo->fDelaySpawn > fTimeOld && pInfo->fDelaySpawn <= m_fTimerSpawn)
		{// スポーンディレイを越えた瞬間にスポーン
			CEnemy::Create(pInfo->nType,(CEnemy::E_Spawn)pInfo->nPaternSpawn);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyFct::Draw(void)
{

}