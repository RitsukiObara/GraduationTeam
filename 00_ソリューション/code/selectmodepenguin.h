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

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	CCollisionSphere *m_pCollisionSphere;	// ���̔���
	ISelectModePenguinState* m_pState;	// �����X�e�[�g
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