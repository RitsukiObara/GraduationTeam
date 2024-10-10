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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float	TIME_MIN = 0.0f;	// 最少タイム
	const float	TIME_MAX = 60.0f * 99.0f + 59.0f + 0.999f;	// 最大タイム
	const int MINUTES_LIMIT = 9;	// 分の上限値
	const int SECOND_LIMIT = 59;	// 秒の上限値
	const int MILLI_LIMIT = 99;	// ミリ秒の上限
	const int MINUTES_DIGIT = 2;	// 分表示の桁数
	const int TIME_DIGIT = 2;	// それぞれの桁数
	const float MAGNIFICATION = 100.0f;	// 掛ける倍率
	const float DIST_NUMBER = 0.03f;	// 数字間の距離
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// 通常数字のサイズ
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// ミニ数字のサイズ
	D3DXVECTOR3 POS_INITIAL = { 0.1f,0.5f,0.0f };	// 初期位置
}

//=====================================================
// コンストラクタ
//=====================================================
CScore::CScore()
{
	m_fScore = 0.0f;
	m_fScaleNumber = 0.0f;
	m_bStop = false;
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
	CScore* pTimer = nullptr;

	pTimer = new CScore;

	if (pTimer != nullptr)
	{// 初期化
		pTimer->Init();
	}

	return pTimer;
}

//=====================================================
// スコア保存処理
//=====================================================
void CScore::SaveScore(float fSecond)
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
	file.write((char*)&fSecond, sizeof(float));

	// ファイルを閉じる
	file.close();
}

//=====================================================
// スコア読込処理
//=====================================================
float CScore::LoadScore(void)
{
	// ファイルを開く
	std::ifstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "スコアの読み込みに失敗！", "警告！", MB_ICONWARNING);
		return 0.0f;
	}

	// 引数のスコアを読み込み
	float fTime = 0.0f;
	file.read((char*)&fTime, sizeof(float));

	// ファイルを閉じる
	file.close();

	// 読み込んだスコアを返す
	return fTime;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScore::Init(void)
{
	m_fScore = 0.0f;	// スコアの初期化
	m_fScaleNumber = 1.0f;	// 初期スケール設定
	m_bStop = false;	// タイマー停止のフラグ

	// 初期位置の設定
	SetPosition(POS_INITIAL);

	// 数字の配列のリサイズ
	m_aNumber.resize(E_Number::NUMBER_MAX);

	int aDigit[E_Number::NUMBER_MAX] =
	{// 桁数
		MINUTES_DIGIT,
		TIME_DIGIT,
		TIME_DIGIT
	};

	// 数字の生成
	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
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

}

//=====================================================
// 数字の更新
//=====================================================
void CScore::UpdateNumber()
{
	if (m_aNumber.empty())
		return;

	// 値の用意
	int aValue[E_Number::NUMBER_MAX] =
	{
#if 0
		(int)m_fSecond / 60,
		(int)m_fSecond % 60,
		(int)(m_fSecond * MAGNIFICATION) % (int)MAGNIFICATION
#else
		(int)((DWORD)(m_fScore * 1000.0f) / 60000),
		(int)((DWORD)(m_fScore * 1000.0f) / 1000 % 60),
		(int)((DWORD)(m_fScore * 1000.0f) % 1000)
#endif
	};

	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}
}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CScore::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	int aDigit[E_Number::NUMBER_MAX] =
	{// 桁数
		MINUTES_DIGIT,
		TIME_DIGIT,
		TIME_DIGIT
	};

	D3DXVECTOR2 aSize[E_Number::NUMBER_MAX] =
	{// 数字のサイズ
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber
	};

	D3DXVECTOR3 posBase = GetPosition();

	// 数字分、生成して設定
	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// 参照するサイズの番号
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0番目でなければ前回のサイズを参照する

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

//=====================================================
// 色の設定
//=====================================================
void CScore::SetColor(E_Number number, D3DXCOLOR col)
{
	if (number < 0 || number > E_Number::NUMBER_MAX)
		return;

	if (number == E_Number::NUMBER_MAX)
	{// 全数字の色設定
		for (auto it : m_aNumber)	// 数字
			it->SetColor(col);
	}
	else
	{// 各数字の色設定
		m_aNumber[number]->SetColor(col);
	}
}

//=====================================================
// 色の取得
//=====================================================
D3DXCOLOR CScore::GetColor(E_Number number)
{
	if (number < 0 || number >= E_Number::NUMBER_MAX)
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return m_aNumber[number]->GetColor();
}

//=====================================================
// 描画処理
//=====================================================
void CScore::Draw()
{

}