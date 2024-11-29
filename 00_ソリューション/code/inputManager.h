//*****************************************************
//
// 入力マネージャー[inputManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CInputManager
{
public:
	enum E_Button
	{// ボタンの種類
		BUTTON_ENTER = 0,	// エンターボタン
		BUTTON_READY,	// 準備ボタン
		BUTTON_BACK,	// 戻るボタン
		BUTTON_SKIP,	// スキップボタン
		BUTTON_PAUSE,	// ポーズ
		BUTTON_AXIS_UP,	// 上方向キー スティック左
		BUTTON_AXIS_DOWN,	// 下方向キー
		BUTTON_AXIS_RIGHT,	// 右方向キー
		BUTTON_AXIS_LEFT,	// 左方向キー
		BUTTON_TRIGGER_UP,	// 上方向弾き
		BUTTON_TRIGGER_DOWN,	// 下方向弾き スティック右
		BUTTON_TRIGGER_RIGHT,	// 右方向弾き
		BUTTON_TRIGGER_LEFT,	// 左方向弾き
		BUTTON_PECK,	// つつき
		BUTTON_JUMP,	// ジャンプ
		BUTTON_SCORE,	// スコア
		BUTTON_RESULT,	// リザルト演出
		BUTTON_SETICE,	// 氷設置
		BUTTON_MAX
	};
	struct S_Axis
	{// 方向のまとめ
		D3DXVECTOR3 axisMove;	// 移動方向
		D3DXVECTOR3 axisCamera;	// カメラを動かす方向
	};

	CInputManager();	// コンストラクタ
	~CInputManager();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateDevice(void);
	bool GetTrigger(E_Button button) { return m_info.abTrigger[button]; }
	bool GetPress(E_Button button) { return m_info.abPress[button]; }
	bool GetRelease(E_Button button) { return m_info.abRelease[button]; }
	S_Axis GetAxis(void) { return m_axis; }

	float GetAngleMove(void) { return atan2f(-m_axis.axisMove.x, -m_axis.axisMove.z); }	// 移動入力角度の取得

	// 静的メンバ関数
	static CInputManager *Create(void);
	static CInputManager *GetInstance(int nIdx = 0);
	static void InitDevice(HINSTANCE hInstance, HWND hWnd);	// デバイスの総初期化
	static void UninitDevice(void);	// デバイスの総終了
	static void ReleaseAll(void);	// 全インスタンス解放
	static void UpdateAll(void);	// 全インスタンス更新
	static vector<CInputManager*> GetArray(void) { return s_aInputMgr; }	// 配列の取得

private:
	struct S_Info
	{// 情報の構造体
		bool abTrigger[BUTTON_MAX];	// トリガー情報
		bool abPress[BUTTON_MAX];	// プレス情報
		bool abRelease[BUTTON_MAX];	// リリース情報
	};

	S_Info m_info;		// 情報
	S_Axis m_axis;		// 方向の情報
	int m_nID;			// 番号
	float m_fDeadZone;	// デッドゾーン

	static vector<CInputManager*> s_aInputMgr;	// 格納用配列
};

#endif
