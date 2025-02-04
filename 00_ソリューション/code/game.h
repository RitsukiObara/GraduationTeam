//*****************************************************
//
// Q[[game.h]
// Author:ûüRç
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// CN[h
//*****************************************************
#include "main.h"
#include "scene.h"
#include "timer.h"
#include "score.h"
#include "UI_enemy.h"
#include "stageResultUI.h"
#include "pause.h"
#include "ocean_flow_UI.h"
#include "resultSingle.h"

//*****************************************************
// Oûé¾
//*****************************************************
class CGame;
class CLight;
class CPause;
class CGameManager;

//*****************************************************
// NXÌè`
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr int MAX_TIME_SINGLE = 120;	// VOÌÅåÔ
	static constexpr int MAX_TIME_MULTI = 180;	// }`ÌÅåÔ

	enum E_State
	{// óÔ
		STATE_NONE,		// ½àµÄ¢È¢óÔ
		STATE_READY,	// õóÔ
		STATE_START,	// JnóÔ
		STATE_NORMAL,	// ÊíóÔ 
		STATE_RESULT,	// UgóÔ
		STATE_END,		// I¹óÔ
		STATE_MAX
	};
	enum E_GameMode
	{// Q[Ì[h
		MODE_NONE = 0,	// ½ÅàÈ¢[h
		MODE_SINGLE,	// VO[h
		MODE_MULTI,		// }`
		MODE_MAX
	};

	CGame();		// RXgN^
	~CGame() {};	// fXgN^

	static CGame *GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(E_State state) { m_state = state; }
	static E_State GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }
	void ReleasePause(void);	// |[YÌðú
	void CreateScore(void);	// XRAÌ¶¬
	void StartGame(void);	// Q[ÌJn

	// Ïæ¾EÝèÖ
	int GetTimeSecond(void) { if (m_pTimer != nullptr) { return m_pTimer->GetSecond(); } return 0; }
	CScore *GetScore(void) { return m_pScore; };	// XRA
	void DecreaseNumEnemy(void) { m_nNumEnemyMax--; }	// GÌÅå
	void AddEnemy(int nType);
	int GetNumEnemyMax(void) { return m_nNumEnemyMax; }
	E_GameMode GetGameMode(void) { return m_GameMode; }

	// ÃIoÖ
	static void SetResult(CResultSingle::E_Result result) { s_result = result; };
	static CResultSingle::E_Result GetResult(void) { return s_result; }

private:
	// oÖ
	void InputPause(void);		// |[YÌüÍ
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void UpdatePause(void);	// |[YÌXV
	void CheckLimit(void);	//^C~bgmF

	// oÏ
	int m_nCntState;					// óÔJÚJE^[
	int m_nTimerCnt;					// ^C}[pÌJEg
	bool m_bStop;						// â~óÔ
	CTimer* m_pTimer;					// ^C}[
	CScore* m_pScore;					// XRA
	CStageResultUI* m_pStageResultUI;	// Xe[WNAUI
	CPause *m_pPause;					// |[YÌ|C^
	E_GameMode m_GameMode;				// Q[[h
	CGameManager *m_pGameManager;		// Q[}l[W[
	int m_nNumEnemyMax;					// GÌÅå
	bool m_bPlayTimeLimitSE;			// ^C~bgSEtO

	// ÃIoÏ
	static E_State m_state;						// óÔ
	static CGame *m_pGame;						// ©gÌ|C^
	static CResultSingle::E_Result s_result;	// Ug
};

namespace game
{
void AddScore(int nScore);	// XRAÁZ
}

#endif