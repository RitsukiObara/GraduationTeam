//*****************************************************
//
// �`���[�g���A���v���C���[�̏���[PlayerTutorial.h]
// Author:���R����
//
//*****************************************************
#ifndef _PlayerTutorial_H_
#define _PlayerTutorial_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "player.h"
#include "tutorial.h"

//*****************************************************
// �O���錾
//*****************************************************s

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerTutorial : public CPlayer
{
public:
	CPlayerTutorial(int nPriority = 5);	// �R���X�g���N�^
	~CPlayerTutorial();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPlayerTutorial *Create(void);	// ��������

private:
	// �����o�֐�
	void CheckSpeed(void);						// �X�s�[�h�̊m�F
	void ReadExplain(void);						// �����i�ޏ���
	void AddProgress(CTutorial::E_State state);	// �i�s�����鏈��
	bool Peck(void) override;					// �˂�������
	void StartJump(void) override;				// �W�����v�̊J�n

	// �����o�ϐ�
	float m_fTimerMove;	// �ړ��J�E���^�[
	bool m_bComplete;	// �`���[�g���A�����I�������̔���
};

#endif