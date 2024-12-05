//*****************************************************
//
// �}���`�p�̍d���X�̏���[iceHardMulti.h]
// Author:���R����
//
//*****************************************************
#ifndef _ICEHARDMULTI_H_
#define _ICEHARDMULTI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceHard.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIceHardMulti : public CIceHard
{
public:
	CIceHardMulti(int nPriority = 2);	// �R���X�g���N�^
	~CIceHardMulti();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_JUMP,		// �W�����v���
		STATE_MAX
	};

	// �����o�֐�
	void IsOnPlayer(void);		// �v���C���[�̌��o
	void UpdateState(void);		// ��Ԃ��Ƃ̍X�V
	void StartJump(void);			// ��΂�����
	void UpdateNormal(void);	// �ʏ�̍X�V
	void UpdateJump(void);		// �W�����v�̍X�V

	// �����o�ϐ�
	E_State m_state;					// ���
	float m_fTimerJump;					// ��΂��J�E���^�[
	vector<CPlayer*> m_apPlayerJump;	// ��΂��v���C���[�̔z��
};

#endif

