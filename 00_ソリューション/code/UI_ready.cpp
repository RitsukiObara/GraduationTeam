//*****************************************************
//
// タイムの処理[timer.cpp]
// Author:森川駿弥
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UI_ready.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int TIME_DIGIT = 1;	// それぞれの桁数
	const int TIME_SECOND = 3;	// 秒数
	const int STATE_COUNT_MAX = 60;	// ステートのカウント最大値
	const float DIST_NUMBER = 0.03f;	// 数字間の距離
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.05f, 0.09f };	// 通常数字のサイズ
	const D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// ミニ数字のサイズ
	const float SIZE_NUM = 0.5;	// Go数字のサイズ
	const D3DXVECTOR3 POS_INITIAL = { 0.6f,0.5f,0.0f };	// 初期位置
	const D3DXVECTOR3 POS_GO = { 0.5f,0.5f,0.0f };	// 初期位置
	const string PATH_TEX_OK = "data\\TEXTURE\\UI\\ready.png";	// コロンのテクスチャパス
	const D3DXCOLOR NORMAL_COL = { 1.0f,1.0f,1.0f,1.0f };		//基準色
	const int FRAME_CNT = 60;		// フレーム秒数
	const int SECOND_ELAPSED = 1;		// 秒数経過
}

//=====================================================
// コンストラクタ
//=====================================================
CUIready::CUIready()
{
	m_nSecond = 0;
	m_fScaleNumber = 0;
	m_bStop = false;
	m_nFrame = 0;
	m_state = STATE_NUMBER;
	m_StateCnt = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CUIready::~CUIready()
{

}

//=====================================================
// 生成処理
//=====================================================
CUIready* CUIready::Create(void)
{
	CUIready* pUI_ready = nullptr;

	pUI_ready = new CUIready;

	if (pUI_ready != nullptr)
	{// 初期化
		pUI_ready->Init();
	}

	return pUI_ready;
}

//=====================================================
// 時間保存処理
//=====================================================
void CUIready::SaveSecond(int fSecond)
{
	//// ファイルを開く
	//std::ofstream file("data\\TEMP\\time.bin", std::ios_base::binary);	// ファイルストリーム
	//if (file.fail())
	//{ // ファイルが開けなかった場合

	//	// エラーメッセージボックス
	//	MessageBox(nullptr, "時間の書き出しに失敗！", "警告！", MB_ICONWARNING);
	//	return;
	//}

	//// 引数の時間を書き出し
	//file.write((char*)&fSecond, sizeof(float));

	//// ファイルを閉じる
	//file.close();
}

////=====================================================
//// 時間読込処理
////=====================================================
//int CUIready::LoadSecond(void)
//{
//	//// ファイルを開く
//	//std::ifstream file("data\\TEMP\\time.bin", std::ios_base::binary);	// ファイルストリーム
//	//if (file.fail())
//	//{ // ファイルが開けなかった場合
//
//	//	// エラーメッセージボックス
//	//	MessageBox(nullptr, "時間の読み込みに失敗！", "警告！", MB_ICONWARNING);
//	//	return 0;
//	//}
//
//	//// 引数の時間を読み込み
//	//int nTime = 0;
//	//file.read((char*)&nTime, sizeof(float));
//
//	//// ファイルを閉じる
//	//file.close();
//
//	//// 読み込んだ時間を返す
//	//return nTime;
//}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIready::Init(void)
{
	m_nSecond = 0;	// 秒の初期化
	m_fScaleNumber = 1.0f;	// 初期スケール設定
	m_bStop = false;	// タイマー停止のフラグ
	m_nFrame = 0;		//フレーム数初期化
	m_StateCnt = 0;		//状態遷移カウントの初期化

	// 初期位置の設定
	SetPosition(POS_INITIAL);

	SetSecond(TIME_SECOND);

	// 数字の配列のリサイズ
	m_aNumber.resize(E_Number::NUMBER_MAX);

	int aDigit[E_Number::NUMBER_MAX] =
	{// 桁数
		TIME_DIGIT,
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
void CUIready::Uninit(void)
{
	for (auto it : m_aNumber)
	{
		it->Uninit();
	}

	m_aNumber.clear();

	if (m_Go != nullptr)
	{
		m_Go->Uninit();
	}

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CUIready::Update(void)
{
	// 秒数をもらう
	m_nSecond = GetSecond();

	switch (m_state)
	{
	case STATE_NUMBER:

		if (m_nSecond <= 0)
		{
			m_state = STATE_GO;

			for (int i = 0; i < E_Number::NUMBER_MAX; i++)
			{
				m_aNumber[i]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}

			// GOUIを生成
			m_Go = CUI::Create();

			m_Go->SetPosition(POS_GO);
			int nIdxTexture = Texture::GetIdx(&PATH_TEX_OK[0]);
			m_Go->SetIdxTexture(nIdxTexture);
			m_Go->SetSize(SIZE_NUM, SIZE_NUM);

			m_Go->SetVtx();
		}

		break;

	case STATE_GO:

		if (m_Go == nullptr)
		{
			return;
		}	
		
		//ステートカウント加算
		m_StateCnt++;

		if (m_StateCnt >= STATE_COUNT_MAX)
		{
			// 終了
			Uninit();
		}

		break;
	}

	UpdateNumber();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在の秒：[%d]", m_nSecond);
	//CDebugProc::GetInstance()->Print("\nスコアの位置：[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// 数字の更新
//=====================================================
void CUIready::UpdateNumber()
{
	if (m_aNumber.empty())
		return;

	if (m_nSecond < 0)
	{
		m_nSecond = 0;
	}

	//フレーム加算
	m_nFrame++;

	if (m_nFrame >= FRAME_CNT)
	{
		m_nSecond = m_nSecond - SECOND_ELAPSED;

		m_nFrame = 0;
	}

	// 値の用意
	int aValue[E_Number::NUMBER_MAX] =
	{
#if 1

#else
		(int)((DWORD)(m_nSecond * 1000) / 60000),
		(int)((DWORD)(m_nSecond * 1000) / 1000 % 60),
#endif
	};

	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		m_aNumber[i]->SetValue(m_nSecond);
	}
}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CUIready::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	int aDigit[E_Number::NUMBER_MAX] =
	{// 桁数
		TIME_DIGIT,
	};

	D3DXVECTOR2 aSize[E_Number::NUMBER_MAX] =
	{// 数字のサイズ
		SIZE_NORMAL_NUM * m_fScaleNumber,
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
void CUIready::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 数字のスケールの設定
//=====================================================
void CUIready::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 色の設定
//=====================================================
void CUIready::SetColor(E_Number number, D3DXCOLOR col)
{
	if (number < 0 || number > E_Number::NUMBER_MAX)
		return;

	if (number == E_Number::NUMBER_MAX)
	{// 全数字の色設定
		for (auto it : m_aNumber)	// 数字
			it->SetColor(col);

			m_Go->SetCol(col);
	}
	else
	{// 各数字の色設定
		m_aNumber[number]->SetColor(col);
	}
}

//=====================================================
// 色の取得
//=====================================================
D3DXCOLOR CUIready::GetColor(E_Number number)
{
	if (number < 0 || number >= E_Number::NUMBER_MAX)
		return D3DXCOLOR(NORMAL_COL);

	return m_aNumber[number]->GetColor();
}

//=====================================================
// 描画処理
//=====================================================
void CUIready::Draw()
{

}