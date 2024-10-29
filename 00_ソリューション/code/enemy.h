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
class CIce;

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
		STATE_APPER,	// �o�����
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

	virtual void UpdateApper(void) = 0;		// �o����Ԃ̍X�V
	virtual void UpdateStop(void) = 0;		// ��~��Ԃ̍X�V
	virtual void UpdateMove(void);		// �ړ���Ԃ̍X�V
	virtual void UpdateAttack(void) = 0;	// �U����Ԃ̍X�V
	virtual void UpdateDrift(void);		// �Y����Ԃ̍X�V

	// �ϐ��擾�E�ݒ�֐�
	void SetGridV(int nValue) { m_nGridV = nValue; }	// �O���b�h�̏c�ԍ�
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }	// �O���b�h�̉��ԍ�
	int GetGridH(void) { return m_nGridH; }
	void SetGridVNext(int nValue) { m_nGridVNext = nValue; }	// ���̃O���b�h�̏c�ԍ�
	int GetGridVNext(void) { return m_nGridVNext; }
	void SetGridHNext(int nValue) { m_nGridHNext = nValue; }	// ���̃O���b�h�̉��ԍ�
	int GetGridHNext(void) { return m_nGridHNext; }
	void EnableFollowIce(bool bValue) { m_bFollowIce = bValue; }	// �X�Ǐ]�t���O
	bool IsFollowIce(void) { return m_bFollowIce; }
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ړ���
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// �ÓI�����o�֐�
	static CEnemy* Create(int nType,int nGridV = 0, int nGridH = 0);	// ��������
	static std::vector<CEnemy*> GetArray(void) { return s_vector; }	// �z��̎擾

private:
	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	virtual void SetApperTransform(void) = 0;	// �o�����̃g�����X�t�H�[���ݒ�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	int m_nGridVNext;	// ���s���O���b�h�̏c�ԍ�
	int m_nGridHNext;	// ���s���O���b�h�̉��ԍ�
	bool m_bFollowIce;	// �X�Ǐ]�t���O
	E_State m_state;	// ���
	CIce *m_pIceLand;	// ����Ă���X
	D3DXVECTOR3 m_move;	// �ړ���

	// �ÓI�����o�ϐ�
	static std::vector<CEnemy*> s_vector;	// ���g�̃|�C���^
};

#endif