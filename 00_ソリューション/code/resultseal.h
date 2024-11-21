//*****************************************************
//
// ���U���g�̃A�U���V[resultseal.h]
// Author:�Ό��D�n
//
//*****************************************************
#ifndef _RESULTSEAL_H_
#define _RESULTSEAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CShadow;

//*****************************************************
// �N���X�̒�`
//*****************************************************
//****************************************
// ���[�h�I����ʂ̗V�ԃy���M���I�u�W�F�N�g�N���X
//****************************************
class CResultSeal : public CMotion
{
public:
	// ���[�V������
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK,		// ����
		MOTION_STARTJUMP,	// �W�����v�J�n
		MOTION_STAYJUMP,	// �W�����v��
		MOTION_LANDING,		// ���n
		MOTION_SWIM,		// �j��
		MOTION_TURN,		// �����]��
		MOTION_MAX
	};

	CResultSeal(D3DXVECTOR3 pos, int nPriority = 4);	// �R���X�g���N�^
	~CResultSeal();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CResultSeal* Create(D3DXVECTOR3 pos);	// ��������

private:
	// �����o�֐�
	void Move(void);

	// �����o�ϐ�
	CCollisionSphere *m_pCollisionSphere;	// ���̔���
	CShadow *m_pShadow;						// �e�̃|�C���^
	int m_nCounterMove;						// �O�C�b���ē����܂ł̃J�E���^
	int m_bMove;							// �A�U���V�����܂�
	float m_fRotateSpeed;						// �ړ����x
	const D3DXVECTOR3 c_centorPos;			// ��钆�S�ʒu
};

#endif