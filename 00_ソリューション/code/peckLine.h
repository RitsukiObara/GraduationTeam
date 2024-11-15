//*****************************************************
//
// �������̏���[peckLine.h]
// Author:���R����
//
//*****************************************************
#ifndef _PECKLINE_H_
#define _PECKLINE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;
class CObject3D;
class COrbit;

//*****************************************************
// �N���X��`
//*****************************************************
class CPeckLine : public CObject
{
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_MOVE,		// �ړ����
		STATE_MAX
	};

	CPeckLine();	// �R���X�g���N�^
	~CPeckLine();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void StartMove(D3DXVECTOR3 posDest);	// �ړ��J�n
	void EndMove(void);						// �ړ��I��

	// �ϐ��擾�E�ݒ�֐�
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }

	// �ÓI�����o�֐�
	static CPeckLine *Create(CPlayer *pPlayer);

private:
	// �����o�֐�
	void CreateObject(void);	// �I�u�W�F�N�g�̐���
	void UpdateMove(void);		// �ړ���Ԃ̍X�V
	void FollowOrbit(void);		// �O�Ղ̒Ǐ]

	// �����o�ϐ�
	CPlayer *m_pPlayer;		// ������̃v���C���[
	CObject3D *m_pPosOrbit;	// �O�Ղ̐�[�p3D�I�u�W�F�N�g
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	COrbit *m_pOrbit;		// �O��
	float m_fTimer;			// �^�C�}�[
	float m_fTimerWait;		// �ҋ@�^�C�}�[
	E_State m_state;		// ���

	// ��ԍX�V�̊֐��|�C���^�^�G�C���A�X��`
	typedef void (CPeckLine::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�
};

#endif