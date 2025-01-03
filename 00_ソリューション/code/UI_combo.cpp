//*****************************************************
//
// コンボUIの処理[UI_combo.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UI_combo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI.h"
#include "game.h"
#include "inputManager.h"
#include "debugproc.h"
#include "player.h"
#include "destroy_score.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CUI_Combo* CUI_Combo::s_pCombo;	// 格納用の配列

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int	COMBO_MIN = 0;	// 最少コンボ
const int	COMBO_MAX = 99;	// 最大コンボ
const float DIST_NUMBER = 0.01f;	// 数字間の距離
const D3DXVECTOR2 SIZE_NORMAL_NUM = { 40.0f, 40.0f };	// 通常数字のサイズ
const D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// ミニ数字のサイズ
const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// 初期位置
const int	WAITTIME = 100;	// 滞留時間
const float	GOAL_X = 0.5f;	// Xのゴール地点
const float	MOVE_SPEED = 10.0f;	// 移動速度
const float	VERTICAL_STOP = 100.0f;	// 縦移動の停止地点
const float	SLOW_MOVE = 1.0f;	// スロー速度
const float	THINITY_SPEED = 0.02f;	// 透明になっていく速度
const float	GOAL_Z = 100.0f;	// Yのゴール地点
const float	THINITY_COL = 0.0f;	// 透明になる
const int	VALUE_COMBO = 2;	// 追加するスコアの桁数(アザラシ)
const float	COMBO_SCALE = 0.7f;	// スコアのスケール
const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,0.0f,1.0f };	// スコアの初期色
const float	COMBO_POS_X = 50.0f;	// コンボのX座標
const float	COMBO_POS_Z = 70.0f;	// コンボのZ座標
}

//=====================================================
// コンストラクタ
//=====================================================
CUI_Combo::CUI_Combo()
{
	m_pScore = nullptr;
	m_Col = NORMAL_COLOR;
	m_nValue = 0;
	m_nCombo = 0;
	m_State = STATE_WAIT;
	m_nCntState = 0;
	m_ShiftPos = POS_INITIAL;
}

//=====================================================
// デストラクタ
//=====================================================
CUI_Combo::~CUI_Combo()
{

}

//=====================================================
// 生成処理
//=====================================================
CUI_Combo* CUI_Combo::Create()
{
	if (s_pCombo == nullptr)
	{
		s_pCombo = new CUI_Combo;

		s_pCombo->Init();

		s_pCombo->SetScaleNumber(COMBO_SCALE);

		//情報の設定
		s_pCombo->SetCombo(VALUE_COMBO);

		s_pCombo->SetColor(NORMAL_COLOR);
	}

	return s_pCombo;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUI_Combo::Init(void)
{
	m_pScore = CDestroyScore::Create();	// スコア
	m_nCombo = COMBO_MIN;	// スコアの初期化
	m_nValue = VALUE_COMBO;	// 桁数の初期化
	m_fScaleNumber = COMBO_SCALE;	// 初期スケール設定
	m_State = STATE_WAIT;	// 状態の初期化
	m_ShiftPos = D3DXVECTOR3(0.0f, 0.0f, COMBO_POS_Z);

	// 初期位置の設定
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUI_Combo::Uninit(void)
{
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	if (m_aNumber3D != nullptr)
	{
		m_aNumber3D->Uninit();
		m_aNumber3D = nullptr;
	}

	CGameObject::Uninit();

	s_pCombo = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CUI_Combo::Update(void)
{
	if (CPlayer::GetInstance().empty())
		return;

	D3DXVECTOR3 pos = (*CPlayer::GetInstance().begin())->GetPosition();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//コンボUIの状態
	switch (m_State)
	{
	case STATE_WAIT:

		m_nCntState++;

		m_Col = NORMAL_COLOR;

		if (m_nCntState >= WAITTIME)
			m_State = STATE_ERASE;

		break;

	case STATE_ERASE:

		m_ShiftPos.z += SLOW_MOVE;

		m_Col.a -= THINITY_SPEED;

		if (m_Col.a <= THINITY_COL)
		{
			// コンボ計算処理
			AddComboScore();

			Uninit();

			return;
		}

		break;
	}

	if (m_pScore != nullptr)
	{
		m_pScore->Update();
		m_pScore->SetState(m_State);
		m_pScore->SetShiftPos(m_ShiftPos);
		m_pScore->SetAlpha(m_Col.a);
	}

	SetColor(m_Col);

	SetPosition(D3DXVECTOR3(pos.x + COMBO_POS_X, pos.y, pos.z + m_ShiftPos.z + COMBO_POS_Z));

	UpdateNumber();
}

//=====================================================
// 数字の更新
//=====================================================
void CUI_Combo::UpdateNumber()
{
	if (m_aNumber3D == nullptr)
		return;

	m_aNumber3D->SetValue(m_nCombo, m_aNumber3D->GetNumPlace());

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n現在のコンボ：[%d]", m_nCombo);
#endif
#endif
}

//=====================================================
// 色の設定
//=====================================================
void CUI_Combo::SetColor(D3DXCOLOR col)
{
	if (m_aNumber3D == nullptr)
		return;

	m_aNumber3D->SetColor(col);
}

//=====================================================
// 位置の設定
//=====================================================
void CUI_Combo::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 数字のスケールの設定
//=====================================================
void CUI_Combo::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// 描画処理
//=====================================================
void CUI_Combo::Draw()
{
	
}

//=====================================================
// 数字のトランスフォーム設定
//=====================================================
void CUI_Combo::TransformNumber()
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
void CUI_Combo::SetCombo(int nDigit)
{
	m_aNumber3D = CNumber3D::Create(nDigit, 0);	// 数字の生成

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// コンボの増加
//=====================================================
void CUI_Combo::AddCombo(CEnemy::TYPE type)
{
	if (CGame::GetState() != CGame::E_State::STATE_NORMAL)
		return;

	if (m_pScore != nullptr)
	{
		m_pScore->AddDestroyScore(type);
	}

	m_nCombo++;

	m_State = STATE_WAIT;

	m_nCntState = 0;

	m_ShiftPos = POS_INITIAL;
}

//=====================================================
// コンボスコアを計算して加算
//=====================================================
void CUI_Combo::AddComboScore(void)
{
	if (m_pScore == nullptr)
		return;

	//コンボ倍率とのスコア計算
	int nScore = m_pScore->GetScore() * m_nCombo;

	//スコアを加算
	game::AddScore(nScore);
}


//=====================================================
// コンボのインスタンス取得
//=====================================================
CUI_Combo* CUI_Combo::GetInstance()
{
	Create();

	return s_pCombo;
}