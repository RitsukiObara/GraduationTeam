//*****************************************************
//
// �X�e�[�W�I������[selectStage.h]
// Author:���R����
//
//*****************************************************
#ifndef _SELECTSTAGE_H_
#define _SELECTSTAGE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectStage : public CScene
{
public:
	CSelectStage();	// �R���X�g���N�^
	~CSelectStage() {};	// �f�X�g���N�^

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// �����o�֐�
	void UpdateCamera(void);
	void Debug(void);

	// �����o�ϐ�

	// �ÓI�����o�ϐ�
};

#endif