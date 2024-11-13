//*****************************************************
//
// �V���O�����U���g�̏���[resultSingle.h]
// Author:���R����
//
//*****************************************************
#ifndef _RESULTSINGLE_H_
#define _RESULTSINGLE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X��`
//*****************************************************
class CResultSingle : CObject
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// �������ĂȂ����
		STATE_FADE,			// �t�F�[�h���
		STATE_SELECT,		// �I�����
		STATE_END,			// �I�����
		STATE_MAX
	};

	CResultSingle();	// �R���X�g���N�^
	~CResultSingle();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CResultSingle *Create(bool bWin = false);

private:
	// �񋓌^��`
	enum SELECT
	{
		SELECT_YES = 0,	// �͂�
		SELECT_NO,		// ������
		SELECT_MAX
	};

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CResultSingle::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState m_aFuncUpdateState[];	// ��ԍX�V�֐�

	// �����o�֐�
	void Create2D(bool bWin);

	void UpdateFade(void);			// �t�F�[�h��Ԃ̍X�V
	void UpdateSelect(void);		// �I����Ԃ̍X�V

	// �����o�ϐ�
	CUI *m_pBg;			// �w�i�̃|�C���^
	CUI *m_pCaption;	// ���o���̃|�C���^

	CUI *m_pContinue;				// �R���e�j���[���S�̃|�C���^
	CUI *m_apSelect[SELECT_MAX];	// �I�����̃|�C���^

	E_State m_state;		// ���
	float m_fCurTime;	// ���݂̑ҋ@����
	int m_nCurSelect;	// ���݂̑I����
	int m_nOldSelect;	// �O��̑I����
};

#endif