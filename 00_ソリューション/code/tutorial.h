//*****************************************************
//
// �`���[�g���A������[tutorial.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CTutorialManager;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorial : public CScene
{
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_MOVE,		// �ړ����
		STATE_PECK,		// �˂������
		STATE_BREAK,	// �j����
		STATE_EXPLAIN,	// �������
		STATE_END,		// �I�����
		STATE_MAX
	};
	CTutorial();	// �R���X�g���N�^
	~CTutorial();	// �f�X�g���N�^

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	// �ϐ��擾�E�ݒ�֐�
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }

	// �ÓI�����o�֐�
	static CTutorial *GetInstance(void) { return s_pTutorial; }	// �C���X�^���X�擾

private:
	// �����o�֐�
	void UpdateMove(void);		// �ړ����
	void UpdateEnd(void);		// �I�����
	void Debug(void);			// �f�o�b�O

	// �����o�ϐ�
	E_State m_state;				// ���
	CTutorialManager *m_pManager;	// �`���[�g���A���}�l�[�W���[�̃|�C���^
	float m_fTimeEnd;				// �I���^�C�}�[

	// �֐��|�C���^�^���`
	typedef void (CTutorial::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�
	static CTutorial *s_pTutorial;					// ���g�̃|�C���^
};

#endif