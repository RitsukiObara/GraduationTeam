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
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CRenderer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CFade;
class CMyEffekseer;
class CCameraState;

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
	void Update(void);
	void Draw();

	static CManager *Create(void);
	static CManager *GetInstance(void) { return m_pManager; }

	static CCamera *GetCamera(void) { return m_pCamera; }
	static CMyEffekseer* GetMyEffekseer(void) { return m_pMyEffekseer; }
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_mode; }
	static float GetDeltaTime(void) { return m_fDeltaTime; }
	static void SetTick(float fDeltaTime) { m_fDeltaTime = fDeltaTime; }

private:
	// メンバ関数
	void ToggleDebugCamera(void);	// デバッグカメラの切り替え

	// メンバ変数
	bool m_bDebugCamera;	// デバッグカメラフラグ
	CCameraState *m_pCameraStateLast;	// 前回のカメラのステイト

	// 静的メンバ変数
	static CCamera *m_pCamera;	// カメラのポインタ
	static CMyEffekseer *m_pMyEffekseer;  // エフェクシアのポインタ
	static CScene *m_pScene;	// 現在のシーン
	static CScene::MODE m_mode;	// 現在のモード
	static float m_fDeltaTime;	// 前回のフレームから経過した秒数
	static CManager *m_pManager;	// 自身のポインタ
};

#endif