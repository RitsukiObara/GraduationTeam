//*****************************************************
//
// �w�i�X�̏���[BG_Ice.h]
// Author:����F�M
//
//*****************************************************
#ifndef _BG_ICE_H_
#define _BG_ICE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CBgIce : public CObjectX
{
public:
	CBgIce();	// �R���X�g���N�^
	~CBgIce();	// �f�X�g���N�^

	static CBgIce* Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

#endif
