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

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorialManager : public CObject
{
public:
	CTutorialManager();	// �R���X�g���N�^
	~CTutorialManager() {};	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// �ÓI�����o�֐�
	static CTutorialManager *Create(void);	// ��������

private:
};

#endif