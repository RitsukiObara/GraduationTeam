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
class CCollisionSphere;

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
	void InputMove(void);	// �ړ�����
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	CInputManager *m_pInputMgr;	// ���̓}�l�[�W���[
	CCollisionSphere *m_pClsnSphere;	// ���̔���
};

#endif