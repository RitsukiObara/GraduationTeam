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
		MOTION_UNYO,	// �񂤂�
		MOTION_CANNOTPECK,
		MOTION_MULTIAPPEAR,
		MOTION_WINGPTPT,	// �H���p�^�p�^
		MOTION_STOMACH,	// ���x�^
		MOTION_FALL,
		MOTION_MAX
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
	void MotionState(void);	// ���[�V�������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	CCollisionSphere *m_pCollisionSphere;	// ���̔���
	ISelectModePenguinState* m_pState;	// �����X�e�[�g

	int nCntMove;	// �ړ��̃J�E���g
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

//****************************************
// �ړ��X�e�[�g
//****************************************
class CSelectModePenguinState_Move : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Move() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// �]�уX�e�[�g
//****************************************
class CSelectModePenguinState_Fall : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Fall() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// ��U��X�e�[�g
//****************************************
class CSelectModePenguinState_ShakeHead : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_ShakeHead() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// ���x�^�X�e�[�g
//****************************************
class CSelectModePenguinState_Stomach : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_Stomach() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// ���ɂ��X�e�[�g
//****************************************
class CSelectModePenguinState_UNYO : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_UNYO() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

//****************************************
// �H�p�^�p�^�X�e�[�g
//****************************************
class CSelectModePenguinState_WingPTPT : public ISelectModePenguinState
{
public:
	CSelectModePenguinState_WingPTPT() {}
	HRESULT Init(CSelectModePenguin* pPenguin);
	void Update(CSelectModePenguin* pPenguin);
};

#endif