//*****************************************************
//
// ���e�̏���[fishshadow.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

#include "enemy.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CFishShadow : public CPolygon3D
{
public:
	CFishShadow();	// �R���X�g���N�^
	~CFishShadow();	// �f�X�g���N�^

	static CFishShadow* Create(CEnemy::TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif