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
#include "resultSingle.h"
#include "player.h"

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
const D3DXVECTOR3 POSR_GAME = { 0.0f,0.0f,-200.0f };	// ゲーム中の注視点位置
const D3DXVECTOR3 POSV_GAME = { 0.0f,1544.0f,-681.0f };	// ゲーム中の視点位置

const D3DXVECTOR3 POSR_DEFAULT_SELECTSTAGE = { 0.0f,0.0f,-400.0f };	// ステージセレクト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_SELECTSTAGE = { 0.0f,2244.0f,-2001.0f };	// ステージセレクト中のデフォルト視点位置

const D3DXVECTOR3 POSR_DEFAULT_SELECTMODE = { 0.0f,50.0f,0.0f };	// モードセレクト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_SELECTMODE = { 0.0f,200.0f,-2000.0f };	// モードセレクト中のデフォルト視点位置

const D3DXVECTOR3 POSR_DEFAULT_RESULTMULTI = { 800.0f,50.0f,-400.0f };	// マルチモードのリザルト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_RESULTMULTI = { 800.0f,400.0f,-2400.0f };	// マルチモードのリザルト中のデフォルト視点位置

namespace resultSingle
{
const D3DXVECTOR3 POS_OFFSET = { 0.0f,100.0f,-500.0f };	// 目標地点のオフセット
const float SPEED_POSR = 0.1f;	// 注視点の速度
}
}

//***********************************************************************************
// プレイヤーの追従
//***********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CFollowPlayer::CFollowPlayer() : m_fTimerPosR(0.0f), m_fLengthPosR(0.0f),m_bDebug(false)
{

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

//**************************************************************************
// ステージセレクト
//**************************************************************************
//=====================================================
// 初期化
//=====================================================
void CCameraStateSelectStage::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// カメラ視点、注視点位置初期化
	pInfoCamera->posR = POSR_DEFAULT_SELECTSTAGE;
	pInfoCamera->posV = POSV_DEFAULT_SELECTSTAGE;
}

//=====================================================
// 更新
//=====================================================
void CCameraStateSelectStage::Update(CCamera* pCamera)
{

}

//**************************************************************************
// シングルリザルト
//**************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CCameraResultSingle::CCameraResultSingle(CResultSingle *pResult)
{
	// リザルトの受け取り
	m_pResult = pResult;
}

//=====================================================
// 初期化
//=====================================================
void CCameraResultSingle::Init(CCamera* pCamera)
{
	// 目的位置設定
	DecidePosDest(pCamera);
}

//=====================================================
// 目的位置設定
//=====================================================
void CCameraResultSingle::DecidePosDest(CCamera* pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// カメラ現在地を初期位置に設定
	m_posInitiial = pInfoCamera->posV;

	//-------------------------------
	// 目標位置の設定
	//-------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();	// プレイヤーの取得
	assert(!apPlayer.empty());

	CPlayer *pPlayer = *apPlayer.begin();
	assert(pPlayer != nullptr);

	// プレイヤーの位置にオフセットを設定して、目標位置にする
	m_posDest = pPlayer->GetPosition() + resultSingle::POS_OFFSET;

	// 初期差分ベクトルの設定
	m_vecDiffInitial = m_posDest - m_posInitiial;
}

//=====================================================
// 更新
//=====================================================
void CCameraResultSingle::Update(CCamera* pCamera)
{
	// プレイヤーの前まで移動する処理
	MoveToPlayerFront(pCamera);
}

//=====================================================
// プレイヤーの前まで移動する処理
//=====================================================
void CCameraResultSingle::MoveToPlayerFront(CCamera* pCamera)
{
	//-------------------------------
	// 視点の移動
	//-------------------------------
	m_fTimerMove += CManager::GetDeltaTime();

	if (m_fTimerMove > CCameraResultSingle::TIME_MOVE)
		m_pResult->SetState(CResultSingle::E_State::STATE_FADE);

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	float fTime = m_fTimerMove / CCameraResultSingle::TIME_MOVE;
	float fRate = easing::EaseOutExpo(fTime);

	pInfoCamera->posV = m_posInitiial + m_vecDiffInitial * fRate;

	//-------------------------------
	// 注視点の移動
	//-------------------------------
	D3DXVECTOR3 posDestR = m_posDest - resultSingle::POS_OFFSET;

	pInfoCamera->posR += (posDestR - pInfoCamera->posR) * resultSingle::SPEED_POSR;
}

//**************************************************************************
// マルチモードのリザルト
//**************************************************************************
//=====================================================
// 初期化
//=====================================================
void CCameraStateResultMulti::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// カメラ視点、注視点位置初期化
	pInfoCamera->posR = POSR_DEFAULT_RESULTMULTI;
	pInfoCamera->posV = POSV_DEFAULT_RESULTMULTI;
}

//=====================================================
// 更新
//=====================================================
void CCameraStateResultMulti::Update(CCamera* pCamera)
{

}

//**************************************************************************
// モードセレクト
//**************************************************************************
//=====================================================
// 初期化
//=====================================================
void CCameraStateSelectMode::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// カメラ視点、注視点位置初期化
	pInfoCamera->posR = POSR_DEFAULT_SELECTMODE;
	pInfoCamera->posV = POSV_DEFAULT_SELECTMODE;
}

//=====================================================
// 更新
//=====================================================
void CCameraStateSelectMode::Update(CCamera* pCamera)
{

}