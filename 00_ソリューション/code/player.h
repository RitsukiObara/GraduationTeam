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
		MOTION_JUMPSTART,
		MOTION_JUMPFLY,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_MAX
	};

	// �v���C���[���
	enum STATE
	{
		STATE_NORMAL = 0,	// �ʏ�
		STATE_DEATH,		// ��
		STATE_INVINCIBLE,	// �f�o�b�O�������G
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPlayer* Create(void);
	static CPlayer* GetInstance(void) { return s_pPlayer; }

	// �擾�E�ݒ�
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	D3DXVECTOR3 GetPositionDest(void) { return m_posDest; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// �擾����

private:
	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	void Input(void);	// ����
	void MoveAnalog(void);	// �A�i���O�ړ�
	void InputMoveAnalog(void);	// �A�i���O�ړ�����
	void CollideIce(void);	// �X�Ƃ̔���
	void MoveGrid(void);	// �O���b�h�ړ�
	bool JudgeSarchOrMove(void);	// �I����Ԃ��ړ���Ԃ��̔���
	void UpdateInputSelectIce(void);	// �X�I����Ԃ̍X�V
	void UpdateInputMoveToIce(void);	// �X�Ɍ������Ĉړ����Ă����Ԃ̍X�V
	CIce *SelectIceByRot(float fRot);	// �X�������Ŏ擾
	void WalkToDestIce(CIce *pIceDest);	// �ڕW�̕X�Ɍ������Ĉړ����鏈��
	bool CheckGridChange(void);	// �O���b�h���ς�������ǂ����̔���
	void InputPeck(void);	// �˂����̓���
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	bool m_bAnalog;	// �A�i���O����
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	STATE m_state;		// �v���C���[���
	CIce *m_pIceMoveDest;	// �ړ��ڕW�̕X

	// �ÓI�����o�ϐ�
	static CPlayer* s_pPlayer;	// ���g�̃|�C���^
};

#endif