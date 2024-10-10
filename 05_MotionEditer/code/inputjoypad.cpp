//*****************************************************
//
// ジョイパッド入力処理[inputjoypad.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "inputjoypad.h"

//====================================================
// コンストラクタ
//====================================================
CInputJoypad::CInputJoypad()
{
	ZeroMemory(&m_aState, sizeof(m_aState));
	ZeroMemory(&m_aStateRelease, sizeof(m_aStateRelease));
	ZeroMemory(&m_aStateTrigger, sizeof(m_aStateTrigger));
	ZeroMemory(&m_aStateRepeat, sizeof(m_aStateRepeat));
	ZeroMemory(&m_aVibration, sizeof(m_aVibration));
	ZeroMemory(&m_aVibState,sizeof(m_aVibState));

	m_nVibTimer = 0;
}

//====================================================
// デストラクタ
//====================================================
CInputJoypad::~CInputJoypad()
{

}

//====================================================
// 初期化処理
//====================================================
HRESULT CInputJoypad::Init(void)
{
	//xinputを有効化する
	XInputEnable(true);

	//メモリのクリア
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		memset(&m_aState[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibration[nCount], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aVibState[nCount], 0, sizeof(PADVIB));
	}

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CInputJoypad::Uninit(void)
{
	//xinputを無効化する
	XInputEnable(false);
}

//====================================================
// 更新処理
//====================================================
void CInputJoypad::Update(void)
{
	//変数宣言
	XINPUT_STATE aState[MAX_PLAYER];
	int nCntKey;

	for (nCntKey = 0; nCntKey < MAX_PLAYER; nCntKey++)
	{
		if (m_nVibTimer > 0)
		{
			m_nVibTimer--;
		}
		else if (m_nVibTimer <= 0)
		{
			memset(&m_aVibState[nCntKey], 0, sizeof(PADVIB));
			m_aVibration[nCntKey].wLeftMotorSpeed = 0;
			m_aVibration[nCntKey].wRightMotorSpeed = 0;
			//振動状態を伝達
			XInputSetState(nCntKey, &m_aVibration[nCntKey]);
			m_nVibTimer = 0;
		}

		if (m_aVibration[nCntKey].wLeftMotorSpeed < 0 ||
			m_aVibration[nCntKey].wRightMotorSpeed < 0)
		{
			m_aVibration[nCntKey].wLeftMotorSpeed = 0;
			m_aVibration[nCntKey].wRightMotorSpeed = 0;
			//振動状態を伝達
			XInputSetState(nCntKey, &m_aVibration[nCntKey]);
		}
		//入力デバイスからデータを取得
		if (XInputGetState(nCntKey, &aState[nCntKey]) == ERROR_SUCCESS)
		{
			for (nCntKey = 0; nCntKey < MAX_PLAYER; nCntKey++)
			{
				//トリガー
				m_aStateTrigger[nCntKey].Gamepad.wButtons =
					(m_aState[nCntKey].Gamepad.wButtons ^ aState[nCntKey].Gamepad.wButtons)
					& aState[nCntKey].Gamepad.wButtons;

				//リリース
				m_aStateRelease[nCntKey].Gamepad.wButtons =
					(m_aState[nCntKey].Gamepad.wButtons ^ aState[nCntKey].Gamepad.wButtons)
					& m_aState[nCntKey].Gamepad.wButtons;

				//リピート
				m_aStateRepeat[nCntKey].Gamepad.wButtons =
					(m_aState[nCntKey].Gamepad.wButtons & aState[nCntKey].Gamepad.wButtons);

				//プレス
				m_aState[nCntKey] = aState[nCntKey];
			}
		}
	}
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickLX(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbLX / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickLY(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbLY / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickRX(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbRX / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック情報取得
//====================================================
float CInputJoypad::GetJoyStickRY(int nPlayer)
{
	//情報を返す
	return (float)m_aState[nPlayer].Gamepad.sThumbRY / USHRT_MAX * 2;
}

//====================================================
// ジョイスティック移動量取得（L）
//====================================================
D3DXVECTOR3 CInputJoypad::GetVecStickL(void)
{
	float fAngle = atan2f(GetJoyStickLX(0), GetJoyStickLY(0));
	float fLength = sqrtf(GetJoyStickLX(0) * GetJoyStickLX(0) + GetJoyStickLY(0) * GetJoyStickLY(0));

	D3DXVECTOR3 vec =
	{
		sinf(fAngle) * fLength,
		cosf(fAngle) * fLength,
		0.0f
	};

	return vec;
}

//====================================================
// プレス情報取得
//====================================================
bool CInputJoypad::GetPress(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//トリガー情報取得
//====================================================
bool CInputJoypad::GetTrigger(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//リリース情報取得
//====================================================
bool CInputJoypad::GetRelease(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
//リピート情報取得
//====================================================
bool CInputJoypad::GetRepeat(PADBUTTOS nKey, int nPlayer)
{
	//三項演算子
	return(m_aStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================
// バイブ情報設定
//====================================================
void CInputJoypad::Vibration(int nPlayer, PADVIB state, short sVib,int nTime)
{
	switch (state)
	{
	case PADVIB_USE:
		m_aVibration[nPlayer].wLeftMotorSpeed = sVib;
		m_aVibration[nPlayer].wRightMotorSpeed = sVib;
		m_aVibState[nPlayer] = state;
		break;
	default:
		break;
	}

	m_nVibTimer = nTime;

	//振動状態を伝達
	XInputSetState(nPlayer, &m_aVibration[nPlayer]);
}
