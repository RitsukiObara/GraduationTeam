//*****************************************************
//
// カメラの処理[camera.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "main.h"
#include "camera.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_SPEED					(4.0f)						//移動スピード
#define ROLL_SPEED					(0.03f)						//回転スピード
#define MOVE_FACT					(0.3f)						//移動減衰係数

//====================================================
//初期化処理
//====================================================
HRESULT CCamera::Init(void)
{
	m_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -500.0f);
	m_camera.posVOld = D3DXVECTOR3(0.0f, 30.0f, -450.0f);
	m_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera.fLength = 0.0f;

	float fLength = sqrtf
	(//Y面での対角線
		(m_camera.posV.x - m_camera.posR.x) * (m_camera.posV.x - m_camera.posR.x) + 
		(m_camera.posV.z - m_camera.posR.z) * (m_camera.posV.z - m_camera.posR.z)
	);

	m_camera.fLength = sqrtf
	(//目標までの直線距離
		fLength * fLength + (m_camera.posV.y - m_camera.posR.y) * (m_camera.posV.y - m_camera.posR.y)
	);

	m_camera.rot.x = atan2f(fLength, m_camera.posV.y - m_camera.posR.y);

	SetPosV();

	return S_OK;
}

//====================================================
//終了処理
//====================================================
void CCamera::Uninit(void)
{
}

//====================================================
//更新処理
//====================================================
void CCamera::Update(void)
{
	Control();
}

//====================================================
//操作処理
//====================================================
void CCamera::Control(void)
{
	// 入力情報入手
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();


	//マウス操作======================================================
	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
	{//右クリック中、視点旋回
		D3DXVECTOR3 rot;

		//マウスの移動量代入
		rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

		D3DXVec3Normalize(&rot, &rot);

		//視点の旋回
		m_camera.rot.y += rot.x * ROLL_SPEED;
		m_camera.rot.x -= rot.y * ROLL_SPEED;

		//注視点を相対位置に設定
		SetPosR();

		//視点移動===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{//左移動
			m_camera.posV.x -= sinf(m_camera.rot.y - D3DX_PI * 0.5f) * MOVE_SPEED;
			m_camera.posV.z -= cosf(m_camera.rot.y - D3DX_PI * 0.5f) * MOVE_SPEED;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{//右移動
			m_camera.posV.x -= sinf(m_camera.rot.y - D3DX_PI * -0.5f) * MOVE_SPEED;
			m_camera.posV.z -= cosf(m_camera.rot.y - D3DX_PI * -0.5f) * MOVE_SPEED;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{//前移動
			m_camera.posV.x += sinf(m_camera.rot.x + D3DX_PI) * sinf(m_camera.rot.y) * MOVE_SPEED;
			m_camera.posV.y += cosf(m_camera.rot.x + D3DX_PI) * MOVE_SPEED;
			m_camera.posV.z += sinf(m_camera.rot.x + D3DX_PI) * cosf(m_camera.rot.y) * MOVE_SPEED;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{//後移動
			m_camera.posV.x += sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * MOVE_SPEED;
			m_camera.posV.y += cosf(m_camera.rot.x) * MOVE_SPEED;
			m_camera.posV.z += sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * MOVE_SPEED;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{//上昇
			m_camera.posV.y += MOVE_SPEED;
			SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{//下降
			m_camera.posV.y -= MOVE_SPEED;
			SetPosR();
		}
	}
}

//====================================================
//視点設定
//====================================================
void CCamera::SetPosV(void)
{
	m_camera.posV =
	{
		m_camera.posR.x + sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * m_camera.fLength,
		m_camera.posR.y + cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posR.z + sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * m_camera.fLength
	};
}

//====================================================
//注視点設定
//====================================================
void CCamera::SetPosR(void)
{
	m_camera.posR =
	{
		m_camera.posV.x - sinf(m_camera.rot.x) * sinf(m_camera.rot.y) * m_camera.fLength,
		m_camera.posV.y - cosf(m_camera.rot.x) * m_camera.fLength,
		m_camera.posV.z - sinf(m_camera.rot.x) * cosf(m_camera.rot.y) * m_camera.fLength
	};
}

//====================================================
//設定処理
//====================================================
void CCamera::SetCamera(void)
{
	// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックス==================================================
	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MIN_DRAW,
		MAX_DRAW);

	//マトリックス設定
	pDevice->SetTransform(D3DTS_PROJECTION,&m_camera.mtxProjection);

	//ビューマトリックス============================================================
	//ビューマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxView);

	//ビューマトリックス作成
	D3DXMatrixLookAtLH(&m_camera.mtxView,
		&m_camera.posV,
		&m_camera.posR,
		&m_camera.vecU);

	//ビューマトリックス設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//====================================================
//情報取得処理
//====================================================
CCamera::Camera *CCamera::GetCamera(void)
{
	return &m_camera;
}