//*****************************************************
//
// ジョイパッド入力処理[inputjoypad.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "input.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_PLAYER	(4)	// プレイヤーの最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CInputJoypad
{
public:
	typedef enum
	{//ゲームパッドボタン
		PADBUTTONS_UP = 0,
		PADBUTTONS_DOWN,
		PADBUTTONS_LEFT,
		PADBUTTONS_RIGHT,
		PADBUTTONS_START,
		PADBUTTONS_BACK,
		PADBUTTONS_LSTICK,
		PADBUTTONS_RSTICK,
		PADBUTTONS_LB,
		PADBUTTONS_RB,
		PADBUTTONS_UNKNOWN,
		PADBUTTONS_UNKNOWN2,
		PADBUTTONS_A,
		PADBUTTONS_B,
		PADBUTTONS_X,
		PADBUTTONS_Y,
		PADBUTTONS_MAX
	}PADBUTTOS;
	enum TRIGGER
	{// トリガー判定
		TRIGGER_RT = 0,
		TRIGGER_LT,
		TRIGGER_MAX
	};
	typedef enum
	{//バイブレーションの状態
		PADVIB_NONE = 0,						//使用していない状態
		PADVIB_USE,								//使用している状態
		PADVIB_MAX
	}PADVIB;
	enum DIRECTION
	{// スティックのトリガー
		DIRECTION_UP = 0,
		DIRECTION_DOWN,
		DIRECTION_RIGHT,
		DIRECTION_LEFT,
		DIRECTION_MAX
	};

	CInputJoypad();	// コンストラクタ
	~CInputJoypad();	// デストラクタ

	static CInputJoypad *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	bool GetPress(PADBUTTOS nKey, int nPlayer);
	bool GetTrigger(PADBUTTOS nKey, int nPlayer);
	bool GetRelease(PADBUTTOS nKey, int nPlayer);
	int GetRepeat(PADBUTTOS nKey, int nPlayer);
	bool GetTriggerTB(TRIGGER trigger, int nPlayer);
	bool GetPressTB(TRIGGER trigger,int nPlayer);
	float GetJoyStickLX(int nPlayer);
	float GetJoyStickLY(int nPlayer);
	float GetJoyStickRX(int nPlayer);
	float GetJoyStickRY(int nPlayer);
	bool GetLStickTrigger(DIRECTION direction, int nPlayer);
	bool GetRStickTrigger(DIRECTION direction,int nPlayer);
	float GetTriggerL(int nPlayer);
	float GetTriggerR(int nPlayer);
	D3DXVECTOR3 GetVecStickL(void);
	bool GetVibration(void) { return m_bVibration; }
	void SetVibration(bool frag) { m_bVibration = frag; }
	void Vibration(int nPlayer, float fVib, int nTime);
	void Vibration(float fVib, int nTime);

	// 静的メンバ関数
	static CInputJoypad *GetInstance(void) { return m_pJoyPad; }

private:
	void CheckTrigger(XINPUT_STATE state, int nPlayer);
	void CheckStickTrigger(XINPUT_STATE state,int nPlayer);

	XINPUT_STATE m_aState[MAX_PLAYER];				//ゲームパッドのプレス情報
	XINPUT_STATE m_aStateTrigger[MAX_PLAYER];		//ゲームパッドのトリガー情報
	XINPUT_STATE m_aStateRelease[MAX_PLAYER];		//ゲームパッドのリリース情報
	XINPUT_STATE m_aStateRepeat[MAX_PLAYER];			//ゲームパッドのリピート情報
	XINPUT_VIBRATION m_aVibration[MAX_PLAYER];		//ゲームパッドの振動情報
	PADVIB m_aVibState[MAX_PLAYER];					//振動の状態
	int m_aVibTimer[MAX_PLAYER];		// バイブレーションのタイマー
	int m_aCntRepeat[MAX_PLAYER][PADBUTTONS_MAX];	// リピートカウンター
	bool m_abTrigggerLStick[MAX_PLAYER][DIRECTION_MAX];	// スティックのトリガー判定
	bool m_abTrigggerRStick[MAX_PLAYER][DIRECTION_MAX];	// スティックのトリガー判定
	bool m_abTriggerTB[MAX_PLAYER][TRIGGER_MAX];	// トリガーの判定
	bool m_abPressTB[MAX_PLAYER][TRIGGER_MAX];	// プレスの判定
	BYTE m_abyTriggerL[MAX_PLAYER];	// 左トリガー
	BYTE m_abyTriggerR[MAX_PLAYER];	// 右トリガー
	bool m_bVibration;				// バイブ有無

	static CInputJoypad *m_pJoyPad;	// 自身のポインタ
};

#endif
