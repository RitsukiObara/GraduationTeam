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
#include "iceManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// ボディのパス
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

	CMotion::Update();
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

	// 移動の入力========================================
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
	{
		m_nGridH--;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
	{
		m_nGridH++;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
	{
		m_nGridV++;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
	{
		m_nGridV--;
	}

	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(m_nGridV, m_nGridH);
	SetPosition(posGrid);

	// つつきの入力========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
	{// 乗っている氷を割る
		D3DXVECTOR3 pos = GetPosition();
		pIceManager->PeckIce(pos, CIceManager::E_Direction::DIRECTION_LEFT);	// 割る処理
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