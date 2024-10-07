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
#include "manager.h"
#include "game.h"
#include "enemy.h"
#include "polygon3D.h"
#include "debugproc.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// ボディのパス
const float TIME_INPUT_BOTHHAND = 0.3f;	// 両手攻撃の猶予
const D3DXVECTOR3 ROT_INITIAL = { 0.0f, D3DX_PI * 0.5f, 0.0f };	// 初期の向き
const D3DXVECTOR3 OFFSET_GUIDE = { 0.0f, 400.0f, 0.0f };	// ガイド表示のオフセット位置
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
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
	// 継承クラスの初期化
	CRikishi::Init();

	InitPose(MOTION_GUARD);

	// 向き
	SetRotation(ROT_INITIAL);

	m_pGuide = CPolygon3D::Create(OFFSET_GUIDE);
	m_pGuide->SetRotation(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	m_pGuide->SetSize(200.0f, 100.0f);

	int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\you.png");
	m_pGuide->SetIdxTexture(nIdx);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// 継承クラスの終了
	CRikishi::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// 入力処理
	Input();

	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	D3DXVECTOR3 posMid = pGame->GetPosMid();
	D3DXVECTOR3 pos = posMid;
	pos.x += Rikishi::DISTANCE_RIKISHI;

	SetPosition(pos);

	CRikishi::Update();

	if (m_pGuide != nullptr)
	{
		m_pGuide->SetMatrixParent(GetMatrix());
	}
}

//=====================================================
// 入力
//=====================================================
void CPlayer::Input(void)
{
	if (CGame::GetState() != CGame::STATE::STATE_NORMAL)
		return;

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_LEFTATTACK))
	{
		Attack(CRikishi::E_Attack::ATTACK_LEFT);

		m_aTimerAttack[CRikishi::ATTACK_LEFT] = TIME_INPUT_BOTHHAND;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_RIGHTATTACK))
	{
		Attack(CRikishi::E_Attack::ATTACK_RIGHT);

		m_aTimerAttack[CRikishi::ATTACK_RIGHT] = TIME_INPUT_BOTHHAND;
	}

	// 両手攻撃の判定
	if (m_aTimerAttack[CRikishi::ATTACK_LEFT] > 0 &&
		m_aTimerAttack[CRikishi::ATTACK_RIGHT] > 0)
	{
		Attack(CRikishi::E_Attack::ATTACK_BOTHHAND);

		m_aTimerAttack[CRikishi::ATTACK_LEFT] = -1.0f;
		m_aTimerAttack[CRikishi::ATTACK_RIGHT] = -1.0f;
	}

	// 攻撃タイマーの減算
	for (int i = 0; i < CRikishi::ATTACK_MAX; i++)
	{
		if (m_aTimerAttack[i] > 0)
		{
			m_aTimerAttack[i] -= CManager::GetDeltaTime();
		}
	}

	CDebugProc::GetInstance()->Print("\nカウンター[%f]", m_aTimerAttack[0]);
	CDebugProc::GetInstance()->Print("\nカウンター[%f]", m_aTimerAttack[1]);
}

//=====================================================
// 攻撃
//=====================================================
void CPlayer::Attack(E_Attack typeAttack)
{
	CRikishi::Attack(typeAttack);
}

//=====================================================
// 攻撃命中時の処理
//=====================================================
void CPlayer::HitAttack(E_Attack typeAttack)
{
	CEnemy *pEnemy = CEnemy::GetInstance();

	if (pEnemy == nullptr)
		return;

	float fKnockBack = pEnemy->Hit(typeAttack, Rikishi::DAMAGE_ATTACK);
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CRikishi::Draw();
}

//=====================================================
// ヒット処理
//=====================================================
float CPlayer::Hit(CRikishi::E_Attack typeAttack, float fDamage)
{
	float fKnockBack = CRikishi::Hit(typeAttack, fDamage);

	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return 0.0f;

	D3DXVECTOR3 addPos = { fKnockBack,0.0f,0.0f };

	pGame->AddPosMid(addPos);

	return 0.0f;
}