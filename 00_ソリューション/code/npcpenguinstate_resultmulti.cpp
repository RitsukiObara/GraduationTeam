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
	namespace Loser
	{
		const float ROTATE_SPEED = 0.005f;
	}
}

//*****************************************************
// 勝者バンザイステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_BANZAI::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_VICTORY);

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
// 敗者逃げステート
//*****************************************************
//=====================================================
// 初期化処理
//=====================================================
HRESULT CNPCPenguinState_Flee::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_FAIL);

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
	float length = D3DXVec3Length(&(c_centorPos - pPenguin->GetPosition()));
	D3DXVECTOR3 rot = pPenguin->GetRotation();

	// 移動向きを変える
	rot.y += Loser::ROTATE_SPEED;
	rot.y = fmodf(rot.y + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI;

	// 位置計算
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, pPenguin->GetPosition().y, 0.0f);
	pos.x = -sinf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.x;
	pos.z = -cosf(rot.y - 0.5f * D3DX_PI) * length + c_centorPos.z;

	// 位置向き設定
	pPenguin->SetPosition(pos);
	pPenguin->SetRotation(rot);
}
