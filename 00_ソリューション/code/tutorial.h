//*****************************************************
//
// �`���[�g���A������[tutorial.h]
// Author:���c�E��
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CBlink2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorial : public CScene
{
public:
	CTutorial();	// �R���X�g���N�^
	~CTutorial();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// �����o�֐�
	void UpdateBlinkUI(void);	// �I������̓_�ōX�V

	// �����o�ϐ�
	CUI *m_pManual;			// �������
	CBlink2D *m_pControl;	// �I��������
	float m_fCurTime;		// ���݂̑ҋ@����
};

#endif