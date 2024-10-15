//*****************************************************
//
// キーボード入力処理[inputkeyboard.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "input.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_KEY_MAX			(256)			// キーの最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	int GetRepeat(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];					// プレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			// トリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			// リリース情報
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];			// リピート情報
	int m_aCntRepeat[NUM_KEY_MAX];	// リピートカウンター
};

#endif