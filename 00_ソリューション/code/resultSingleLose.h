//*****************************************************
//
// �V���O���������U���g�̏���[resultSingleLose.h]
// Author:���R����
//
//*****************************************************
#ifndef _RESULTSINGLELOSE_H_
#define _RESULTSINGLELOSE_H_

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
class CResultSingleLose : public CResultSingle
{
public:
	enum E_State
	{
		STATE_NONE = 0,		// �������ĂȂ����
		STATE_END,			// �I�����
		STATE_MAX
	};
	CResultSingleLose();	// �R���X�g���N�^
	~CResultSingleLose();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�

private:
	// �񋓌^��`
	enum E_Select
	{
		SELECT_NO = 0,	// ������
		SELECT_YES,		// �͂�
		SELECT_MAX
	};

	// �����o�֐�
	void Create2D(void);	// 2DUI�̐���

	// �����o�ϐ�

	E_State m_state;		// ���
	float m_fTimer;	// �^�C�}�[

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CResultSingleLose::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�
};

#endif