//*****************************************************
//
// �X�e�[�W�I���Ǘ�����[SelectStageManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _SELECTSTAGEMANAGER_H_
#define _SELECTSTAGEMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStageManager.h"
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	CSelectStageManager();	// �R���X�g���N�^
	~CSelectStageManager() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);	// �ǂݍ��ݏ���

	// �ÓI�����o�֐�
	static CSelectStageManager *Create(void);	// ��������

private:
	// �����o�֐�

	// �����o�ϐ�

	// �ÓI�����o�ϐ�
};

#endif