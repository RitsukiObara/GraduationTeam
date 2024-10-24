//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;
class CIce;
class CInputManager;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	// ���[�V������
	enum MOTION
	{
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_STARTJUMP,
		MOTION_STAYJUMP,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_FLOW,
		MOTION_MAX
	};
	// �v���C���[���
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ�
		STATE_DEATH,		// ��
		STATE_INVINCIBLE,	// �f�o�b�O�������G
		STATE_MAX
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �擾�E�ݒ�
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ړ���
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// ���͉\�t���O
	bool IsEnableInput(void) { return m_bEnableInput; }

	// �ÓI�����o�֐�
	static CPlayer* Create(void);

private:
	// �\���̒�`
	struct S_FragMotion
	{// ���[�V�����t���O�̍\����
		bool bWalk;	// ���s
		bool bPeck;	// �˂���
	};

	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	void Input(void);	// ����
	void MoveAnalog(void);	// �A�i���O�ړ�
	void InputMoveAnalog(void);	// �A�i���O�ړ�����
	void DisableTurn(void);	// �U��Ԃ�̖�����
	void Forward(void);	// �O�i����
	void DecreaseMove(void);	// �ړ��ʂ̌���
	void FactingRot(void);	// �����̕␳
	void JudgeTurn(void);	// �U������̌��o
	void CollideIce(void);	// �X�Ƃ̔���
	CIce *SelectIceByRot(float fRot);	// �X�������Ŏ擾
	void WalkToDestIce(CIce *pIceDest);	// �ڕW�̕X�Ɍ������Ĉړ����鏈��
	bool CheckGridChange(void);	// �O���b�h���ς�������ǂ����̔���
	void InputPeck(void);	// �˂����̓���
	void ManageMotion(void);	// ���[�V�����̊Ǘ�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	bool m_bEnableInput;	// ���͉\�t���O
	bool m_bTurn;	// �U��Ԃ��Ă���t���O
	float m_fRotTurn;	// �U��Ԃ�p�x
	D3DXVECTOR3 m_move;	// �ړ���
	float m_fTimerStartMove;	// �ړ��̗����オ��̃^�C�}�[
	STATE m_state;		// �v���C���[���
	CIce *m_pIceMoveDest;	// �ړ��ڕW�̕X
	S_FragMotion m_fragMotion;	// ���[�V�����t���O
	CInputManager *m_pInputMgr;	// ���̓}�l�[�W���[
};

#endif