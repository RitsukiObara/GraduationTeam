//*****************************************************
//
// G\¦UI[UI_enemy.h]
// Author:ûüRç
//
//*****************************************************

#ifndef _UIENEMY_H_
#define _UIENEMY_H_

//*****************************************************
// CN[h
//*****************************************************
#include "object.h"
#include "UI.h"

//*****************************************************
// Oûé¾
//*****************************************************
class CNumber;
class CIcon;

//*****************************************************
// NXÌè`
//*****************************************************
class CUIEnemy : public CObject
{// G\¦UI
public:
	CUIEnemy();	// RXgN^
	~CUIEnemy();	// fXgN^

	// oÖ
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddEnemy(int nType);	// GÌÇÁ
	void DeleteEnemy(int nType);	// GÌí

	// Ïæ¾EÝèÖ

	// ÃIoÖ
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return s_pUIEnemy; }

private:
	// oÖ
	void ManageIcon(void);	// ACRÌÇ
	void Debug(void);	// fobO

	// oÏ
	vector<CIcon*> m_apIcon;	// ACRÌzñ

	// ÃIoÏ
	static CUIEnemy *s_pUIEnemy;	// ©gÌ|C^
};

class CIcon : public CUI
{// G\¦ACR
public:
	// ñ^è`
	enum E_State
	{// óÔ
		STATE_NONE = 0,	// ½àµÄ¢È¢óÔ
		STATE_NORMAL,	// ÊíóÔ
		STATE_FALL,	// ºóÔ
		STATE_MAX
	};

	CIcon() : CUI(), m_state(E_State::STATE_NONE), m_nType(0) {};	// RXgN^
	~CIcon() {};	// fXgN^

	// oÖ
	HRESULT Init(void);
	void Uninit(void) { CUI::Uninit(); };
	void Update(void);
	void Draw(void) { CUI::Draw(); };
	void StartFall(void);

	// Ïæ¾EÝèÖ
	void SetState(E_State state) { m_state = state; }	// óÔ
	E_State GetState(void) { return m_state; }
	void SetType(int nValue) { m_nType = nValue; }		// íÞ
	int GetType(void) { return m_nType; }

	// ÃIoÖ
	static CIcon *Create(int nType);

private:
	// oÖ
	void UpdateFall(void);	// ºÌXV

	// oÏ
	E_State m_state;	// óÔ
	D3DXVECTOR3 m_move;	// Ú®Ê
	int m_nType;		// íÞ
};

#endif
