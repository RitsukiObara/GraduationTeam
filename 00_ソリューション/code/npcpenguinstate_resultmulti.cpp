//*****************************************************
//
// マルチモードのリザルトステート[npcpenguinstate_resultmulti.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguinstate_resultmulti.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
	const float SCALE_BODY = 1.8f;	// 体のスケール
	
	const float FACT_DECREASE_MOVE = 0.9f;	// 移動量の減衰係数
	const float ADD_MOVE = 4.0f;	// 移動の追加量
	
	const float RADIUS_COLLISION = 200.0f;	// 球の判定の半径
}

//*****************************************************
// 勝者バンザイステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_BANZAI::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_BANZAI::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_BANZAI::Update(CNPCPenguin* pPenguin)
{
	
}

//*****************************************************
// 勝者ジャンプステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Jump::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Jump::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Jump::Update(CNPCPenguin* pPenguin)
{
	
}

//*****************************************************
// 敗者逃げステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Flee::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_FALL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNPCPenguinState_Flee::Uninit(void)
{
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CNPCPenguinState_Flee::Update(CNPCPenguin* pPenguin)
{
	
}
