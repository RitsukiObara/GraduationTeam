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
	void Update(void) override;
	void Draw(void) override;
	void EndGame(void) override;	// �Q�[���̏I��

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�

private:
	// �����o�֐�
	void UpdateStart(void) override;		// �J�n��Ԃ̍X�V
	void UpdateNormal(void) override;	// �ʏ��Ԃ̍X�V
	void ManagePlayer(void);	// �v���C���[�̊Ǘ�
	void DeathPlayer(int nIdx);	// �v���C���[�̎��S
	void CheckEndGame(void);	// �Q�[���I���̃`�F�b�N

	void UpdateResult(void) override;	// ���U���g��Ԃ̍X�V
	void UpdateEnd(void) override;		// �I����Ԃ̍X�V

	// �����o�ϐ�
	vector<CPlayer*> m_apPlayer;	// �v���C���[�̔z��
	int m_nNumDeathPlayer;	// �v���C���[�̎��̐�

	// �ÓI�����o�ϐ�
};

#endif