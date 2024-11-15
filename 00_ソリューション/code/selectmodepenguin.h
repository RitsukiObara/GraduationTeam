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

//*****************************************************
// �N���X�̒�`
//*****************************************************
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
	static CSelectModePenguin* Create(void);	// ��������

private:
	// �����o�֐�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	CCollisionSphere *m_pClsnSphere;	// ���̔���
};

#endif