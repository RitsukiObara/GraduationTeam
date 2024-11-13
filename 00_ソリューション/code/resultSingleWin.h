//*****************************************************
//
// �V���O���������U���g�̏���[resultSingleWin.h]
// Author:���R����
//
//*****************************************************
#ifndef _RESULTSINGLEWIN_H_
#define _RESULTSINGLEWIN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultSingle.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CNumber;

//*****************************************************
// �N���X��`
//*****************************************************
class CResultSingleWin : public CResultSingle
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// �������ĂȂ����
		STATE_MOVECAMERA,	// �J�����ړ����
		STATE_APPERSCORE,	// �X�R�A�o��
		STATE_END,			// �I�����
		STATE_MAX
	};
	enum E_ScoreCaption
	{
		CAPTION_SCORE = 0,	// �X�R�A
		CAPTION_TIMEPECK,	// ��������
		CAPTION_MAX
	};

	CResultSingleWin();	// �R���X�g���N�^
	~CResultSingleWin();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�

private:
	// �����o�֐�
	void Create2D(void);	// 2DUI�̐���
	void CreatepOwnScore(void);	// ���g�̃X�R�A�̕\��

	void UpdateApperScore(void);	// �X�R�A�o����Ԃ̍X�V

	// �����o�ϐ�
	CUI *m_apCaptionScore[CAPTION_MAX];		// ���g�̃L���v�V�����̃|�C���^
	CNumber *m_apNumberOwn[CAPTION_MAX];	// ���g�̐��т̐����|�C���^

	E_State m_state;		// ���
	float m_fTimer;	// �^�C�}�[

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CResultSingleWin::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�
};

#endif