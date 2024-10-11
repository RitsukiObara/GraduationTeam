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

//*****************************************************
// 前方宣言
//*****************************************************
class CGame;
class CLight;

//*****************************************************
// クラスの定義
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr float MAX_TIME = 60.0f;	// 最大時間

	enum STATE
	{// 状態
		STATE_NONE,		// 何もしていない状態
		STATE_START,	// 開始状態
		STATE_NORMAL,	// 通常状態
		STATE_RESULT,	// リザルト状態
		STATE_END,		// 終了状態
		STATE_MAX
	};

	CGame();	// コンストラクタ
	~CGame() {};	// デストラクタ

	static CGame *GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(STATE state) { m_state = state; }
	static STATE GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }

	// 変数取得・設定関数
	vector<CLight*> GetLight(void) { return m_aLight; };	// ライトの配列
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// ライトの色
	void ResetDestColLight(void);	// ライト色リセット
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// 中心座標
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };
	float GetTimeSecond(void) { return m_pTimer->GetSecond(); }

private:
	// メンバ関数
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void CreateLight(void);	// ライトの生成
	void UpdateLight(void);	// ライトの更新

	// メンバ変数
	int m_nCntState;	// 状態遷移カウンター
	bool m_bStop;	// 停止状態
	vector<CLight*> m_aLight;	// ゲーム中のライト
	CTimer* m_pTimer;		// タイマー
	//CScore* m_pScore;		// スコア
	D3DXCOLOR m_colLight;	// ライト色
	D3DXVECTOR3 m_posMid;	// プレイヤーと敵の中心座標

	// 静的メンバ変数
	static STATE m_state;	// 状態
	static CGame *m_pGame;	// 自身のポインタ
};

#endif