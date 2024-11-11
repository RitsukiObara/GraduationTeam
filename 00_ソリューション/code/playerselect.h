//*****************************************************
//
// ゲーム処理[playerselect.h]
// Author:堀川萩大
//
//*****************************************************
#ifndef _PLAYERSELECT_H_
#define _PLAYERSELECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_PLAYER	(4)

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayerSelect;
class CLight;
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerSelect : public CScene
{
public:
	enum E_State
	{// 状態
		STATE_NONE,		// 何もしていない状態
		STATE_START,	// 開始状態
		STATE_NORMAL,	// 通常状態 
		STATE_RESULT,	// リザルト状態
		STATE_END,		// 終了状態
		STATE_MAX
	};
	enum E_GameMode
	{// ゲームのモード
		MODE_NONE = 0,	// 何でもないモード
		MODE_SINGLE,	// シングルモード
		MODE_MULTI,		// マルチ
		MODE_MAX
	};
	enum E_StandyrState
	{// プレイヤーの準備状態
		
		STANDBY_WATE,
		STANDBY_OK,
		STANDBY_PLAY,
		STANDBY_MAX
	};

	CPlayerSelect();	// コンストラクタ
	~CPlayerSelect() {};	// デストラクタ

	static CPlayerSelect* GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(E_State state) { m_state = state; }
	static E_State GetState(void) { return m_state; }

	// 変数取得・設定関数
	vector<CLight*> GetLight(void) { return m_aLight; };	// ライトの配列
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// ライトの色
	void ResetDestColLight(void);	// ライト色リセット
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// 中心座標
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };

private:
	// メンバ関数
	void UpdateCamera(void);
	void Debug(void);
	void CreateLight(void);	// ライトの生成
	void UpdateLight(void);	// ライトの更新

	// メンバ変数
	vector<CLight*> m_aLight;	// ゲーム中のライト
	D3DXCOLOR m_colLight;	// ライト色
	D3DXVECTOR3 m_posMid;	// プレイヤーと敵の中心座標
	E_GameMode m_GameMode;	// ゲームモード
	int m_nCountPlayer;
	E_StandyrState m_StandbyState[MAX_PLAYER];	// プレイヤーの状態
	CUI* m_apPlayerUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ
	CUI* m_apStateUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ

	// 静的メンバ変数
	static E_State m_state;	// 状態
	static CPlayerSelect* m_pGame;	// 自身のポインタ

};

#endif