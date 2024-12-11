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
#include "enemy.h"
#include "selectStagePenguin.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVE_SPEED = 21.0f;				// 移動スピード
const float ROLL_SPEED = 0.04f;				// 回転スピード
const float FACT_CORRECT_CONTOROLLL = 0.9f;	// 操作時の位置補正係数

const float LENGTH_FOLLOW = 1500.0f;	// 追従時のカメラ距離
const float ANGLE_FOLLOW = 0.33f;		// 追従時のカメラ角度
const float SPPED_MOVE_FOLLOW = 0.09f;	// 追従時のカメラ速度
const D3DXVECTOR3 POSR_GAME = { 0.0f,0.0f,-300.0f };		// ゲーム中の注視点位置
const D3DXVECTOR3 POSV_GAME = { 0.0f,1544.0f,-681.0f };		// ゲーム中の視点位置
const D3DXVECTOR3 POSR_MULTI = { -80.0f,0.0f,-330.0f };		// マルチ中の注視点位置
const D3DXVECTOR3 POSV_MULTI = { -80.0f,1670.0f,-1170.0f };	// マルチ中の視点位置

const D3DXVECTOR3 POSR_DEFAULT_SELECTSTAGE = { 0.0f,0.0f,-400.0f };		// ステージセレクト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_SELECTSTAGE = { 0.0f,2244.0f,-2001.0f };	// ステージセレクト中のデフォルト視点位置

const D3DXVECTOR3 POSR_DEFAULT_SELECTMODE = { 0.0f,50.0f,0.0f };		// モードセレクト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_SELECTMODE = { 0.0f,200.0f,-2000.0f };	// モードセレクト中のデフォルト視点位置

const D3DXVECTOR3 POSR_DEFAULT_RESULTMULTI = { 800.0f,50.0f,-400.0f };		// マルチモードのリザルト中のデフォルト注視点位置
const D3DXVECTOR3 POSV_DEFAULT_RESULTMULTI = { 800.0f,400.0f,-2400.0f };	// マルチモードのリザルト中のデフォルト視点位置

//-----------------------------------
// プレイヤー追従の定数
//-----------------------------------
namespace followPlayer
{
const float DIST_WIDTH = 200.0f;			// カメラの変更する幅
const float DEFAULT_DIST_CAMERA = 1500.0f;	// デフォルトの距離
const float NEAR_LINE = 500.0f;				// 近いと判断する距離
const float TIME_MOVE = 1.5f;				// 移動にかかる時間
const float SPEED_CLOSE = 0.05f;			// 近づく速度
}

//-----------------------------------
// シングルリザルトの定数
//-----------------------------------
namespace resultSingle
{
const D3DXVECTOR3 POS_OFFSET = { 0.0f,100.0f,-500.0f };	// 目標地点のオフセット
const float SPEED_POSR = 0.1f;	// 注視点の速度
}

//-----------------------------------
// ステージ選択の定数
//-----------------------------------
namespace selectStage
{
const D3DXVECTOR3 OFFSET = { 0.0f,2250.0f,-2000.0f };	// オフセット
const float FACT_MOVE = 0.6f;							// 移動係数
}
}

//***********************************************************************************
// プレイヤーの追従
//***********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CFollowPlayer::CFollowPlayer() : m_fTimerMove(0.0f)
{

}

//=====================================================
// 初期化
//=====================================================
void CFollowPlayer::Init(CCamera *pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->fLength = LENGTH_FOLLOW;
	pInfoCamera->posR = POSR_GAME;

	pInfoCamera->rot.x = ANGLE_FOLLOW;
	pInfoCamera->rot.y = D3DX_PI;

	pCamera->SetPosV();
}

//=====================================================
// 更新
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// 距離の管理
	ManageDist();

	// 角度の設定
	pInfoCamera->rot.x = ANGLE_FOLLOW;
	pInfoCamera->rot.y = D3DX_PI;

	// 注視点位置の設定
	pInfoCamera->posR = POSR_GAME;

	// 視点の設定
	pCamera->SetPosV();
}

//=====================================================
// 距離の管理
//=====================================================
void CFollowPlayer::ManageDist(void)
{
	CCamera *pCamera = CManager::GetCamera();
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	if (pInfoCamera == nullptr)
		return;

	// 敵が近かったら距離の割合に応じてカメラを動かす
	float fRate;
	if (IsNearEnemy(&fRate))
		m_fTimerMove += CManager::GetDeltaTime() * followPlayer::SPEED_CLOSE;
	else
		m_fTimerMove -= CManager::GetDeltaTime();

	// 値の制限
	universal::LimitValue(&m_fTimerMove, followPlayer::TIME_MOVE, 0.0f);
	
	// タイマーをイージング
	float fTime = m_fTimerMove / followPlayer::TIME_MOVE;
	fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	pInfoCamera->fLength = followPlayer::DEFAULT_DIST_CAMERA - followPlayer::DIST_WIDTH * fRate;
}

//=====================================================
// 敵が近い判定
//=====================================================
bool CFollowPlayer::IsNearEnemy(float *pRate)
{
	if (CPlayer::GetInstance().empty())
		return false;

	CPlayer *pPlayer = *CPlayer::GetInstance().begin();
	vector<CEnemy*> apEnemy = CEnemy::GetInstance();

	if (pPlayer == nullptr || apEnemy.empty())
		return false;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 全敵との距離チェック
	float fDistMin = followPlayer::NEAR_LINE;
	CEnemy *pEnemyNearest = nullptr;

	for (CEnemy *pEnemy : apEnemy)
	{
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posPlayer, pEnemy->GetPosition(), fDistMin, &fDiff))
		{
			fDistMin = fDiff;

			pEnemyNearest = pEnemy;

			if (pRate != nullptr)
				*pRate = 1.0f - fDiff / followPlayer::NEAR_LINE;
		}
	}

	return pEnemyNearest != nullptr;
}

//***********************************************************************************
// マルチのカメラ
//***********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CMultiGame::CMultiGame()
{

}

//=====================================================
// 更新
//=====================================================
void CMultiGame::Update(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posR = POSR_MULTI;
	pInfoCamera->posV = POSV_MULTI;
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
	// カメラの追従
	D3DXVECTOR3 posPenguin = m_pPenguin->GetPosition();

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posRDest = posPenguin;
	pInfoCamera->posRDest.y = 0.0f;
	pInfoCamera->posVDest = posPenguin + selectStage::OFFSET;

	pCamera->MoveDist(selectStage::FACT_MOVE);

	pInfoCamera->posV.y = selectStage::OFFSET.y;
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

	//-------------------------------
	// 目標位置の設定
	//-------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();	// プレイヤーの取得
	assert(!apPlayer.empty());

	CPlayer* pPlayer = *apPlayer.begin();
	assert(pPlayer != nullptr);
	D3DXVECTOR3 rot = pPlayer->GetRotation();

	universal::FactingRot(&rot.y, 0.0f, 0.02f);

	pPlayer->SetRotation(rot);
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