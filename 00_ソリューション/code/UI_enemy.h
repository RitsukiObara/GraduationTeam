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

//*****************************************************
// Oûé¾
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// NXÌè`
//*****************************************************
class CUIEnemy : public CObject
{
public:

	CUIEnemy();	// RXgN^
	~CUIEnemy();	// fXgN^

	// oÖ
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddEnemy(void);	// GÌÇÁ
	void DeleteEnemy(void);	// GÌí

	// Ïæ¾EÝèÖ

	// ÃIoÖ
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return s_pUIEnemy; }

private:
	// oÖ
	void Debug(void);	// fobO

	// oÏ
	vector<CUI*> m_apIcon;	// ACRÌzñ

	// ÃIoÏ
	static CUIEnemy *s_pUIEnemy;	// ©gÌ|C^
};

#endif
