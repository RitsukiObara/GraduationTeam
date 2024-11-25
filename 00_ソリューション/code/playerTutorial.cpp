//*****************************************************
//
// チュートリアルプレイヤーの処理[playerTutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "playerTutorial.h"
#include "manager.h"
#include "inputManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
//------------------------------
// 移動の定数
//------------------------------
namespace move
{
const float LINE_SPPED_MOVE = 2.0f;	// 移動してるとみなすしきい値
const float TIME_COMPLETE = 2.0f;	// 完了までかかる時間
}
}

//=====================================================
// コンストラクタ
//=====================================================
CPlayerTutorial::CPlayerTutorial(int nPriority) : CPlayer(nPriority), m_fTimerMove(0.0f), m_bComplete(false)
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayerTutorial* CPlayerTutorial::Create(void)
{
	CPlayerTutorial *pPlayerTutorial = nullptr;

	pPlayerTutorial = new CPlayerTutorial;

	if (pPlayerTutorial != nullptr)
		pPlayerTutorial->Init();

	return pPlayerTutorial;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerTutorial::Init(void)
{
	// 継承クラスの初期化
	CPlayer::Init();
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerTutorial::Uninit(void)
{
	// 継承クラスの終了
	CPlayer::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerTutorial::Update(void)
{
	// 継承クラスの更新
	CPlayer::Update();

	// スピードの確認
	CheckSpeed();

	// 説明読了処理
	ReadExplain();
}

//=====================================================
// スピードの確認
//=====================================================
void CPlayerTutorial::CheckSpeed(void)
{
	D3DXVECTOR3 move = GetMove();

	float fSpeed = D3DXVec3Length(&move);

	if (fSpeed > move::LINE_SPPED_MOVE)
	{// 一定速度を越えていたらタイマー進行
		m_fTimerMove += CManager::GetDeltaTime();

		// 一定時間移動で進行
		if (m_fTimerMove > move::TIME_COMPLETE)
			AddProgress(CTutorial::E_State::STATE_MOVE);
	}
}

//=====================================================
// 説明読了処理
//=====================================================
void CPlayerTutorial::ReadExplain(void)
{
	CInputManager *pInputMgr = GetInpuManager();

	if (pInputMgr == nullptr)
		return;

	if(pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		AddProgress(CTutorial::E_State::STATE_EXPLAIN_BREAK);

	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		AddProgress(CTutorial::E_State::STATE_EXPLAIN_ENEMY);
}

//=====================================================
// 突っつく処理
//=====================================================
bool CPlayerTutorial::Peck(void)
{
	// チュートリアル進行
	AddProgress(CTutorial::E_State::STATE_PECK);

	// 継承クラスの突っつき
	CPlayer::Peck();

	return false;
}

//=====================================================
// チュートリアルの進行
//=====================================================
void CPlayerTutorial::AddProgress(CTutorial::E_State state)
{
	CTutorial *pTutorial = CTutorial::GetInstance();
	if (pTutorial == nullptr)
		return;

	CTutorial::E_State stateCur = pTutorial->GetState();
	
	// 指定した状態とチュートリアルの状態が一致したら進行させる
	if (stateCur == state)
	{
		pTutorial->AddCntProgress(this);

		m_bComplete = true;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerTutorial::Draw(void)
{
	// 継承クラスの描画
	CPlayer::Draw();
}