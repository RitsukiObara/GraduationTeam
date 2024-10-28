//*****************************************************
//
// �A�U���V�̏���[seals.h]
// Author:����F�M
//
//*****************************************************
#ifndef _SEALS_H_
#define _SEALS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSeals : public CEnemy
{
public:
	// ��ԗ�
	typedef enum
	{
		STATE_STOP = 0,	// ��~���
		STATE_MOVE,		// �ړ����
		STATE_ATTACK,	// �U�����
		STATE_DRIFT,	// �������
		STATE_MAX
	}STATE;

	// �֐��|�C���^�^�̒�`
	typedef bool (CSeals::*UpdateState)(void);

	CSeals(int nPriority = 4);	// �R���X�g���N�^
	~CSeals();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void UpdateStop(void);		// ��~��Ԃ̍X�V
	void UpdateMove(void);		// �ړ���Ԃ̍X�V
	void UpdateAttack(void);	// �U����Ԃ̍X�V
	void UpdateDrift(void);		// �Y����Ԃ̍X�V

	// �ÓI�����o�֐�
	static CSeals* Create(void);

private:
	// �A�U���V�̏��
	STATE m_State;
};

#endif
