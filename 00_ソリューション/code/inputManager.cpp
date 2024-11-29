//*****************************************************
//
// 入力マネージャー[inputManager.cpp]
// Author:髙山桃也
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
// 定数定義
//*****************************************************
namespace
{
const float DEADZONE_DEFAULT = 0.2f;	// スティックのデッドゾーン
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CInputManager*> CInputManager::s_aInputMgr;	// 格納用の配列

//=====================================================
// コンストラクタ
//=====================================================
CInputManager::CInputManager()
{
	ZeroMemory(&m_info, sizeof(S_Info));
	ZeroMemory(&m_axis, sizeof(S_Axis));
	m_nID = -1;
	m_fDeadZone = 0.0f;
}

//=====================================================
// デストラクタ
//=====================================================
CInputManager::~CInputManager()
{

}

//=====================================================
// デバイスの総初期化
//=====================================================
void CInputManager::InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	// ジョイパッド生成
	CInputJoypad::Create();

	// キーボード生成
	CInputKeyboard::Create(hInstance, hWnd);

	// マウス生成
	CInputMouse::Create(hInstance, hWnd);
}

//=====================================================
// 生成処理
//=====================================================
CInputManager *CInputManager::Create(void)
{
	CInputManager *pInputMgr = new CInputManager;

	if (pInputMgr != nullptr)
	{
		pInputMgr->Init();
	}

	return pInputMgr;
}

//=====================================================
// インスタンスの取得
//=====================================================
CInputManager *CInputManager::GetInstance(int nIdx)
{
	if (nIdx < 0 || nIdx > (int)s_aInputMgr.size() - 1)
		return nullptr;

	return s_aInputMgr[nIdx];
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CInputManager::Init(void)
{
	// 番号の保存
	m_nID = s_aInputMgr.size();

	// 配列に格納
	s_aInputMgr.push_back(this);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CInputManager::Uninit(void)
{
	// 自身のポインタ破棄
	delete this;
}

//=====================================================
// デバイスの総初期化
//=====================================================
void CInputManager::UninitDevice(void)
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
}

//=====================================================
// 全インスタンスの解放
//=====================================================
void CInputManager::ReleaseAll(void)
{
	for (auto it : s_aInputMgr)
	{
		if (it != nullptr)
			it->Uninit();
	}

	s_aInputMgr.clear();
}

//=====================================================
// 全インスタンスの更新
//=====================================================
void CInputManager::UpdateAll(void)
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

	for (auto it : s_aInputMgr)
	{
		if (it != nullptr)
			it->Update();
	}
}

//=====================================================
// 更新処理
//=====================================================
void CInputManager::Update(void)
{
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	// エンター
	m_info.abTrigger[BUTTON_ENTER] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, m_nID) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// 準備ボタン
	m_info.abTrigger[BUTTON_READY] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, m_nID) ||
		pKeyboard->GetTrigger(DIK_O)
	);

	// 戻るボタン
	m_info.abTrigger[BUTTON_BACK] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_BACK, m_nID) ||
		pKeyboard->GetTrigger(DIK_BACK)
	);

	// スキップボタン
	m_info.abPress[BUTTON_SKIP] =
	(
		pJoypad->GetPress(CInputJoypad::PADBUTTONS_BACK, m_nID) ||
		pKeyboard->GetPress(DIK_TAB)
	);

	// ポーズ
	m_info.abTrigger[BUTTON_PAUSE] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, m_nID) ||
		pKeyboard->GetTrigger(DIK_P)
	);

	// 上方向キー
	m_info.abTrigger[BUTTON_AXIS_UP] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_UP, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP, m_nID) ||
		pKeyboard->GetTrigger(DIK_W)
	);

	// 下方向キー
	m_info.abTrigger[BUTTON_AXIS_DOWN] =
	(
		pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_DOWN, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, m_nID) ||
		pKeyboard->GetTrigger(DIK_S)
	);

	// 左方向キー
	m_info.abTrigger[BUTTON_AXIS_LEFT] =
		(
			pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_LEFT, m_nID) ||
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, m_nID) ||
			pKeyboard->GetTrigger(DIK_A)
		);

	// 右方向キー
	m_info.abTrigger[BUTTON_AXIS_RIGHT] =
		(
			pJoypad->GetLStickTrigger(CInputJoypad::DIRECTION_RIGHT, m_nID) ||
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, m_nID) ||
			pKeyboard->GetTrigger(DIK_D)
		);

	// 上方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_UP] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_UP, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP, m_nID)
	);

	// 下方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_DOWN] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_DOWN, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, m_nID)
	);

	// 右方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_RIGHT] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_RIGHT, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, m_nID)
	);

	// 左方向弾き
	m_info.abTrigger[BUTTON_TRIGGER_LEFT] =
	(
		pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION_LEFT, m_nID) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, m_nID)
	);

	// つつき
	m_info.abTrigger[BUTTON_PECK] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_B, m_nID) ||
		pKeyboard->GetTrigger(DIK_RETURN)
	);

	// ジャンプ
	m_info.abTrigger[BUTTON_JUMP] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, m_nID) ||
		pKeyboard->GetTrigger(DIK_SPACE)
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

	// 氷設置
	m_info.abTrigger[BUTTON_SETICE] =
	(
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, m_nID) ||
		pKeyboard->GetTrigger(DIK_RSHIFT)
	);

	// 方向のリセット
	ZeroMemory(&m_axis, sizeof(S_Axis));

	// 移動方向の設定=============
	// ジョイパッド
	D3DXVECTOR3 vecInput = D3DXVECTOR3(pJoypad->GetJoyStickLX(m_nID), 0.0f, pJoypad->GetJoyStickLY(m_nID));
	float fLengthInput = sqrtf(vecInput.x * vecInput.x + vecInput.z * vecInput.z);

	if (fLengthInput < DEADZONE_DEFAULT)
		vecInput = { 0.0f,0.0f,0.0f };

	m_axis.axisMove = vecInput;

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
	m_axis.axisCamera += D3DXVECTOR3(pJoypad->GetJoyStickRX(m_nID), -pJoypad->GetJoyStickRY(m_nID), 0.0f);
	m_axis.axisCamera += D3DXVECTOR3(pMouse->GetMoveIX(), pMouse->GetMoveIY(), 0.0f);

	D3DXVec3Normalize(&m_axis.axisCamera, &m_axis.axisCamera);
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