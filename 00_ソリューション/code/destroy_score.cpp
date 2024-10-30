//*****************************************************
//
// 敵を倒した時のスコア処理[destroy_score.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "destroy_score.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// 最少スコア
	const float DIST_NUMBER = 0.01f;	// 数字間の距離
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 50.0f, 50.0f };	// 通常数字のサイズ
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// 初期位置
	const int	WAITTIME = 60;	// 滞留時間
	const float	GOAL_X = 0.5f;	// Xのゴール地点
	const float	MOVE_SPEED = 0.1f;	// 移動速度
	const float	VERTICAL_STOP = 0.15f;	// 縦移動の停止地点
	const float	SLOW_MOVE = 0.001f;	// スロー速度
	const float	THINITY_SPEED = 0.02f;	// 透明になっていく速度
	const float	GOAL_Y = 0.1f;	// Yのゴール地点
	const float	THINITY_COL = 0.0f;	// 透明になる
	const int	VALUE_SCORE = 2;	// 追加するスコアの桁数(アザラシ)
	const float	SCORE_SCALE = 1.0f;	// スコアのスケール
	const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,1.0f,1.0f };	// スコアの初期色
	const int	ADD_SEALS_SCORE = 1000;	// アザラシのスコア
	const int	VALUE_SEALS_SCORE = 4;	// アザラシの桁数
	const float	SCORE_POS_X = 200.0f;	// スコアのX座標
	const float	SCORE_POS_Y = 150.0f;	// スコアのY座標
}

//=====================================================
// コンストラクタ
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_Col = NORMAL_COLOR;
	m_nValue = 0;
	m_nScore = 0;
	m_State = STATE_BESIDE;
	m_nCntState = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CDestroyScore::~CDestroyScore()
{

}

//=====================================================
// 生成処理
//=====================================================
CDestroyScore* CDestroyScore::Create(CEnemy::TYPE type)
{
	CDestroyScore* pScore = nullptr;

	pScore = new CDestroyScore;

	if (pScore != nullptr)
	{// 初期化
		pScore->Init();

		pScore->SetScaleNumber(SCORE_SCALE);

		//敵ごとのスコアの設定
		pScore->SetEnemyScore(type);

		//情報の設定
		pScore->SetScore(pScore->m_nValue);
	}

	return pScore;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// スコアの初期化
	m_nValue = VALUE_SCORE; //桁数の初期化
	m_fScaleNumber = SCORE_SCALE;	// 初期スケール設定
	m_State = STATE_WAIT;	//状態の初期化


	// 初期位置の設定
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDestroyScore::Uninit(void)
{
	if (m_aNumber3D != nullptr)
	{
		m_aNumber3D->Uninit();
	}

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CDestroyScore::Update(void)
{
	if (CPlayer::GetInstance().empty())
	{
		return;
	}

	D3DXVECTOR3 pos = (*CPlayer::GetInstance().begin())->GetPosition();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetPosition(D3DXVECTOR3(pos.x - SCORE_POS_X,pos.y,pos.z + SCORE_POS_Y));

	//コンボUIの状態
	switch (m_State)
	{
	case STATE_BESIDE:

		/*if (pos.x < GOAL_X)
		{
			move.x = MOVE_SPEED;

			pos.x += move.x;

			SetPosition(pos);
		}
		else
		{*/

		m_State = STATE_VERTICAL;
		//}

		break;

	case STATE_VERTICAL:

		/*if (pos.x >= GOAL_X)
		{
			move.y += MOVE_SPEED;

			pos.y -= move.y;

			SetPosition(pos);

			if (pos.y < VERTICAL_STOP)
			{*/

		m_State = STATE_WAIT;
		/*	}
		}*/

		break;

	case STATE_WAIT:

		m_nCntState++;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		/*if (pos.x > GOAL_Y)
		{*/

		/*move.y = SLOW_MOVE;

		pos.y -= move.y;

		SetPosition(pos);*/

		m_Col.a -= THINITY_SPEED;

		SetColor(D3DXCOLOR(m_Col));

		//}
		if (m_Col.a <= THINITY_COL)
		{
			CGame::GetInstance()->GetScore()->AddScore(m_nScore);

			Uninit();
		}

		break;
	}

	UpdateNumber();
}

//=====================================================
// 数字の更新
//=====================================================
void CDestroyScore::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber3D == nullptr)
		return;

	//// 値の用意
	//int aValue[SCORE_DIGIT] =
	//{
	//	(m_nScore % 1000000 / 100000),
	//	(m_nScore % 100000 / 10000),
	//	(m_nScore % 10000 / 1000),
	//	(m_nScore % 1000 / 100),
	//	(m_nScore % 100 / 10),
	//	(m_nScore % 10),
	//};

	//std::vector<int> value;

	//value.resize(m_aNumber3D->GetNumAll());

	//for (int nCnt = 0; nCnt < (int)m_aNumber3D->GetNumAll(); nCnt++)
	//{
	//	// 値を計算
	//	value[nCnt] = (m_nCombo % (int)(pow(10, (m_aNumber3D->GetNumAll() - (nCnt)))) / (int)(pow(10, (m_aNumber3D->GetNumAll() - (nCnt + 1)))));
	//}

	// スコアの加算========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_SCORE))
	{// スコアを加算する

	}

	m_aNumber3D->SetValue(m_nScore, m_aNumber3D->GetNumPlace());

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在のスコア：[%d]", m_nScore);
	//CDebugProc::GetInstance()->Print("\nスコアの位置：[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

////=====================================================
//// 色の設定
////=====================================================
//void CUI_Combo::SetColor(E_Number number, D3DXCOLOR col)
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
//=====================================================
// 色の設定
//=====================================================
void CDestroyScore::SetColor(D3DXCOLOR col)
{
	m_aNumber3D->SetColor(col);
}

//=====================================================
// 位置の設定
//=====================================================
void CDestroyScore::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 数字のスケールの設定
//=====================================================
void CDestroyScore::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 描画処理
//=====================================================
void CDestroyScore::Draw()
{

}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CDestroyScore::TransformNumber()
{
	if (m_aNumber3D == nullptr)
		return;

	// 数字のサイズ
	D3DXVECTOR2 Size = SIZE_NORMAL_NUM * m_fScaleNumber;

	D3DXVECTOR3 posBase = GetPosition();

	//// パラメーター設定
	//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// サイズに応じて数字間のスペースをあける

	//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

	// パラメーター設定
	m_aNumber3D->SetPosition(posBase);
	m_aNumber3D->SetSizeAll(Size.x, Size.y);

}

//=====================================================
// 情報の設定
//=====================================================
void CDestroyScore::SetScore(int nDigit)
{
	m_aNumber3D = CNumber3D::Create(nDigit, 0);	// 数字の生成

	// 数字のトランスフォームの設定
	TransformNumber();

}

//=====================================================
// セットスコア処理
//=====================================================
void CDestroyScore::SetEnemyScore(CEnemy::TYPE type)
{
	switch (type)
	{
	case CEnemy::TYPE_SEALS:

		m_nScore = ADD_SEALS_SCORE;
		m_nValue = VALUE_SEALS_SCORE;

		break;

	default:
		break;
	}
}