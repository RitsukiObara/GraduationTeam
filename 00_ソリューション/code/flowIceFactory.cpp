//*****************************************************
//
// 流氷ファクトリー[flowIceFactory.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "flowIceFactory.h"
#include "iceManager.h"
#include "ice.h"
#include "flowIce.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\flowIce.txt";	// 流氷情報のファイルパス

const float TIME_CREATE_FLOWICE = 5.0f;	// 流氷を作る時間
const int NUM_ICE = 4;	// 適当な初期アイス数
}

//=====================================================
// コンストラクタ
//=====================================================
CFlowIceFct::CFlowIceFct(int nPriority) : CObject(nPriority), m_fTimerCreateFlowIce(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CFlowIceFct::~CFlowIceFct()
{

}

//=====================================================
// 生成処理
//=====================================================
CFlowIceFct *CFlowIceFct::Create(void)
{
	CFlowIceFct *pFct = nullptr;

	pFct = new CFlowIceFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CFlowIceFct::Init(void)
{
	// 読込処理
	Load();

	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CFlowIceFct::Load(void)
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

			if (key == "SETFLOWICE")
			{// 流氷情報読込開始
				// 流氷情報の生成
				S_InfoFlowIce *pInfoFlowIce = new S_InfoFlowIce;

				if (pInfoFlowIce == nullptr)
					continue;

				// 最大の行数を決める
				int nNumV;
				int nNumH;
				GetNumFlowIce(file, temp, &nNumV, &nNumH, pInfoFlowIce);

				while (std::getline(file, temp))
				{
					std::istringstream iss(temp);
					std::string key;
					iss >> key;

					for (int i = 0; i < (int)temp.length(); i++)
					{
						char cData = temp[i];


					}

					//if (key == "TYPE")
					//{// 種類
					//	int nType;
					//	iss >> temp >> nType;
					//	lightInfo.Type = (D3DLIGHTTYPE)nType;
					//}

					if (key == "END_SETFLOWICE")
					{// 終了
						if (pInfoFlowIce != nullptr)
							m_apInfoFlowIce.push_back(pInfoFlowIce);

						break;
					}
				}
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
// 氷の縦横の数を取得
//=====================================================
void CFlowIceFct::GetNumFlowIce(std::ifstream& file, string str, int *pNumV, int *pNumH, S_InfoFlowIce* pInfoFlowIce)
{
	if (pInfoFlowIce == nullptr)
		return;

	// 数字のリセット
	*pNumV = 0;
	*pNumH = 0;

	while (std::getline(file, str))
	{// 読込開始
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (str.length() == 0)
			continue;	// 読み込んだ文字が空なら通らない

		// 行の配列追加
		vector<int> aIdx;
		pInfoFlowIce->aIdx.push_back(aIdx);

		for (int i = 0; i < (int)str.length(); i++)
		{// 文字列の行の長さ検知
			char cData = str[i];

			if (cData != ' ')
			{// 空白じゃない場合、行の長さを増やす
				*pNumH += 1;
			}
		}

		for (int i = 0; i < (int)str.length(); i++)
		{// 文字列を分解して解析
			char cData = str[i];

			if (cData != ' ')
			{// 空白じゃない場合、行の長さを増やす
				int nData = cData - '0';

				pInfoFlowIce->aIdx[*pNumV].push_back(nData);
			}
		}

		// なにかしら文字があれば行数を増やす
		*pNumV += 1;

		if (key == "END_SETFLOWICE")
		{// 値を保存して終了
			break;
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CFlowIceFct::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFlowIceFct::Update(void)
{
	// 経過時間加算
	m_fTimerCreateFlowIce += CManager::GetDeltaTime();

	// 一定時間で流氷を流す
	if (m_fTimerCreateFlowIce > TIME_CREATE_FLOWICE)
	{
		// 流氷の生成
		//CreateFlowIce();

		m_fTimerCreateFlowIce = 0.0f;
	}
}

//=====================================================
// 流氷の生成
//=====================================================
void CFlowIceFct::CreateFlowIce(void)
{
	// 流氷システムの生成
	CFlowIce *pFlowIce = CFlowIce::Create();

	if (pFlowIce == nullptr)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 氷を生成し、システムに追加
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();

	CIceManager *pIceManager = CIceManager::GetInstance();

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
		// 生成する場所に既に氷がある場合は、スキップ
		if (pIceManager->GetGridIce(&aV[i], &aH[i]) != nullptr)
			continue;

		// 氷を生成し、流氷システムに追加
		CIce *pIce = pIceManager->CreateIce(aV[i], aH[i]);

		if (pIce == nullptr)
			continue;

		pIce->ChangeState(new CIceStateFlow);
		pFlowIce->AddIceToArray(pIce);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFlowIceFct::Draw(void)
{

}