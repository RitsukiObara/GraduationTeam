//*****************************************************
//
// カメラステイト[CameraState.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "CameraState.h"
#include "camera.h"
#include "effect3D.h"
#include "inputmouse.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "manager.h"
#include "title.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVE_SPEED = 21.0f;	//移動スピード
const float ROLL_SPEED = 0.04f;	//回転スピード
const float FACT_CORRECT_CONTOROLLL = 0.9f;	// 操作時の位置補正係数

const float LENGTH_FOLLOW = 412.0f;	// 追従時のカメラ距離
const float ANGLE_FOLLOW = 0.73f;	// 追従時のカメラ角度
const D3DXVECTOR3 POSR_GAME = { 0.0f,0.0f,00.0f };	// ゲーム中の注視点位置
const D3DXVECTOR3 POSV_GAME = { 0.0f,1044.0f,-581.0f };	// ゲーム中の視点位置
}

//***********************************************************************************
// プレイヤーの追従
//***********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CFollowPlayer::CFollowPlayer() : m_fTimerPosR(0.0f), m_fLengthPosR(0.0f),m_bDebug(false)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		CCamera::Camera *pInfoCamera = pCamera->GetCamera();

		pInfoCamera->fLength = LENGTH_FOLLOW;

		pInfoCamera->rot.x = ANGLE_FOLLOW;

		pInfoCamera->posR = POSR_GAME;
		pInfoCamera->posV = POSV_GAME;
	}
}

//=====================================================
// 更新
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posR = POSR_GAME;
	pInfoCamera->posV = POSV_GAME;
}

//=====================================================
// 操作する
//=====================================================
void CMoveControl::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// 入力取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	//マウス操作======================================================
	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
	{//右クリック中、視点旋回
		D3DXVECTOR3 rot;

		//マウスの移動量代入
		rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

		D3DXVec3Normalize(&rot, &rot);

		//視点の旋回
		pInfoCamera->rot.y += rot.x * ROLL_SPEED;
		pInfoCamera->rot.x -= rot.y * ROLL_SPEED;

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{//加速
			fMove *= 7;
		}

		D3DXVECTOR3 rotMove = pInfoCamera->rot;
		D3DXVECTOR3 vecPole = { 0.0f,0.0f,0.0f };

		// 視点移動===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{// 左移動
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{// 右移動
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 前移動
			pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x) * MOVE_SPEED;
			pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{// 後移動
			pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x + D3DX_PI) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x + D3DX_PI) * MOVE_SPEED;
			pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x + D3DX_PI) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{// 上昇
			pInfoCamera->posVDest.y += fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{// 下降
			pInfoCamera->posVDest.y -= fMove;
			pCamera->SetPosR();
		}

		pCamera->SetPosR();
	}

	pCamera->MoveDist(FACT_CORRECT_CONTOROLLL);
}

//**************************************************************************
// タイトル
//**************************************************************************
//=====================================================
// 更新
//=====================================================
void CCameraStateTitle::Update(CCamera* pCamera)
{
	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->rot.y += 0.005f;

	universal::LimitRot(&pInfoCamera->rot.y);

	pCamera->SetPosV();

	CDebugProc::GetInstance()->Print("\n視点  [%f, %f, %f]", pInfoCamera->posV.x, pInfoCamera->posV.y, pInfoCamera->posV.z);
	CDebugProc::GetInstance()->Print("\n注視点[%f, %f, %f]", pInfoCamera->posR.x, pInfoCamera->posR.y, pInfoCamera->posR.z);
	CDebugProc::GetInstance()->Print("\nカメラ距離[%f]", pInfoCamera->fLength);
	CDebugProc::GetInstance()->Print("\n角度  [%f, %f, %f]", pInfoCamera->rot.x, pInfoCamera->rot.y, pInfoCamera->rot.z);
}