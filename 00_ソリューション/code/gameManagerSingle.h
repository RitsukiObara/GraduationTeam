//*****************************************************
//
// �V���O���Q�[���}�l�[�W���[[gameManagerSingle.h]
// Author:���R����
//
//*****************************************************
#ifndef _GAMEMANAGERSINGLE_H_
#define _GAMEMANAGERSINGLE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameManager.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGameManagerSingle : public CGameManager
{
public:
	CGameManagerSingle();	// �R���X�g���N�^
	~CGameManagerSingle() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update() override;
	void Draw() override;

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�

private:
	// �����o�֐�

	// �����o�ϐ�

	// �ÓI�����o�ϐ�
};

#endif