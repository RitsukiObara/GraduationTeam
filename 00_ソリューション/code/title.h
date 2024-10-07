//*****************************************************
//
// �^�C�g������[title.h]
// Author:���c�E��
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObjectX;
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,		// �������ĂȂ����
		STATE_LOGO_MOVE,	// ���S�̈ړ����
		STATE_SELECT_MOVE,	// �I�����ڂ̈ړ����
		STATE_SELECT,		// �I�����
		STATE_END,			// �I�����
		STATE_MAX
	};

	CTitle();	// �R���X�g���N�^
	~CTitle();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	STATE GetState(void) { return m_state; }
	
private:
	enum SELECT
	{
		SELECT_GAME = 0,	// �Q�[���J�n
		SELECT_TUTORIAL,	// �`���[�g���A���\��
		SELECT_MAX
	};

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CTitle::*AFuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
	void UpdateLogoMove(void);		// ���S�̈ړ���Ԃ̍X�V
	void UpdateSelectMove(void);	// �I�����ڂ̈ړ���Ԃ̍X�V
	void UpdateSelect(void);		// �I����Ԃ̍X�V
	void CreateLight(void);			// ���C�g�̐���

	// �����o�ϐ�
	STATE m_state;					// ���
	CUI *m_apSelect[SELECT_MAX];	// �I�����̃|�C���^
	CUI *m_pLogo;					// ���S�̃|�C���^
	float m_fCurTime;				// ���݂̑ҋ@����
	int m_nCurSelect;				// ���݂̑I����
	int m_nOldSelect;				// �O��̑I����
};

#endif