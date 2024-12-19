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
#include "player.h"
#include "UI_combo.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// 最少スコア
	const float DIST_NUMBER = 0.01f;	// 数字間の距離
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 30.0f, 30.0f };	// 通常数字のサイズ
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// 初期位置
	const float	GOAL_X = 0.5f;	// Xのゴール地点
	const float	MOVE_SPEED = 10.0f;	// 移動速度
	const float	VERTICAL_STOP = 100.0f;	// 縦移動の停止地点
	const float	GOAL_Z = 100.0f;	// Yのゴール地点
	const float	SCORE_SCALE = 1.0f;	// スコアのスケール
	const float	NORMAL_ALPHA = 1.0f;	// スコアの初期透明度
	const int	ADD_SCORE[CEnemy::TYPE_MAX] =				// アザラシのスコア
	{
		1000,		// スコア
		2000,		// シロクマ
	};
	const int	VALUE_SCORE = 5;		// 桁数
	const float	SCORE_POS_X = 150.0f;	// スコアのX座標
	const float	SCORE_POS_Z = 50.0f;	// スコアのZ座標
}

//=====================================================
// コンストラクタ
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_nScore = 0;
	m_state = CUI_Combo::STATE_WAIT;
	m_ShiftPos = POS_INITIAL;
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
	CDestroyScore* pScore = new CDestroyScore;

	if (pScore != nullptr)
	{
		pScore->Init();

		pScore->SetScaleNumber(SCORE_SCALE);

		//情報の設定
		pScore->SetScore();

		pScore->SetAlpha(NORMAL_ALPHA);
	}
	
	return pScore;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// スコアの初期化
	m_fScaleNumber = SCORE_SCALE;	// 初期スケール設定
	m_state = CUI_Combo::STATE_WAIT;	// 状態の初期化
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
		m_aNumber3D = nullptr;
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
#endif
#endif
}

//=====================================================
// 透明度の設定
//=====================================================
void CDestroyScore::SetAlpha(float fAlpha)
{
	if (m_aNumber3D != nullptr)
		m_aNumber3D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
}

//=====================================================
// 状態の設定
//=====================================================
void CDestroyScore::SetState(CUI_Combo::E_State state)
{
	m_state = state;
}

//=====================================================
// ずらす幅の設定
//=====================================================
void CDestroyScore::SetShiftPos(D3DXVECTOR3 shiftpos)
{
	m_ShiftPos = shiftpos;
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
void CDestroyScore::SetScore(void)
{
	m_aNumber3D = CNumber3D::Create(VALUE_SCORE, 0);	// 数字の生成

	// 数字のトランスフォームの設定
	TransformNumber();
}

//=====================================================
// スコアの取得
//=====================================================
int CDestroyScore::GetScore(void)
{
	return m_nScore;
}

//=====================================================
// 敵を倒した時のスコア増加
//=====================================================
void CDestroyScore::AddDestroyScore(CEnemy::TYPE type)
{
	// 種類が存在しない場合、この関数を抜ける
	if (type >= CEnemy::TYPE_MAX) { assert(false); return; }

	m_nScore += ADD_SCORE[type];
}
