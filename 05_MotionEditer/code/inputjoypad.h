//*****************************************************
//
// ジョイパッド入力処理[inputjoypad.h]
// Author:髙山桃也
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
	//ゲームパッドボタン
	typedef enum
	{
		PADBUTTONS_UP = 0,				//ゲームパッドのボタン
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

	//バイブレーションの状態
	typedef enum
	{
		PADVIB_NONE = 0,						//使用していない状態
		PADVIB_USE,								//使用している状態
		PADVIB_MAX
	}PADVIB;

	CInputJoypad();	// コンストラクタ
	~CInputJoypad();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool GetPress(PADBUTTOS nKey, int nPlayer);
	bool GetTrigger(PADBUTTOS nKey, int nPlayer);
	bool GetRelease(PADBUTTOS nKey, int nPlayer);
	bool GetRepeat(PADBUTTOS nKey, int nPlayer);
	float GetJoyStickLX(int nPlayer);
	float GetJoyStickLY(int nPlayer);
	float GetJoyStickRX(int nPlayer);
	float GetJoyStickRY(int nPlayer);
	D3DXVECTOR3 GetVecStickL(void);
	void Vibration(int nPlayer, PADVIB state, short sVib, int nTime);

private:
	XINPUT_STATE m_aState[MAX_PLAYER];				//ゲームパッドのプレス情報
	XINPUT_STATE m_aStateTrigger[MAX_PLAYER];		//ゲームパッドのトリガー情報
	XINPUT_STATE m_aStateRelease[MAX_PLAYER];		//ゲームパッドのリリース情報
	XINPUT_STATE m_aStateRepeat[MAX_PLAYER];			//ゲームパッドのリピート情報
	XINPUT_VIBRATION m_aVibration[MAX_PLAYER];		//ゲームパッドの振動情報
	PADVIB m_aVibState[MAX_PLAYER];					//振動の状態
	int m_nVibTimer;		// バイブレーションのタイマー
	int m_aCntRepeat[MAX_PLAYER][PADBUTTONS_MAX];	// リピートカウンター
};

#endif
