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
	CSeals(int nPriority = 4);	// �R���X�g���N�^
	~CSeals();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void UpdateApper(void) override;	// �o����Ԃ̍X�V
	bool CollideLand(void);	// ���n����

	void UpdateStop(void) override;		// ��~��Ԃ̍X�V

	void UpdateMove(void) override;		// �ړ���Ԃ̍X�V

	void UpdateAttack(void) override;	// �U����Ԃ̍X�V

	void UpdateDrift(void) override;	// �Y����Ԃ̍X�V

	void ManageMotion(void);	// ���[�V�����Ǘ�

	// �ÓI�����o�֐�
	static CSeals* Create(void);

private:
	enum E_Motion
	{// ���[�V����
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK,		// ����
		MOTION_STARTJUMP,	// �W�����v�J�n
		MOTION_STAYJUMP,	// �W�����v��
		MOTION_LANDING,		// ���n
		MOTION_MAX
	};

	// �����o�֐�
	void SetApperTransform(void) override;
	void CollidePlayer(void);	// �v���C���[�Ƃ̔���
	void MoveToIce(void);	// �X�Ɍ������Ĉړ�
	void SarchTarget(void);	// �^�[�Q�b�g�̒T��
	void FindPlayerGrid(void);	// �v���C���[�O���b�h�̔���

	// �����o�ϐ�
	CPlayer *m_pPlayerTarget;	// �^�[�Q�b�g�v���C���[
};

#endif
