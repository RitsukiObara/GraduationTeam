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
#include "UI_combo.h"
//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CDestroyScore* CDestroyScore::s_pDestroyScore;	// 格納用の配列

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// 最少スコア
	const float DIST_NUMBER = 0.01f;	// 数字間の距離
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 30.0f, 30.0f };	// 通常数字のサイズ
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// 初期位置
	const int	WAITTIME = 60;	// 滞留時間
	const float	GOAL_X = 0.5f;	// Xのゴール地点
	const float	MOVE_SPEED = 10.0f;	// 移動速度
	const float	VERTICAL_STOP = 100.0f;	// 縦移動の停止地点
	const float	SLOW_MOVE = 1.0f;	// スロー速度
	const float	THINITY_SPEED = 0.02f;	// 透明になっていく速度
	const float	GOAL_Z = 100.0f;	// Yのゴール地点
	const float	THINITY_COL = 0.0f;	// 透明になる
	const float	SCORE_SCALE = 1.0f;	// スコアのスケール
	const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,1.0f,1.0f };	// スコアの初期色
	const int	ADD_SEALS_SCORE = 1000;	// アザラシのスコア
	const int	VALUE_SEALS_SCORE = 5;	// アザラシの桁数
	const float	SCORE_POS_X = 150.0f;	// スコアのX座標
	const float	SCORE_POS_Z = 50.0f;	// スコアのZ座標
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
	m_ShiftPos = POS_INITIAL;
	m_nAddScore = 0;
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
CDestroyScore* CDestroyScore::Create()
{
	if (s_pDestroyScore == nullptr)
	{
		s_pDestroyScore = new CDestroyScore;

		s_pDestroyScore->Init();

		s_pDestroyScore->SetScaleNumber(SCORE_SCALE);

		//情報の設定
		s_pDestroyScore->SetScore(s_pDestroyScore->m_nValue);

		s_pDestroyScore->SetColor(NORMAL_COLOR);
	}
	
	return s_pDestroyScore;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// スコアの初期化
	m_nValue = VALUE_SEALS_SCORE; //桁数の初期化
	m_fScaleNumber = SCORE_SCALE;	// 初期スケール設定
	m_State = STATE_VERTICAL;	//状態の初期化
	m_ShiftPos = D3DXVECTOR3(0.0f, 0.0f, SCORE_POS_Z);


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

	s_pDestroyScore = nullptr;
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

	//コンボUIの状態
	switch (m_State)
	{
	case STATE_BESIDE:

		m_State = STATE_VERTICAL;

		break;

	case STATE_VERTICAL:

		m_ShiftPos.z += MOVE_SPEED;

		if (m_ShiftPos.z >= VERTICAL_STOP)
		{
			m_State = STATE_WAIT;
		}

		break;

	case STATE_WAIT:

		m_nCntState++;

		m_Col = NORMAL_COLOR;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		m_ShiftPos.z += SLOW_MOVE;

		m_Col.a -= THINITY_SPEED;

		if (m_Col.a <= THINITY_COL)
		{
			

			return;
		}

		break;
	}

	SetColor(D3DXCOLOR(m_Col));

	SetPosition(D3DXVECTOR3(pos.x - SCORE_POS_X, pos.y, pos.z + m_ShiftPos.z));

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

	m_aNumber3D->SetValue(m_nScore, m_aNumber3D->GetNumPlace());

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在のスコア：[%d]", m_nScore);
	//CDebugProc::GetInstance()->Print("\nスコアの位置：[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

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

		m_nAddScore = ADD_SEALS_SCORE;

		break;

	default:
		break;
	}
}

//=====================================================
// 敵を倒した時のスコア増加
//=====================================================
void CDestroyScore::AddDestroyScore(CEnemy::TYPE type)
{
	//敵ごとのスコアの設定
	SetEnemyScore(type);

	m_nScore += m_nAddScore;

	m_State = STATE_WAIT;

	m_nCntState = 0;
}

//=====================================================
// コンボスコアを計算して加算
//=====================================================
void CDestroyScore::AddComboScore(void)
{
	CUI_Combo *pUICombo = CUI_Combo::GetInstance();
	if (pUICombo == nullptr)
		return;

	int nCombo = pUICombo->GetCombo();

	//コンボ倍率とのスコア計算
	m_nScore = m_nScore * nCombo;

	//スコアを加算
	game::AddScore(m_nScore);

	Uninit();
}

//=====================================================
// 敵を倒した時のスコアインスタンス取得
//=====================================================
CDestroyScore* CDestroyScore::GetInstance()
{
	Create();

	return s_pDestroyScore;
}