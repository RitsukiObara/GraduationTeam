//*****************************************************
//
// 入力マネージャー[inputManager.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "inputManager.h"
#include "manager.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CInputManager *CInputManager::m_pInputManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CInputManager::CInputManager()
{
	ZeroMemory(&m_info, sizeof(SInfo));
	ZeroMemory(&m_axis, sizeof(SAxis));
	m_fAccele = 0.0f;
	m_fBreake = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CInputManager::~CInputManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CInputManager *CInputManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputManager == nullptr)
	{// インスタンス生成
		m_pInputManager = new CInputManager;

		// 初期化処理
		m_pInputManager->Init(hInstance, hWnd);
	}

	return m_pInputManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CInputManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ジョイパッド生成
	CInputJoypad::Create();

	// キーボード生成
	CInputKeyboard::Create(hInstance, hWnd);

	// マウス生成
	CInputMouse::Create(hInstance, hWnd);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CInputManager::Uninit(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->Uninit();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->Uninit();
	}

	if (pMouse != nullptr)
	{
		pMouse->Uninit();
	}

	// 自身のポインタ破棄
	m_pInputManager = nullptr;
	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CInputManager::Update(void)
{
	// 各入力デバイスの更新
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->Update();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->Update();
	}

	if (pMouse != nullptr)
	{
		pMouse->Update();
	}

	// エンター
	m_info.abTrigger[BUTTON_ENTER] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// 戻るボタン
	m_info.abTrigger[BUTTON_BACK] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_BACK, 0) ||
		pMouse->GetTrigger(CInputMouse::BUTTON_RMB)
	);

	// ポーズ
	m_info.abTrigger[BUTTON_PAUSE] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) ||
		pKeyboard->GetTrigger(DIK_P)
	);

	// 上方向キー
	m_info.abTrigger[BUTTON_AXIS_UP] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_UP,0) || 
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP,0) ||
		pKeyboard->GetTrigger(DIK_W)
	);

	// 下方向キー
	m_info.abTrigger[BUTTON_AXIS_DOWN] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_DOWN, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, 0) ||
		pKeyboard->GetTrigger(DIK_S)
	);

	// 左方向キー
	m_info.abTrigger[BUTTON_AXIS_LEFT] =
		(
			pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_LEFT, 0) ||
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, 0) ||
			pKeyboard->GetTrigger(DIK_A)
		);

	// 右方向キー
	m_info.abTrigger[BUTTON_AXIS_RIGHT] =
		(
			pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_RIGHT, 0) ||
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, 0) ||
			pKeyboard->GetTrigger(DIK_D)
		);

	// 上方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_UP] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_UP, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP, 0)
	);

	// 下方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_DOWN] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_DOWN, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, 0)
	);

	// 右方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_RIGHT] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_RIGHT, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, 0)
	);

	// 左方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_LEFT] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_LEFT, 0) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, 0)
	);

	// つつき
	m_info.abTrigger[BUTTON_PECK] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, 0) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// スコア加算
	m_info.abTrigger[BUTTON_SCORE] =
	(
		pKeyboard->GetPress(DIK_L)
	);

	// リザルト演出
	m_info.abTrigger[BUTTON_RESULT] =
	(
		pKeyboard->GetTrigger(DIK_R)
	);

	// 方向のリセット
	ZeroMemory(&m_axis, sizeof(SAxis));

	// 移動方向の設定=============
	// ジョイパッド
	m_axis.axisMove = D3DXVECTOR3(pJoypad->GetJoyStickLX(0), 0.0f, pJoypad->GetJoyStickLY(0));

	// キーボード
	if (pKeyboard->GetPress(DIK_W))
	{
		m_axis.axisMove.z += 1.0f;
	}
	if (pKeyboard->GetPress(DIK_S))
	{
		m_axis.axisMove.z -= 1.0f;
	}
	if (pKeyboard->GetPress(DIK_A))
	{
		m_axis.axisMove.x -= 1.0f;
	}
	if (pKeyboard->GetPress(DIK_D))
	{
		m_axis.axisMove.x += 1.0f;
	}

	if (D3DXVec3Length(&m_axis.axisMove) > 1.0f)
		D3DXVec3Normalize(&m_axis.axisMove, &m_axis.axisMove);

	// カメラの向く方向設定=============
	m_axis.axisCamera += D3DXVECTOR3(pJoypad->GetJoyStickRX(0), -pJoypad->GetJoyStickRY(0), 0.0f);
	m_axis.axisCamera += D3DXVECTOR3(pMouse->GetMoveIX(), pMouse->GetMoveIY(), 0.0f);

	D3DXVec3Normalize(&m_axis.axisCamera, &m_axis.axisCamera);

	// アクセルの操作
	m_fAccele = pJoypad->GetTriggerR(0);

	if (pKeyboard->GetPress(DIK_W))
	{
		m_fAccele = 1.0f;
	}

	// ブレーキの操作
	m_fBreake = pJoypad->GetTriggerL(0);

	if (pKeyboard->GetPress(DIK_S))
	{
		m_fBreake = 1.0f;
	}
}

void CInputManager::UpdateDevice(void)
{
	// 各入力デバイスの更新
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->UpdateDevice();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->UpdateDevice();
	}

	if (pMouse != nullptr)
	{
		pMouse->UpdateDevice();
	}
}