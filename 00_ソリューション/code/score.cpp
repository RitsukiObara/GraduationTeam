//*****************************************************
//
// スコアの処理[Score.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "score.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// 最少スコア
	const int	SCORE_MAX = 999999;	// 最大スコア
	const int SCORE_LIMIT = 9;	// スコアの上限値
	const float DIST_NUMBER = 0.01f;	// 数字間の距離
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// 通常数字のサイズ
	const D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// ミニ数字のサイズ
	const D3DXVECTOR3 POS_INITIAL = { 0.09f, 0.07f, 0.0f };	// 初期位置
}

//=====================================================
// コンストラクタ
//=====================================================
CScore::CScore()
{
	m_nScore = SCORE_MIN;
	m_fScaleNumber = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CScore::~CScore()
{

}

//=====================================================
// 生成処理
//=====================================================
CScore* CScore::Create(void)
{
	CScore* pScore = nullptr;

	pScore = new CScore;

	if (pScore != nullptr)
	{// 初期化
		pScore->Init();

		//情報の設定
		pScore->SetData();
	}

	return pScore;
}

//=====================================================
// スコア保存処理
//=====================================================
void CScore::SaveScore(int nSecond)
{
	// ファイルを開く
	std::ofstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "スコアの書き出しに失敗！", "警告！", MB_ICONWARNING);
		return;
	}

	// 引数のスコアを書き出し
	file.write((char*)&nSecond, sizeof(int));

	// ファイルを閉じる
	file.close();
}

//=====================================================
// スコア読込処理
//=====================================================
int CScore::LoadScore(void)
{
	// ファイルを開く
	std::ifstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "スコアの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return 0;
	}

	// 引数のスコアを読み込み
	int nScore = 0;
	file.read((char*)&nScore, sizeof(int));

	// ファイルを閉じる
	file.close();

	// 読み込んだスコアを返す
	return nScore;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScore::Init(void)
{
	m_nScore = SCORE_MIN;	// スコアの初期化
	m_fScaleNumber = 1.0f;	// 初期スケール設定

	// 初期位置の設定
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CScore::Uninit(void)
{
	for (auto it : m_aNumber)
	{
		it->Uninit();
	}

	m_aNumber.clear();

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CScore::Update(void)
{
	UpdateNumber();
}

//=====================================================
// 数字の更新
//=====================================================
void CScore::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber.empty())
		return;

	std::vector<int> value;

	value.resize(m_aNumber.size());

	for (int nCnt = 0; nCnt < (int)m_aNumber.size(); nCnt++)
	{
		// 値を計算
		value[nCnt] = (m_nScore % (int)(pow(10, (m_aNumber.size() - (nCnt)))) / (int)(pow(10, (m_aNumber.size() - (nCnt + 1)))));
	}

	for (int i = 0; i < (int)m_aNumber.size(); i++)
	{
		m_aNumber[i]->SetValue(value[i]);
	}

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在のスコア：[%d]", m_nScore);
#endif
#endif
}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CScore::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	// 数字のサイズ
	D3DXVECTOR2 Size = SIZE_NORMAL_NUM * m_fScaleNumber;

	D3DXVECTOR3 posBase = GetPosition();

	// 数字分、生成して設定
	for (int i = 0; i < (int)m_aNumber.size(); i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// 参照するサイズの番号
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0番目でなければ前回のサイズを参照する

		// パラメーター設定
		float fWidth = Size.x * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける
		D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		m_aNumber[i]->SetPosition(pos);
		m_aNumber[i]->SetSizeAll(Size.x, Size.y);

		if (i == 0)	// 0以上のときしか入らない処理
			continue;
	}
}

//=====================================================
// 情報の設定
//=====================================================
void CScore::SetData(int nDigit)
{
	// 数字の配列のリサイズ
	m_aNumber.resize(nDigit);

	// 数字の生成
	for (int i = 0; i < nDigit; i++)
	{
		m_aNumber[i] = CNumber::Create(1, 0);	// 数字の生成
	}

	// 数字のトランスフォームの設定
	TransformNumber();

}

//=====================================================
// 位置の設定
//=====================================================
void CScore::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 数字のスケールの設定
//=====================================================
void CScore::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 色の設定
//=====================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (auto it : m_aNumber)	// 数字
			it->SetColor(col);
}

//=====================================================
// 描画処理
//=====================================================
void CScore::Draw()
{

}