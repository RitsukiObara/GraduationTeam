//*****************************************************
//
// マルチ用の硬い氷の処理[iceHardMulti.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceHardMulti.h"
#include "player.h"
#include "iceManager.h"
#include "manager.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "MyEffekseer.h"
#include "motion.h"
#include "seals.h"
#include "particle.h"
#include "game.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_JUMP = 3.0f;						// 飛ばすまでの時間
const float SPEED_REPAIR = 0.3f;					// 回復の速度(1.0fで進行と同じ速度)
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// 初期の色
const float SIZE_COLLIDE = Grid::SIZE * 0.5f;		// 判定のサイズ

const float POW_VIB_JUMP = 0.6f;	// 飛びそうな時の振動強さ
const int TIME_VIB_JUMP = 10;		// 飛びそうな時の振動時間

const float POW_VIB_BLOW = 1.0f;	// 吹き飛ばしの振動強さ
const int TIME_VIB_BLOW = 60;		// 吹き飛ばしの振動時間

const string PATH_SEAL = "data\\MOTION\\motionSeals.txt";	// アザラシのパス
const float HEIGHT_SEAL = -50.0f;							// アザラシの高さ

//--------------------------------
// ジャンプの定数
//--------------------------------
namespace jump
{
const float POW = 30.0f;		// ジャンプ力
const float HEIGHT = 500.0f;	// 高さ
const float TIME = 1.0f;		// 時間
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CIceHardMulti::CIceHardMulti(int nPriority) : CIceHard(nPriority), m_state(E_State::STATE_NONE), m_fTimerJump(0.0f), m_pSeal(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceHardMulti::~CIceHardMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceHardMulti::Init(void)
{
	CIceHard::Init();

	// 状態の初期化
	m_state = E_State::STATE_NORMAL;

	// アザラシの生成
	m_pSeal = CMotion::Create((char*)(&PATH_SEAL[0]));

	if (m_pSeal != nullptr)
	{
		// ポーズ初期化
		m_pSeal->InitEndPose(CSeals::E_Motion::MOTION_BLOW);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CIceHardMulti::Uninit(void)
{
	CIceHard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIceHardMulti::Update(void)
{
	// プレイヤーが乗ってる判定
	IsOnPlayer();

	// 状態ごとの更新
	UpdateState();

	// アザラシの追従
	FollowSeal();

	// 継承クラスの更新
	CIceHard::Update();
}

//=====================================================
// プレイヤーが乗ってる判定
//=====================================================
void CIceHardMulti::IsOnPlayer(void)
{
	if (CGame::GetState() != CGame::E_State::STATE_NORMAL)
		return;

	// 配列のクリア
	m_apPlayerJump.clear();

	// プレイヤーの取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		if (pPlayer->GetState() == CPlayer::E_State::STATE_BLOW)
			continue;

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer, SIZE_COLLIDE, nullptr))
		{// 一定距離以内にいたらカウンターを加算
			m_fTimerJump += CManager::GetDeltaTime();

			// コントローラーを振動させる
			float fRate = m_fTimerJump / TIME_JUMP;
			pPlayer->VibJoypad(POW_VIB_JUMP * fRate, TIME_VIB_JUMP);

			m_apPlayerJump.push_back(pPlayer);
		}
	}

	if (m_apPlayerJump.empty())
	{// 誰も乗ってなかったらタイマー減算
		m_fTimerJump -= CManager::GetDeltaTime() * SPEED_REPAIR;
	}
}

//=====================================================
// 状態ごとの更新
//=====================================================
void CIceHardMulti::UpdateState(void)
{
	switch (m_state)
	{
	case CIceHardMulti::STATE_NORMAL:	// 通常更新
		UpdateNormal();
		break;
	case CIceHardMulti::STATE_JUMP:	// ジャンプ中の更新
		UpdateJump();
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================================
// 通常の更新
//=====================================================
void CIceHardMulti::UpdateNormal(void)
{
	if (m_fTimerJump > TIME_JUMP)
	{// 一定時間乗ってたらジャンプさせる
		StartJump();

		// ジャンプするまでの時間をリセット
		m_fTimerJump = 0.0f;
	}

	// 値の補正
	universal::LimitValuefloat(&m_fTimerJump, TIME_JUMP, 0.0f);

	// 色の設定
	D3DXCOLOR col = COL_INIT;

	// 割合の計算
	float fRate = m_fTimerJump / TIME_JUMP;

	// 赤以外の色を割合分抜いていく
	col.g -= fRate;
	col.b -= fRate;

	// 各メッシュに色を設定
	CMeshCylinder *pCylinder = GetMeshCyliner();
	CFan3D *pFan = GetFan3D();

	if (pCylinder == nullptr || pFan == nullptr)
		return;

	pCylinder->SetCol(col);
	pFan->SetCol(col);
}

//=====================================================
// ジャンプ中の更新
//=====================================================
void CIceHardMulti::UpdateJump(void)
{
	if (m_fTimerMove > jump::TIME)
	{// 時間を越えたら通常に戻る
		m_fTimerMove = 0.0f;
		m_state = E_State::STATE_NORMAL;
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE_ICEJUMP, GetPosition(), GetRotation(), D3DXVECTOR3(100.0f, 100.0f, 100.0f));

		// さざ波の生成
		CreateRipple();

		// トランスフォームの設定
		SetHeightOcean(0.0f);
		SetTransform(Grid::SIZE);

		return;
	}

	m_fTimerMove += CManager::GetDeltaTime();

	// 時間を正規化
	float t = m_fTimerMove / jump::TIME;

	universal::LimitValuefloat(&t, 1.0f, 0.0f);

	// 高さを計算
	float fHeight = jump::HEIGHT * (1.0f - 4.0f * (t - 0.5f) * (t - 0.5f));

	SetHeightOcean(fHeight);
}

//=====================================================
// ジャンプ開始処理
//=====================================================
void CIceHardMulti::StartJump(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	bool bOther = CheckOtherIceCanBlow();

	for (CPlayer *pPlayer : m_apPlayerJump)
	{
		if (pPlayer == nullptr)
			continue;

		// 上に移動量を与える
		pPlayer->SetMove(D3DXVECTOR3(0.0f, jump::POW, 0.0f));

		// ランダムな氷の取得
		CIce *pIce = nullptr;

		while (true)
		{
			pIce = pIceMgr->GetRandomIce();

			if (pIce == nullptr)
				continue;

			if (bOther && pIce == this)
				continue;

			if (!pIce->IsPeck())
				break;
		}

		// プレイヤー吹き飛ばしの開始
		pPlayer->StartBlow(pIce);

		pPlayer->VibJoypad(POW_VIB_BLOW, TIME_VIB_BLOW);

		// SEの再生
		Sound::Play(CSound::LABEL_SE_BLOW_ICE);
	}

	// 初期値を現在位置に設定
	m_posInit = GetPosition();

	// 状態を設定
	m_state = E_State::STATE_JUMP;

	// アザラシのモーション設定
	if (m_pSeal != nullptr)
		m_pSeal->SetMotion(CSeals::E_Motion::MOTION_BLOW);

	// パーティクルの発生
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_DROP, GetPosition());

	// さざ波の破棄
	DeleteRipple();
}

//=====================================================
// アザラシの追従
//=====================================================
void CIceHardMulti::FollowSeal(void)
{
	if (m_pSeal == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();
	pos.y = HEIGHT_SEAL;
	m_pSeal->SetPosition(pos);
}

//=====================================================
// 自分以外にも飛べる氷があるかチェック
//=====================================================
bool CIceHardMulti::CheckOtherIceCanBlow(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	// 流氷の除外
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });

	// つっつき氷の除外
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && ice->IsPeck(); });

	for (CIce *pIce : apIce)
	{
		if (pIce != this)
			return true;
	}

	return false;
}

//=====================================================
// 描画処理
//=====================================================
void CIceHardMulti::Draw(void)
{
	CIceHard::Draw();
}