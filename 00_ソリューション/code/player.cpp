//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "player.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "debugproc.h"
#include "particle.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPing-.txt";	// ボディのパス
const int MOVE_FRAME = 25;	// 移動にかかるフレーム数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0)
{
	m_isMove = false;
	m_pPlayer = this;
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// 入力処理
	Input();

	// モーション更新
	CMotion::Update();

	// モーション完了後の処理
	MotionFinishCheck();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 入力
//=====================================================
void CPlayer::Input(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// グリッド取得========================================
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

	// 移動していなければ入力受付========================================
	D3DXVECTOR3 pos = GetPosition();
	if (m_isMove == false)
	{
		// 移動の入力========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
		{
			m_nGridH--;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
		{
			m_nGridH++;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
		{
			m_nGridV++;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
		{
			m_nGridV--;
			m_isMove = true;
		}

		if (m_isMove == true && GetMotion() == 0)
		{
			// ジャンプモーション
			SetMotion(MOTION_JUMPSTART);
		}

		// つつきの入力========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
		{// 乗っている氷を割る
			pIceManager->PeckIce(m_nGridV, m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);	// 割る処理

			CParticle::Create(D3DXVECTOR3(posGrid.x, posGrid.y - 20.0f, posGrid.z), CParticle::TYPE_ICEBREAK, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// 氷割りモーション
			SetMotion(MOTION_PECK);
		}
	}

	if (m_isMove == true && GetMotion() == MOTION_JUMPSTART && IsFinish() == true)
	{// どこか移動した
		// 目標位置設定========================================
		SetPositionDest(posGrid);

		// 移動量設定========================================
		D3DXVECTOR3 move = (posGrid - pos) / MOVE_FRAME;
		move.y = 10.0f;
		SetMove(move);

		// ジャンプモーション
		SetMotion(MOTION_JUMPFLY);
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr)
		return;
	
	pDebugProc->Print("\n縦[%d]横[%d]", m_nGridV, m_nGridH);

	if (pInputKeyboard->GetTrigger(DIK_RSHIFT))
	{
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH);
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RCONTROL))
	{
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH,CIce::E_Type::TYPE_HARD);
		}
	}
}

//=====================================================
// モーション完了後の処理
//=====================================================
void CPlayer::MotionFinishCheck(void)
{
	if (CMotion::IsFinish() == true)
	{// 何かしらのモーション終了
		if (CMotion::GetMotion() == MOTION_LANDING)
		{// 着地終了（通常状態遷移）
			SetMotion(MOTION_NEUTRAL);
			m_isMove = false;
		}
		if (CMotion::GetMotion() == MOTION_PECK)
		{// 氷割り終了（通常状態遷移）
			SetMotion(MOTION_NEUTRAL);
			m_isMove = false;
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}