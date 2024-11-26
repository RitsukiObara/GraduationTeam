//*****************************************************
//
// モード選択ペンギン[selectmodepenguin.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguinstate_selectmode.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace Stand
	{
		const int MOTION_COUNT = 60 * 3;	// モーションをする時間
		const vector<CNPCPenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_WALK,		// 移動
			CNPCPenguin::MOTION_NECKSHAKE,	// 首振り
			CNPCPenguin::MOTION_STOMACH,		// 腹ベタァ
			CNPCPenguin::MOTION_UNYO,		// 首うにょん
			CNPCPenguin::MOTION_WINGPTPT		// 羽パタ
		};
	}
	namespace Move
	{
		const int MOTION_COUNT = 60 * 5 + 20;	// モーションをする時間
		const int FALL_CHANCE = 18;			// コケ確率（値：％）
		const float MOVE_SPEED = 2.5f;		// 移動速度
	}
	namespace ShakeHead
	{
		const int MOTION_COUNT = 60 * 2;	// モーションをする時間
	}
	namespace Stomach
	{
		const int MOTION_COUNT = 60 * 3;	// モーションをする時間
	}
}

//*****************************************************
// 立ってるだけステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Stand::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Stand::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Stand::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stand::MOTION_COUNT)
	{// ステート時間終了
		// どのステートに遷移するか抽選
		CNPCPenguin::MOTION rnd = Stand::NEXT_MOTION_PATTERN[(rand() % (int)Stand::NEXT_MOTION_PATTERN.size())];
		switch (rnd)
		{
		case CNPCPenguin::MOTION_WALK:
			pPenguin->SetState(new CNPCPenguinState_Move);
			break;
		case CNPCPenguin::MOTION_NECKSHAKE:
			pPenguin->SetState(new CNPCPenguinState_ShakeHead);
			break;
		case CNPCPenguin::MOTION_STOMACH:
			pPenguin->SetState(new CNPCPenguinState_Stomach);
			break;
		case CNPCPenguin::MOTION_UNYO:
			pPenguin->SetState(new CNPCPenguinState_UNYO);
			break;
		case CNPCPenguin::MOTION_WINGPTPT:
			pPenguin->SetState(new CNPCPenguinState_WingPTPT);
			break;
		default:
			m_nCounter = 0;
			break;
		}
	}
}

//*****************************************************
// 移動ステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Move::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_WALK);

	CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_VOICE01);	// ペンギンの羽ばたき音
	CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_WALKING);  // ペンギンの歩く音

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Move::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Move::Update(CNPCPenguin* pPenguin)
{
	// 移動
	D3DXVECTOR3 rot = pPenguin->GetRotation();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	move.x = -sinf(rot.y) * Move::MOVE_SPEED;
	move.z = -cosf(rot.y) * Move::MOVE_SPEED;

	pPenguin->Translate(move);

	// 回る
	rot.y += 0.005f * D3DX_PI;
	pPenguin->SetRotation(rot);

	// ステート終了カウント
	m_nCounter++;
	if (m_nCounter >= Move::MOTION_COUNT)
	{// ステート時間終了
		// コケチャンス
		int rnd = rand() % 100;
		if (rnd < Move::FALL_CHANCE)
		{// コケ
			pPenguin->SetState(new CNPCPenguinState_Fall);
		}
		else
		{// 普通に立つ
			pPenguin->SetState(new CNPCPenguinState_Stand);
		}
	}
}

//*****************************************************
// 転びステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Fall::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_FALL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Fall::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Fall::Update(CNPCPenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// コケ終わった
		pPenguin->SetState(new CNPCPenguinState_Stand);
	}
}

//*****************************************************
// 首振りステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_ShakeHead::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NECKSHAKE);

	// ペンギンの羽ばたき音
	CSound::GetInstance()->Play(CSound::LABEL_SE_FLAPPING_WING);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_ShakeHead::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_ShakeHead::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= ShakeHead::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CNPCPenguinState_Stand);
	}
}

//*****************************************************
// 腹ベタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Stomach::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_STOMACH);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Stomach::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Stomach::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stomach::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CNPCPenguinState_Stand);
	}
}

//*****************************************************
// 首うねステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_UNYO::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_UNYO);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_UNYO::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_UNYO::Update(CNPCPenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 首うにょん終わった
		pPenguin->SetState(new CNPCPenguinState_Stand);
	}
}

//*****************************************************
// 羽パタパタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_WingPTPT::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_WINGPTPT);

	// ペンギンの羽ばたき音
	CSound::GetInstance()->Play(CSound::LABEL_SE_FLAPPING_WING);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_WingPTPT::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_WingPTPT::Update(CNPCPenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 羽パタ終わった
		pPenguin->SetState(new CNPCPenguinState_Stand);
	}
}
