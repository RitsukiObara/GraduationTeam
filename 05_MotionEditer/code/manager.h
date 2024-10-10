//*****************************************************
//
// マネージャー処理[manager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CDebugProc;
class CCamera;
class CLight;
class CTexture;

//*****************************************************
// クラスの定義
//*****************************************************
class CManager
{
public:
	CManager();	// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
	void Uninit(void);
	void Update();
	void Draw();
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetKeyboard(void) { return m_pKeyboard; }
	static CInputMouse *GetMouse(void) { return m_pMouse; }
	static CInputJoypad *GetJoypad(void) { return m_pJoypad; }
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; }
	static CCamera *GetCamera(void) { return m_pCamera; }
	static CLight *GetLight(void) { return m_pLight; }
	static CTexture *GetTexture(void) { return m_pTexture; }

private:
	static CRenderer *m_pRenderer;	// レンダラーのクラス
	static CInputKeyboard *m_pKeyboard;	// キーボードのクラス
	static CInputMouse *m_pMouse;	// マウスのクラス
	static CInputJoypad *m_pJoypad;	// マウスのクラス
	static CDebugProc *m_pDebugProc;	// デバッグプロシージャのクラス
	static CCamera *m_pCamera;	// カメラのポインタ
	static CLight *m_pLight;	// ライトのポインタ
	static CTexture *m_pTexture;	// テクスチャ管理へのポインタ
};

namespace Manager
{
D3DXVECTOR3 GetPosVCamera(void);
}

#endif
