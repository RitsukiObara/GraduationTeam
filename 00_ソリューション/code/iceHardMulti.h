//*****************************************************
//
// �}���`�p�̍d���X�̏���[iceHardMulti.h]
// Author:���R����
//
//*****************************************************
#ifndef _ICEHARDMULTI_H_
#define _ICEHARDMULTI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceHard.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIceHardMulti : public CIceHard
{
public:
	CIceHardMulti(int nPriority = 2);	// �R���X�g���N�^
	~CIceHardMulti();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool IsCanPeck(void) override { return false; };	// �˂����邩�̔����Ԃ�

	// �ϐ��擾�E�ݒ�֐�
	void EnableBreak(bool bBreak) override {};

	// �ÓI�����o�֐�

private:

};

#endif

