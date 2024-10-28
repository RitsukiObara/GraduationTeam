//*****************************************************
//
// 流氷ファクトリー[flowIceFactory.cpp]
// Author:��山桃也
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
const int ADD_CREATE_FLOWICE = 5;	// 流氷の生成する位置の増やすグリッド数
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

#ifdef _DEBUG
	CreateFlowIce();
#endif

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

				m_apInfoFlowIce.push_back(pInfoFlowIce);
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

		if (key == "END_SETFLOWICE")
		{// 終了
			break;
		}

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
		CreateFlowIce();

		// 次の氷の形を決める
		DecideNextFlowIce();

		// カウントリセット
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

	// 生成する場所をグリッドの端からずらす
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();
	nNumGridH += ADD_CREATE_FLOWICE;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	S_InfoFlowIce *pInfo = m_apInfoFlowIce[m_nIdxNextIce];

	for (int i = 0; i < (int)pInfo->aIdx.size(); i++)
	{
		for (int j = 0; j < (int)pInfo->aIdx[i].size(); j++)
		{
			int n = pInfo->aIdx[i][j];

			if (pInfo->aIdx[i][j] != 0)
			{// 氷を生成するマスなら氷を生成
				// 氷を生成し、流氷システムに追加
				CIce *pIce = pIceManager->CreateFlowIce(0 + (int)pInfo->aIdx.size() - i - 1, nNumGridH + j);

				if (pIce == nullptr)
					continue;

				pIce->ChangeState(new CIceStateFlow);
				pFlowIce->AddIceToArray(pIce);
			}
		}
	}
}

//=====================================================
// 次の流氷の形を決める
//=====================================================
void CFlowIceFct::DecideNextFlowIce(void)
{
	if (m_apInfoFlowIce.empty())
		return;

	// 配列のサイズから決める
	int nMax = (int)m_apInfoFlowIce.size();

	m_nIdxNextIce = universal::RandRange(nMax, 0);
}

//=====================================================
// 描画処理
//=====================================================
void CFlowIceFct::Draw(void)
{

}