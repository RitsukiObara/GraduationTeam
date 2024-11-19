//*****************************************************
//
// ゲーム処理[game.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"
#include "timer.h"
#include "score.h"
#include "UI_enemy.h"
#include "stageResultUI.h"
#include "pause.h"
#include "ocean_flow_UI.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CGame;
class CLight;
class CPause;

//*****************************************************
// クラスの定義
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr int MAX_TIME = 120;	// 最大時間

	enum E_State
	{// 状態
		STATE_NONE,		// 何もしていない状態
		STATE_READY,	// 準備状態
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

	CGame();	// コンストラクタ
	~CGame() {};	// デストラクタ

	static CGame *GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(E_State state) { m_state = state; }
	static E_State GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }
	void ReleasePause(void) { m_pPause = nullptr; };	// ポーズの解放
	void CreateScore(void);	// スコアの生成
	void StartGame(void);	// ゲームの開始

	// 変数取得・設定関数
	int GetTimeSecond(void) { if (m_pTimer != nullptr) { return m_pTimer->GetSecond(); } return 0; }
	CScore *GetScore(void) { return m_pScore; };	// スコア

private:
	// メンバ関数
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void UpdatePause(void);	// ポーズの更新

	// メンバ変数
	int m_nCntState;	// 状態遷移カウンター
	int m_nTimerCnt;	// タイマー用のカウント
	bool m_bStop;	// 停止状態
	CTimer* m_pTimer;		// タイマー
	CScore* m_pScore;		// スコア
	CStageResultUI* m_pStageResultUI;	// ステージクリアUI
	CPause *m_pPause;	// ポーズのポインタ
	E_GameMode m_GameMode;	// ゲームモード

	// 静的メンバ変数
	static E_State m_state;	// 状態
	static CGame *m_pGame;	// 自身のポインタ
};

namespace game
{
void AddScore(int nScore);	// スコア加算
}

#endif