//*****************************************************
//
// 敵UIの処理[UI_enemy.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UI_enemy.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float DIST_NUMBER = 100.0f;	// 数字間の距離
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// 通常数字のサイズ
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.005f, 0.010f };	// ミニ数字のサイズ
	D3DXVECTOR3 POS_INITIAL = { 0.5f,0.5f,0.0f };	// 初期位置
}

//=====================================================
// コンストラクタ
//=====================================================
CUI_Enemy::CUI_Enemy()
{
	m_nEnemy = 0;
	m_fScaleNumber = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CUI_Enemy::~CUI_Enemy()
{

}

//=====================================================
// 生成処理
//=====================================================
CUI_Enemy* CUI_Enemy::Create(int m_nEnemy)
{

	CUI_Enemy* pUI_enemy = nullptr;

	pUI_enemy = new CUI_Enemy;

	pUI_enemy->SetUI(m_nEnemy);

	m_nEnemy = pUI_enemy->GetUI();

	if (pUI_enemy != nullptr)
	{// 初期化
		pUI_enemy->Init();
	}

	return pUI_enemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUI_Enemy::Init(void)
{
	m_fScaleNumber = 1.0f;	// 初期スケール設定

	int m_nEnemy = CUI_Enemy::GetUI();

	// 初期位置の設定
	SetPosition(POS_INITIAL);

	// 数字の配列のリサイズ
	m_aNumber.resize(m_nEnemy);

	// 数字の生成
	for (int i = 0; i < m_nEnemy; i++)
	{
		m_aNumber[i] = CNumber::Create(m_nEnemy, 0);	// 数字の生成
	}

	// 数字のトランスフォームの設定
	TransformNumber();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUI_Enemy::Uninit(void)
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
void CUI_Enemy::Update(void)
{
	UpdateNumber();
}

//=====================================================
// 数字の更新
//=====================================================
void CUI_Enemy::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber.empty())
		return;

	/*for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}*/

	D3DXVECTOR3 pos = GetPosition();

	SetPosition(pos);

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在の数値：[%d]", m_nEnemy);
	CDebugProc::GetInstance()->Print("\nスコアの位置：[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CUI_Enemy::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	D3DXVECTOR3 posBase = GetPosition();

	// 数字分、生成して設定
	for (int i = 0; i < m_nEnemy; i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// 参照するサイズの番号
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0番目でなければ前回のサイズを参照する

		/*D3DXVECTOR3 pos = D3DXVECTOR3(posBase.x + (1 * i), posBase.y, posBase.z);

		SetPosition(pos);*/

		//// パラメーター設定
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける

		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

		//// パラメーター設定
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける
		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		//m_aNumber[i]->SetPosition(pos);
		//m_aNumber[i]->SetSizeAll(aSize[i].x, aSize[i].y);

		if (i == 0)	// 0以上のときしか入らない処理
			continue;
	}
}

//=====================================================
// 位置の設定
//=====================================================
void CUI_Enemy::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 数字のスケールの設定
//=====================================================
void CUI_Enemy::SetScaleNumber(float fScale)
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
void CUI_Enemy::Draw()
{

}