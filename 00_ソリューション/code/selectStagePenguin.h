//*****************************************************
//
// �X�e�[�W�I���y���M��[slectStagePenguin.h]
// Author:���R����
//
//*****************************************************
#ifndef _SELECTSTAGEPENGUIN_H_
#define _SELECTSTAGEPENGUIN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CInputManager;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectStagePenguin : public CMotion
{
public:
	CSelectStagePenguin(int nPriority = 4);	// �R���X�g���N�^
	~CSelectStagePenguin();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CSelectStagePenguin *Create(void);	// ��������

private:
	// �����o�֐�
	void Input(void);	// ����
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	CInputManager *m_pInputMgr;	// ���̓}�l�[�W���[
};

#endif