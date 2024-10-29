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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int	WAITTIME = 60;	// 滞留時間
	const float	GOAL_X = 0.2f;	// Xのゴール地点
	const float	MOVE_SPEED = 0.01f;	// 移動速度
	const float	VERTICAL_STOP = 0.15f;	// 縦移動の停止地点
	const float	SLOW_MOVE = 0.001f;	// スロー速度
	const float	THINITY_SPEED = 0.02f;	// 透明になっていく速度
	const float	GOAL_Y = 0.1f;	// Yのゴール地点
	const float	THINITY_COL = 0.0f;	// 透明になる
	const int	ADD_SEALS_SCORE = 1000;	// 追加するスコア(アザラシ)
	const int	VALUE_SEALS_SCORE = 4;	// 追加するスコアの桁数(アザラシ)
	const float	SCORE_SCALE = 0.6f;	// スコアのスケール
	D3DXVECTOR3	SCORE_PLACE = { 0.5f,0.6f,0.0f };	// スコアの場所
}

//=====================================================
// コンストラクタ
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_State = STATE_BESIDE;
	m_nCntState = 0;
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nScore = 0;
	m_nValue = 0;
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
		pScore->SetData(pScore->m_nValue);

		//スコアの設定
		pScore->SetScore(pScore->m_nScore);
	}

	return pScore;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	CScore::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDestroyScore::Uninit(void)
{
	CScore::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CDestroyScore::Update(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(SCORE_PLACE);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//敵倒した時UIの状態
	switch (m_State)
	{
	case STATE_BESIDE:

		/*if (pos.x < GOAL_X)
		{
			move.x += MOVE_SPEED;

			pos.x += move.x;*/

			SetPosition(pos);
		/*}
		else
		{*/
			m_State = STATE_VERTICAL;
		/*}*/

		break;

	case STATE_VERTICAL:

		/*if (pos.x > GOAL_X)
		{
			move.y += MOVE_SPEED;

			pos.y -= move.y;

			SetPosition(pos);

			if (pos.y < VERTICAL_STOP)
			{*/
				/*SetPosition(pos);*/

				m_State = STATE_WAIT;
			/*}*/
		/*}*/

		break;

	case STATE_WAIT:

		m_nCntState++;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		
			//動きをゆっくり
			move.y += SLOW_MOVE;

			pos.y -= move.y;

			SetPosition(pos);

			//色を透明にしていく
			m_Col.a -= THINITY_SPEED;

			SetColor(m_Col);
		
		if (pos.y <= GOAL_Y || m_Col.a <= THINITY_COL)
		{
			//スコア加算
			CGame::GetInstance()->GetScore()->AddScore(m_nScore);

			//終了
			Uninit();
		}

		break;
	}

	CScore::Update();
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
void CDestroyScore::Draw()
{
	CScore::Draw();
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