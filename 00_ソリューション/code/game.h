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

//*****************************************************
// Oûé¾
//*****************************************************
class CGame;
class CLight;
class CPause;

//*****************************************************
// NXÌè`
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr float MAX_TIME = 60.0f;	// ÅåÔ

	enum E_State
	{// óÔ
		STATE_NONE,		// ½àµÄ¢È¢óÔ
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

	CGame();	// RXgN^
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
	void ReleasePause(void) { m_pPause = nullptr; };	// |[YÌðú

	// Ïæ¾EÝèÖ
	vector<CLight*> GetLight(void) { return m_aLight; };	// CgÌzñ
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// CgÌF
	void ResetDestColLight(void);	// CgFZbg
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// SÀW
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };
	float GetTimeSecond(void) { return m_pTimer->GetSecond(); }

private:
	// oÖ
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void CreateLight(void);	// CgÌ¶¬
	void UpdateLight(void);	// CgÌXV
	void UpdatePause(void);	// |[YÌXV

	// oÏ
	int m_nCntState;	// óÔJÚJE^[
	bool m_bStop;	// â~óÔ
	vector<CLight*> m_aLight;	// Q[ÌCg
	CTimer* m_pTimer;		// ^C}[
	CScore* m_pScore;		// XRA
	COceanFlowUI* m_pOceanFlowUI;		// C¬UI
	D3DXCOLOR m_colLight;	// CgF
	D3DXVECTOR3 m_posMid;	// vC[ÆGÌSÀW
	CStageResultUI* m_pStageResultUI;	// Xe[WNAUI
	CPause *m_pPause;	// |[YÌ|C^
	E_GameMode m_GameMode;	// Q[[h

	// ÃIoÏ
	static E_State m_state;	// óÔ
	static CGame *m_pGame;	// ©gÌ|C^
};

#endif