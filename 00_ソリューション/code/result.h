//*****************************************************
//
// ���U���g�̏���[result.h]
// Author:���R����
//
//*****************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CTimer;

//*****************************************************
// �N���X��`
//*****************************************************
class CResult : CObject
{
public:
	enum STATE
	{
		STATE_NONE = 0,		// �������ĂȂ����
		STATE_FADE,			// �t�F�[�h���
		STATE_CLEAR_MOVE,	// �N���A�󋵈ړ����
		STATE_CONT_MOVE,	// �R���e�j���[�ړ����
		STATE_SELECT_MOVE,	// �I�����ڂ̈ړ����
		STATE_SELECT,		// �I�����
		STATE_END,			// �I�����
		STATE_MAX
	};

	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CResult *Create(bool bWin = false);
	static CResult *GetInstance(void) { return s_pResult; }

private:
	// �񋓌^��`
	enum SELECT
	{
		SELECT_YES = 0,	// �͂�
		SELECT_NO,		// ������
		SELECT_MAX
	};

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CResult::*AFuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static AFuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
	void Create2D(bool bWin);

	void UpdateFade(void);			// �t�F�[�h��Ԃ̍X�V
	void UpdateClearMove(void);		// �N���A�󋵈ړ���Ԃ̍X�V
	void UpdateContMove(void);		// �R���e�j���[�ړ���Ԃ̍X�V
	void UpdateSelectMove(void);	// �I�����ڂ̈ړ���Ԃ̍X�V
	void UpdateSelect(void);		// �I����Ԃ̍X�V

	// �����o�ϐ�
	CUI *m_pBg;			// �w�i�̃|�C���^
	CUI *m_pCaption;	// ���o���̃|�C���^
	CTimer* m_pCurTime;	// ����̌o�ߎ���

	CUI *m_pContinue;				// �R���e�j���[���S�̃|�C���^
	CUI *m_apSelect[SELECT_MAX];	// �I�����̃|�C���^

	STATE m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
	int m_nCurSelect;	// ���݂̑I����
	int m_nOldSelect;	// �O��̑I����

	// �ÓI�����o�֐�
	static CResult *s_pResult;	// ���g�̃|�C���^
};

#endif