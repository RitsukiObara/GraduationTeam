//*****************************************************
//
// �������̏���[peckLine.h]
// Author:���R����
//
//*****************************************************
#ifndef _PECKLINE_H_
#define _PECKLINE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CPeckLine : public CObject
{
public:
	CPeckLine();	// �R���X�g���N�^
	~CPeckLine();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPeckLine *Create(void);

private:
	// �����o�֐�
	
	// �����o�ϐ�
};

#endif