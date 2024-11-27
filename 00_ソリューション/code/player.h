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
class CFlowIce;
class CPeckLine;
class CShadow;

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
		MOTION_VICTORY,
		MOTION_FAIL,
		MOTION_GUTS,
		MOTION_NECKSHAKE,
		MOTION_UNYO,	// �񂤂�
		MOTION_CANNOTPECK,
		MOTION_MULTIAPPEAR,
		MOTION_WINGPTPT,	// �H���p�^�p�^
		MOTION_STOMACH,	// ���x�^
		MOTION_FALL,
		MOTION_MAX
	};
	// �v���C���[���
	enum E_State
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ�
		STATE_FLOW,		// ������Ă���
		STATE_DEATH,		// ��
		STATE_INVINCIBLE,	// �f�o�b�O�������G
		STATE_MAX
	};

	CPlayer(int nPriority = 5);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage) override;	// �q�b�g����
	virtual bool Peck(void);		// �˂�������

	// �擾�E�ݒ�
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// �ړ���
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// ���͉\�t���O
	bool IsEnableInput(void) { return m_bEnableInput; }
	void SetGridV(int nValue) { m_nGridV = nValue; }				// �O���b�h�̏c�ԍ�
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }				// �O���b�h�̉��ԍ�
	int GetGridH(void) { return m_nGridH; }
	void SetState(E_State state) { m_state = state; }				// ���
	E_State GetState(void) { return m_state; }
	int GetTimePeck(void) { return m_nTimePeck; }					// �X��˂�����
	void BindInputMgr(CInputManager *pInputMgr) { m_pInputMgr = pInputMgr; }	// ���̓}�l�[�W���[
	CInputManager *GetInpuManager(void) { return m_pInputMgr; }
	void SetID(int nID) { m_nID = nID; }	// �ԍ�
	int GetID(void) { return m_nID; }
	void SetShadowPos(D3DXVECTOR3 pos);

	// �ÓI�����o�֐�
	static CPlayer* Create(void);										// ��������
	static vector<CPlayer*> GetInstance(void) { return s_apPlayer; }	// �C���X�^���X�̎擾
	static void EnableInputAll(bool bValue);							// �S�v���C���[�̓��̓t���O
	static int GetNumPlayer(void) { return (int)s_apPlayer.size(); }	// �v���C���[���̎擾

protected:
	virtual void StartJump(void);	// �W�����v�̊J�n

private:
	// �\���̒�`
	struct S_FragMotion
	{// ���[�V�����t���O�̍\����
		bool bWalk;		// ���s
		bool bPeck;		// �˂���
		bool bJump;		// �W�����v
	};

	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	void CreateDirUI(void);	// ����UI�̐���

	void FollowIce(void);	// �X�ɒǏ]
	void Input(void);		// ����
	
	void MoveAnalog(void);		// �A�i���O�ړ�
	void InputMoveAnalog(void);	// �A�i���O�ړ�����
	void DisableTurn(void);		// �U��Ԃ�̖�����
	void Forward(void);			// �O�i����
	void DecreaseMove(void);	// �ړ��ʂ̌���
	void FactingRot(void);		// �����̕␳
	void JudgeTurn(void);		// �U������̌��o
	
	void CollideIce(void);	// �X�Ƃ̔���
	bool CheckAnyIceForward(void);	// �O���ɕX�����邩�̔���

	void InputPeck(void);			// �˂����̓���
	void RotationDirUI(int nDir);	// ����UI�̉�]
	
	CIce *SelectIceByRot(float fRot);	// �X�������Ŏ擾
	bool CheckGridChange(void);			// �O���b�h���ς�������ǂ����̔���
	
	void StartFlows(void);	// �Y���J�n
	bool FindFlowIce(void);	// �Y������X�̌��o
	void StayFlow(void);	// �Y�����̏���
	void FlowDeath(void);	// �Y�����̎�
	void EndFlows(void);	// �Y���I��

	void InputJump(void);			// �W�����v�̓���
	void SarchJumpIce(void);		// �W�����v��̕X��T��
	void StayJump(void);			// �W�����v���̏���
	void LimitInSideFlowIce(void);	// ���X�̓����ɐ���
	void EndJump(void);				// �W�����v�̏I��
	
	void FollowDirUI(void);	// ����UI�̒Ǐ]

	void Event(EVENT_INFO* pEventInfo) override;	// ���[�V�����C�x���g
	void ManageMotion(void);						// ���[�V�����̊Ǘ�

	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�

	bool m_bEnableInput;	// ���͉\�t���O
	bool m_bTurn;			// �U��Ԃ��Ă���t���O
	float m_fRotTurn;		// �U��Ԃ�p�x

	D3DXVECTOR3 m_move;			// �ړ���
	float m_fTimerStartMove;	// �ړ��̗����オ��̃^�C�}�[
	E_State m_state;			// �v���C���[���

	vector<CIce*> m_apIceJump;	// �W�����v��̌��̕X
	CIce *m_pIceDestJump;		// �W�����v�ڕW�̕X
	bool m_bEnableJump;			// �W�����v�\�t���O
	D3DXVECTOR3 m_posInitJump;	// �W�����v�����ʒu

	CIce *m_pIceMoveDest;			// �ړ��ڕW�̕X
	CIce *m_pLandFlow;				// �Y�����ɏ���Ă�X�̃|�C���^
	CFlowIce *m_pLandSystemFlow;	// ����Ă闬�X�V�X�e��
	int m_nTimePeck;				// �X��˂�����
	CPolygon3D *m_pDir;				// �����������|���S��
	CPeckLine *m_pPeckLine;			// ������
	CShadow *m_pShadow;				// �e�̃|�C���^

	S_FragMotion m_fragMotion;	// ���[�V�����t���O

	int m_nID;					// �ԍ�
	CInputManager *m_pInputMgr;	// ���̓}�l�[�W���[

	// �ÓI�����o�ϐ�
	static vector<CPlayer*> s_apPlayer;	// �i�[�p�̔z��
};

#endif