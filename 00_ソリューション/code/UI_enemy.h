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
class CUI;

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
	void AddEnemy(void);	// �G�̒ǉ�
	void DeleteEnemy(void);	// �G�̍폜

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return s_pUIEnemy; }

private:
	// �����o�֐�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	vector<CUI*> m_apIcon;	// �A�C�R���̔z��

	// �ÓI�����o�ϐ�
	static CUIEnemy *s_pUIEnemy;	// ���g�̃|�C���^
};

#endif
