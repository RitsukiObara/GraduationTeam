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
	void UpdateStart(void) override;		// �J�n��Ԃ̍X�V
	void UpdateNormal(void) override;	// �ʏ��Ԃ̍X�V
	void UpdateResult(void) override;	// ���U���g��Ԃ̍X�V
	void UpdateEnd(void) override;		// �I����Ԃ̍X�V

	// �����o�ϐ�
	vector<CPlayer*> m_apPlayer;	// �v���C���[�̔z��

	// �ÓI�����o�ϐ�
};

#endif