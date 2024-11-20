//*****************************************************
//
// �V���N�}�̏���[bears.h]
// Author:���R����
//
//*****************************************************
#ifndef _BEARS_H_
#define _BEARS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBears : public CEnemy
{
public:
	CBears(int nPriority = 4);	// �R���X�g���N�^
	~CBears();	// �f�X�g���N�^

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
	static CBears* Create(void);

private:
	enum E_Motion
	{// ���[�V����
		MOTION_WALK = 0,	// ����
		MOTION_CHARGE,		// �ˌ�
		MOTION_STARTJUMP,	// �W�����v�J�n
		MOTION_STAYJUMP,	// �W�����v��
		MOTION_LANDING,		// ���n
		MOTION_TURN,		// �U�����
		MOTION_TURNCHARGE,	// �ːi�U�����
		MOTION_MAX
	};

	// �����o�֐�
	void SetApperTransform(void) override;
	void CollidePlayer(void);						// �v���C���[�Ƃ̔���
	void MoveToIce(void);							// �X�Ɍ������Ĉړ�
	void Decreasemove(void);						// �ړ��ʂ̌���
	void SarchTarget(void);							// �^�[�Q�b�g�̒T��
	void StopMoveByNotGrid(CIce *pIce) override;	// �O���b�h�����Ȃ��ړ����~�߂�

	bool CanCharge(D3DXVECTOR3 pos, int nIdxTargetV, int nIdxTargetH);							// �ˌ��ł��邩�̔���
	bool IsAliveTarget(int nIdxV, int nIdxH,float fRot,int nIdxTargetV, int nIdxTargetH);		// �^�[�Q�b�g�ɓ��B�������̍ċA�֐�

	void ReadyCharge(void);	// �ˌ��̏���
	void StartCharge(void);	// �ˌ��̊J�n
	void Charge(void);		// �ˌ����̏���
	void EndCharge(void);	// �ˌ��̏I��

	void AliveDestGrid(void) override;				// �O���b�h�ɓ��������Ƃ��̏���
	void DecideNextStrollGrid(void);				// ���̎U��������߂�
	void Death(void) override;						// ���S���̏���
	void Event(EVENT_INFO* pEventInfo) override;	// ���[�V�����C�x���g

	// �����o�ϐ�
	CPlayer *m_pPlayerTarget;	// �^�[�Q�b�g�v���C���[
	D3DXVECTOR3 m_vecCharge;	// �ːi����x�N�g��
	float m_fTimerAcceleCharge;	// �ːi�����J�E���^�[
};

#endif
