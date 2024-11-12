//*****************************************************
//
// �}���`�Q�[���}�l�[�W���[[gameManagerMulti.h]
// Author:���R����
//
//*****************************************************
#ifndef _GAMEMANAGERMULTI_H_
#define _GAMEMANAGERMULTI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameManager.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGameManagerMulti : public CGameManager
{
public:
	CGameManagerMulti();	// �R���X�g���N�^
	~CGameManagerMulti() {};	// �f�X�g���N�^

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
	vector<CPlayer*> m_apPlayer;	// �v���C���[�̔z��

	// �ÓI�����o�ϐ�
};

#endif