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
class CPlayer;
class CEnemyFct;

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
	void UpdateStart(void) override;	// �J�n��Ԃ̍X�V
	void UpdateNormal(void) override;	// �ʏ��Ԃ̍X�V
	void ManagePlayer(void);			// �v���C���[�̊Ǘ�
	void DeathPlayer(void);				// �v���C���[�̎��S
	void ManageEnemy(void);				// �G�̊Ǘ�

	void UpdateResult(void) override;	// ���U���g��Ԃ̍X�V
	void UpdateEnd(void) override;		// �I����Ԃ̍X�V

	// �����o�ϐ�
	CPlayer *m_pPlayer;		// �v���C���[�̃|�C���^
	CEnemyFct *m_pEnemyFct;	// �G�t�@�N�g���[

	// �ÓI�����o�ϐ�
};

#endif