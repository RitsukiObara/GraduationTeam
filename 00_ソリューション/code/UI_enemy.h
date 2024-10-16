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

	// Ïæ¾EÝèÖ

	// ÃIoÖ
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return m_pUIEnemy; }

private:
	// oÏ
	CNumber *m_pNumber;	// Ì|C^

	// ÃIoÏ
	static CUIEnemy *m_pUIEnemy;	// ©gÌ|C^
};

#endif
