//*****************************************************
//
// タイトル用NPCペンギンステート[npcpenguinstate_title.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguinstate_title.h"
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
		const int MOTION_COUNT = 150;	// モーションをする時間
		const vector<CNPCPenguin::MOTION> NEXT_MOTION_PATTERN =
		{
			CNPCPenguin::MOTION_NECKSHAKE,		// 首振り
			CNPCPenguin::MOTION_STOMACH,		// 腹ベタァ
			CNPCPenguin::MOTION_UNYO,			// 首うにょん
			CNPCPenguin::MOTION_UNYO,			// 首うにょん
			CNPCPenguin::MOTION_WINGPTPT,		// 羽パタ
			CNPCPenguin::MOTION_WINGPTPT		// 羽パタ
		};
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
HRESULT CNPCPenguinState_StandTitle::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_StandTitle::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_StandTitle::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stand::MOTION_COUNT)
	{// ステート時間終了
		// どのステートに遷移するか抽選
		CNPCPenguin::MOTION rnd = Stand::NEXT_MOTION_PATTERN[(rand() % (int)Stand::NEXT_MOTION_PATTERN.size())];
		switch (rnd)
		{
		case CNPCPenguin::MOTION_NECKSHAKE:
			pPenguin->SetState(new CNPCPenguinState_ShakeHeadTitle);
			break;
		case CNPCPenguin::MOTION_STOMACH:
			pPenguin->SetState(new CNPCPenguinState_StomachTitle);
			break;
		case CNPCPenguin::MOTION_UNYO:
			pPenguin->SetState(new CNPCPenguinState_UNYOTitle);
			break;
		case CNPCPenguin::MOTION_WINGPTPT:
			pPenguin->SetState(new CNPCPenguinState_WingPTPTTitle);
			break;
		default:
			m_nCounter = 0;
			break;
		}
	}
}

//*****************************************************
// 首振りステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_ShakeHeadTitle::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NECKSHAKE);

	// ペンギンの羽ばたき音
	CSound::GetInstance()->Play(CSound::LABEL_SE_FLAPPING_WING);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_ShakeHeadTitle::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_ShakeHeadTitle::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= ShakeHead::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CNPCPenguinState_StandTitle);
	}
}

//*****************************************************
// 腹ベタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_StomachTitle::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_STOMACH);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_StomachTitle::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_StomachTitle::Update(CNPCPenguin* pPenguin)
{
	m_nCounter++;
	if (m_nCounter >= Stomach::MOTION_COUNT)
	{// ステート時間終了
		pPenguin->SetState(new CNPCPenguinState_StandTitle);
	}
}

//*****************************************************
// 首うねステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_UNYOTitle::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_UNYO);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_UNYOTitle::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_UNYOTitle::Update(CNPCPenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 首うにょん終わった
		pPenguin->SetState(new CNPCPenguinState_StandTitle);
	}
}

//*****************************************************
// 羽パタパタステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_WingPTPTTitle::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_WINGPTPT);

	// ペンギンの羽ばたき音
	CSound::GetInstance()->Play(CSound::LABEL_SE_FLAPPING_WING);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_WingPTPTTitle::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_WingPTPTTitle::Update(CNPCPenguin* pPenguin)
{
	if (pPenguin->IsFinish())
	{// 羽パタ終わった
		pPenguin->SetState(new CNPCPenguinState_StandTitle);
	}
}
