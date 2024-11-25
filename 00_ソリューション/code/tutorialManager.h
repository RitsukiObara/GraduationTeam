//*****************************************************
//
// �`���[�g���A���}�l�[�W���[[tutorialManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "tutorial.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorialManager : public CObject
{
public:
	CTutorialManager();	// �R���X�g���N�^
	~CTutorialManager() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void ChangeState(CTutorial::E_State stateNext);	// ��Ԃ̕ύX

	// �ÓI�����o�֐�
	static CTutorialManager *Create(void);	// ��������

private:
	// �����o�֐�
	void UpdateMove(void);		// �ړ����
	void UpdatePeck(void);		// �˂������
	void UpdateBreak(void);		// �j����
	void UpdateExplain(void);	// �������

	// �֐��|�C���^�^���`
	typedef void (CTutorialManager::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�
};

#endif