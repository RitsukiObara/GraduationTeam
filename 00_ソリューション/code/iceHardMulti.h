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

private:
	// �����o�֐�
	void IsOnPlayer(void);	// �v���C���[�̌��o
	void UpdateState(void);	// ��Ԃ��Ƃ̍X�V

	// �����o�ϐ�
	float m_fTimerSink;	// ���ރJ�E���^�[
};

#endif

