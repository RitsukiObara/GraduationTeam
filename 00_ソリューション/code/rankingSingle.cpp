//*****************************************************
//
// シングルのランキング処理[rankingSingle.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "rankingSingle.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_DATA_DEFAULT = 3;	// デフォルトのデータ数
}

//====================================================
// コンストラクタ
//====================================================
CRankingSingle::CRankingSingle()
{

}

//====================================================
// デストラクタ
//====================================================
CRankingSingle::~CRankingSingle()
{

}

//====================================================
// 生成処理
//====================================================
CRankingSingle *CRankingSingle::Create(string pathSave)
{
	CRankingSingle *pRanking = nullptr;

	pRanking = new CRankingSingle;

	if (pRanking != nullptr)
	{
		pRanking->m_pathSave = pathSave;
		pRanking->Init();
	}

	return pRanking;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CRankingSingle::Init(void)
{
#if 0
	Reset();	// リセット処理

	Save();
#endif

	Load();

	return S_OK;
}

//====================================================
// リセット
//====================================================
void CRankingSingle::Reset(void)
{
	int aScore[NUM_DATA_DEFAULT] = { 500,200,100 };
	int nTimePeck[NUM_DATA_DEFAULT] = { 50, 80, 90 };

	for (int i = 0; i < NUM_DATA_DEFAULT; i++)
	{
		S_InfoRank *pInfoRank = new S_InfoRank;

		if (pInfoRank == nullptr)
			continue;
		
		pInfoRank->nScore = aScore[i];
		pInfoRank->nTimePeck = nTimePeck[i];

		m_apRank.push_back(pInfoRank);
	}
}

//====================================================
// 配列の解放
//====================================================
void CRankingSingle::ReleaseArray(void)
{
	for (int i = 0; i < (int)m_apRank.size(); i++)
	{
		if (m_apRank[i] == nullptr)
			continue;

		delete m_apRank[i];
		m_apRank[i] = nullptr;
	}

	m_apRank.clear();
}

//====================================================
// ソート
//====================================================
void CRankingSingle::Sort(void)
{
	// 降順にソート (valueを基準に)
	std::sort(m_apRank.begin(), m_apRank.end(), [](S_InfoRank* a, S_InfoRank* b) { return a->nScore > b->nScore; });

	// 昇順にソート (peckを基準に)
	std::sort(m_apRank.begin(), m_apRank.end(), [](S_InfoRank* a, S_InfoRank* b) { return a->nTimePeck < b->nTimePeck; });
}

//====================================================
// ランクの追加
//====================================================
int CRankingSingle::AddRank(int nScore, int nTimePeck)
{
	int nUpdate = -1;

	// ソートして、一番小さい値と比較
	Sort();
	
	S_InfoRank *pInfoMin = m_apRank[m_apRank.size() - 1];

	if (pInfoMin->nScore <= nScore)
	{
		pInfoMin->nScore = nScore;
		pInfoMin->nTimePeck = nTimePeck;

		// 再ソートしておく
		Sort();

		// 更新した番号の保存
		for (int i = 0; i < (int)m_apRank.size(); ++i)
		{
			if (m_apRank[i] != pInfoMin)
				continue;

			nUpdate = i;
			break;
		}
	}

#ifndef _DEBUG
	// 保存する
	Save();
#endif

	return nUpdate;
}

//====================================================
// 終了処理
//====================================================
void CRankingSingle::Uninit(void)
{
	ReleaseArray();

	Release();
}

//====================================================
// 更新処理
//====================================================
void CRankingSingle::Update(void)
{

}

//====================================================
// 描画処理
//====================================================
void CRankingSingle::Draw(void)
{

}

//====================================================
// 読込
//====================================================
void CRankingSingle::Load(void)
{
	// 配列の解放
	ReleaseArray();

	// ファイルを開く
	std::ifstream inputFile(m_pathSave, std::ios::binary);

	if (!inputFile.is_open())
		assert(("ランキングファイルを開けませんでした", false));

	while (true)
	{// ファイルの終わりまで読み込む
		S_InfoRank *pInfoRank = new S_InfoRank;

		inputFile.read(reinterpret_cast<char*>(pInfoRank), sizeof(S_InfoRank));

		if (inputFile.eof())
		{
			delete pInfoRank;
			break;
		}

		m_apRank.push_back(pInfoRank);
	}

	inputFile.close();
}

//====================================================
// 保存
//====================================================
void CRankingSingle::Save(void)
{
	// ファイルを開く
	std::ofstream outputFile(m_pathSave, std::ios::binary);

	if (!outputFile.is_open())
		assert(("ランキングファイルを開けませんでした", false));

	// 情報サイズ
	size_t size = m_apRank.size();

	for (S_InfoRank *pInfo : m_apRank)
	{
		// データの保存
		outputFile.write(reinterpret_cast<char*>(&pInfo->nScore), sizeof(int));
		outputFile.write(reinterpret_cast<char*>(&pInfo->nTimePeck), sizeof(int));
	}

	outputFile.close();
}