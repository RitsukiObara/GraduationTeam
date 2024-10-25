//*****************************************************
//
// 流氷が流れてくる処理[iceSpeedManager.cpp]
// Author:若木 一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceSpeedManager.h"
#include "ice.h"
#include "iceManager.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int NUM_ICE = 4;	// 氷の数
	const std::string ICE_FLOW = "data\\TEXT\\kari.txt";	// 流れる氷のパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************


//=====================================================
// コンストラクタ
//=====================================================
CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceSpeedManager::~CIceSpeedManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CIceSpeedManager* CIceSpeedManager::Create(void)
{
	CIceSpeedManager* pIceSpeedManager = nullptr;

	pIceSpeedManager = new CIceSpeedManager;

	if (pIceSpeedManager == nullptr)
		return nullptr;

	pIceSpeedManager->Init();

	return pIceSpeedManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceSpeedManager::Init(void)
{
	// 流氷の生成
	CreateIce();

	// 読込
	Load((char*)&ICE_FLOW[0]);

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CIceSpeedManager::Load(char* pPath)
{
	//for (int nCntIce = 0; nCntIce < MAX_ICEFLOW; nCntIce++)
	//{// パーティクル情報の破棄
	//	if (m_aMotionInfo[nCntIce].pEvent != nullptr)
	//	{
	//		delete m_aMotionInfo[nCntIce].pEvent;
	//		m_aMotionInfo[nCntIce].pEvent = nullptr;
	//	}
	//}

	ZeroMemory(&m_apIce[0], sizeof(m_apIce));
	m_nNumIce = 0;

	//変数宣言
	char cTemp[MAX_STRING];
	int nCntIce = 0;

	//ファイルから読み込む
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{
		// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// 氷の総数読み込み
			if (strcmp(cTemp, "NUM_ICE") == 0)
			{
				// "="読み込み (無視)
				(void)fscanf(pFile, "%s", &cTemp[0]);

				// 氷の総数を読み込む
				(void)fscanf(pFile, "%d", &m_nNumIce);
			}

			// "SET_FLOW_ICE" ブロック開始
			if (strcmp(cTemp, "SET_FLOW_ICE") == 0)
			{
				int row = 0;
				while (true)
				{
					// テキストを読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// "END_SET_FLOW_ICE" を検出したら終了
					if (strcmp(cTemp, "END_SET_FLOW_ICE") == 0)
					{
						break;
					}

					// 行の数値を処理
					std::istringstream iss(cTemp);
					int value;
					int col = 0;
					while (iss >> value)
					{
						// 数値が1の場合、氷を生成
						if (value == 1)
						{
							D3DXVECTOR3 pos;
							//pos.x = static_cast<float>(row); // X位置
							//pos.y = 0.0f;                   // 固定値Y
							//pos.z = static_cast<float>(col); // Z位置

							// 氷の位置をセット
							m_apIce[nCntIce]->pIce->SetPosition(pos);

							// 氷カウント増加
							nCntIce++;
						}
						col++;
					}
					row++;
				}
			}

			// "END_SCRIPT" を検出したら終了
			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("流れる氷のデータ読み込みに失敗", false));
	}
}

//=====================================================
// 氷の生成
//=====================================================
void CIceSpeedManager::CreateIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int aV[NUM_ICE] =
	{// 縦のグリッド番号の配列
		3,4,5,6
	};
	int aH[NUM_ICE] =
	{// 横のグリッド番号の配列
		9,9,9,9
	};

	for (int i = 0; i < NUM_ICE; i++)
	{
		CIce* pIce = pIceManager->CreateIce(aV[i], aH[i]);
		pIce->ChangeState(new CIceStateFlow);
		m_apIceSpeed.push_back(pIce);
	}
}

//=====================================================
// 氷を配列に追加
//=====================================================
void CIceSpeedManager::AddIceToArray(CIce* pIce)
{
	m_apIceSpeed.push_back(pIce);
}

//=====================================================
// 終了処理
//=====================================================
void CIceSpeedManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CIceSpeedManager::Update(void)
{
	// どれかの氷が止まっていないかのチェック
	CheckSomeIceStop();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// どれかの氷が止まっていないかのチェック
//=====================================================
void CIceSpeedManager::CheckSomeIceStop(void)
{
	for (auto it : m_apIceSpeed)
	{
		if (it->IsStop())
		{// 止まっているなら全ての氷を止める
			StopAllIce();
			Uninit();	// 自身の破棄
			break;
		}
	}
}

//=====================================================
// 全ての氷を止める
//=====================================================
void CIceSpeedManager::StopAllIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIceSpeed)
	{
		it->ChangeState(new CIceStaeteNormal);

		pIceManager->AddIce(it, it->GetPosition());
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CIceSpeedManager::Debug(void)
{
	for (auto it : m_apIceSpeed)
	{
		D3DXVECTOR3 posIce = it->GetPosition();

		CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// 描画処理
//=====================================================
void CIceSpeedManager::Draw(void)
{

}