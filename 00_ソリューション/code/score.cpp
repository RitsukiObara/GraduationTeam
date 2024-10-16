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
	const int SCORE_DIGIT = 6;	// スコア表示の桁数
	const float DIST_NUMBER = 0.01f;	// 数字間の距離
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// 通常数字のサイズ
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// ミニ数字のサイズ
	D3DXVECTOR3 POS_INITIAL = { 0.01f,0.5f,0.0f };	// 初期位置
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

	// 数字の配列のリサイズ
	m_aNumber.resize(SCORE_DIGIT);

	int aDigit[SCORE_DIGIT] =
	{// 桁数
		1,
		1,
		1,
		1,
		1,
		1,
	};

	// 数字の生成
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i] = CNumber::Create(aDigit[i], 0);	// 数字の生成
	}

	// 数字のトランスフォームの設定
	TransformNumber();

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

	// 値の用意
	int aValue[SCORE_DIGIT] =
	{
		(m_nScore % 1000000 / 100000),
		(m_nScore % 100000 / 10000),
		(m_nScore % 10000 / 1000),
		(m_nScore % 1000 / 100),
		(m_nScore % 100 / 10),
		(m_nScore % 10),
	};

	// スコアの加算========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_SCORE))
	{// スコアを加算する
		AddScore(1000);
	}

	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在のスコア：[%d]", m_nScore);
	//CDebugProc::GetInstance()->Print("\nスコアの位置：[%f,%f,%f]", pos.x, pos.y, pos.z);
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

	int aDigit[SCORE_DIGIT] =
	{// 桁数
		1,
		1,
		1,
		1,
		1,
		1,
	};

	D3DXVECTOR2 aSize[SCORE_DIGIT] =
	{// 数字のサイズ
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber
	};

	D3DXVECTOR3 posBase = GetPosition();

	// 数字分、生成して設定
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// 参照するサイズの番号
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0番目でなければ前回のサイズを参照する

		//// パラメーター設定
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける

		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

		// パラメーター設定
		float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける
		D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		m_aNumber[i]->SetPosition(pos);
		m_aNumber[i]->SetSizeAll(aSize[i].x, aSize[i].y);

		if (i == 0)	// 0以上のときしか入らない処理
			continue;
	}
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

////=====================================================
//// 色の設定
////=====================================================
//void CScore::SetColor(E_Number number, D3DXCOLOR col)
//{
//	if (number < 0 || number > SCORE_DIGIT)
//		return;
//
//	if (number == SCORE_DIGIT)
//	{// 全数字の色設定
//		for (auto it : m_aNumber)	// 数字
//			it->SetColor(col);
//	}
//	else
//	{// 各数字の色設定
//		m_aNumber[number]->SetColor(col);
//	}
//}
//
////=====================================================
//// 色の取得
////=====================================================
//D3DXCOLOR CScore::GetColor(E_Number number)
//{
//	if (number < 0 || number >= SCORE_DIGIT)
//		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//	return m_aNumber[number]->GetColor();
//}

//=====================================================
// 描画処理
//=====================================================
void CScore::Draw()
{

}