//*****************************************************
//
// �G���\��UI[UI_enemy.h]
// Author:���R����
//
//*****************************************************

#ifndef _UIENEMY_H_
#define _UIENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIEnemy : public CObject
{
public:

	CUIEnemy();	// �R���X�g���N�^
	~CUIEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return m_pUIEnemy; }

private:
	// �����o�ϐ�
	CNumber *m_pNumber;	// �����̃|�C���^

	// �ÓI�����o�ϐ�
	static CUIEnemy *m_pUIEnemy;	// ���g�̃|�C���^
};

#endif
