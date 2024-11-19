//*****************************************************
//
// ���[�h�I���y���M��[selectmodepenguin.h]
// Author:�Ό��D�n
//
//*****************************************************
#ifndef _SELECTMODEPENGUIN_H_
#define _SELECTMODEPENGUIN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
__interface ISelectModePenguinState;

//*****************************************************
// �N���X�̒�`
//*****************************************************
//****************************************
// ���[�h�I����ʂ̗V�ԃy���M���I�u�W�F�N�g�N���X
//****************************************
class CSelectModePenguin : public CMotion
{
public:
		// ���[�V������
	enum MOTION
	{
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_STARTJUMP,
		MOTION_STAYJUMP,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_FLOW,
		MOTION_VICTORY,
		MOTION_FAIL,
		MOTION_GUTS,
		MOTION_NECKSHAKE,
		MOTION_UNYO,
		MOTION_CANNOTPECK,
		MOTION_MAX
	};

	// �\���̒�`
	struct S_FragMotion
	{// ���[�V�����t���O�̍\����
		bool bWalk;		// ���s
		bool bPeck;		// �˂���
		bool bJump;		// �W�����v
	};

	CSelectModePenguin(int nPriority = 4);	// �R���X�g���N�^
	~CSelectModePenguin();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CSelectModePenguin* Create(ISelectModePenguinState* pState = nullptr);	// ��������

	// �ݒ�
	void SetState(ISelectModePenguinState* pState);
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// ���͉\�t���O

private:
	void ManageMotion(void);			// ���[�V�����̊Ǘ�
	void EndJump(void);				// �W�����v�̏I��

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	CCollisionSphere *m_pCollisionSphere;	// ���̔���
	ISelectModePenguinState* m_pState;	// �����X�e�[�g
	S_FragMotion m_fragMotion;	// ���[�V�����t���O
	bool m_bEnableInput;	// ���͉\�t���O
};

//****************************************
// �����X�e�[�g
//****************************************
//----------------------------------------
// �y���M���̓����X�e�[�g���C���^�[�t�F�[�X
//----------------------------------------
__interface ISelectModePenguinState
{
	HRESULT Init(CSelectModePenguin *pPenguin) = 0;
	void Update(CSelectModePenguin* pPenguin) = 0;
};

//****************************************
//		������������������X�e�[�g�ǉ�����������
//****************************************
//****************************************
// �����Ă邾���X�e�[�g
//****************************************
class CSelectModePenguinState_Stand : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stand(){}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

#endif