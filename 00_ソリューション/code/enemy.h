//*****************************************************
//
// �G�̏���[enemy.h]
// Author:����F�M
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CMotion
{
public:
	// �񋓌^��`
	enum TYPE
	{// ���
		TYPE_SEALS = 0,
		TYPE_MAX
	};
	enum E_State
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_STOP,		// ��~���
		STATE_MOVE,		// �ړ����
		STATE_ATTACK,	// �U�����
		STATE_DRIFT,	// �������
		STATE_MAX
	};

	// �֐��|�C���^�^�̒�`
	typedef void (CEnemy::*UpdateState)(void);

	CEnemy(int nPriority = 4);	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void TranslateByGrid(int nIdxV,int nIdxH);	// �O���b�h�ɂ��ړ�
	void FollowIce(void);	// �X�ɒǏ]

	virtual void UpdateStop(void) = 0;		// ��~��Ԃ̍X�V
	virtual void UpdateMove(void) = 0;		// �ړ���Ԃ̍X�V
	virtual void UpdateAttack(void) = 0;	// �U����Ԃ̍X�V
	virtual void UpdateDrift(void);		// �Y����Ԃ̍X�V

	// �ϐ��擾�E�ݒ�֐�
	void SetGridV(int nValue) { m_nGridV = nValue; }	// �O���b�h�̏c�ԍ�
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }	// �O���b�h�̉��ԍ�
	int GetGridVH(void) { return m_nGridH; }
	void SetGridVNext(int nValue) { m_nGridVNext = nValue; }	// ���̃O���b�h�̏c�ԍ�
	int GetGridVNext(void) { return m_nGridVNext; }
	void SetGridHNext(int nValue) { m_nGridHNext = nValue; }	// ���̃O���b�h�̉��ԍ�
	int GetGridHNext(void) { return m_nGridHNext; }
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }

	// �ÓI�����o�֐�
	static CEnemy* Create(int nType);	// ��������
	static std::vector<CEnemy*> GetArray(void) { return s_vector; }	// �z��̎擾

private:
	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	int m_nGridVNext;	// ���s���O���b�h�̏c�ԍ�
	int m_nGridHNext;	// ���s���O���b�h�̉��ԍ�
	E_State m_state;	// ���
	CIce *m_pIceLand;	// ����Ă���X

	// �ÓI�����o�ϐ�
	static std::vector<CEnemy*> s_vector;	// ���g�̃|�C���^
};

#endif