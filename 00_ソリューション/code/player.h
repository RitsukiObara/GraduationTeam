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

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	// ���[�V������
	enum EMotion
	{
		MOTION_NEUTRAL = 0,
		MOTION_JUMPSTART,
		MOTION_JUMPFLY,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_MAX
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return s_pPlayer; }

	// �擾����
	D3DXVECTOR3 GetMove(void) { return m_move; }	
	D3DXVECTOR3 GetPositionDest(void) { return m_posDest; }

	// �ݒ菈��
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	virtual void SetPosition(D3DXVECTOR3 pos) override { CGameObject::SetPosition(pos); m_posDest = pos; }

private:
	// �����o�֐�
	void Input(void);	// ����
	void MoveAnalog(void);	// �A�i���O�ړ�
	void InputMoveAnalog(void);	// �A�i���O�ړ�����
	void CollideIce(void);	// �X�Ƃ̔���
	void MoveGrid(void);	// �O���b�h�ړ�
	void MoveToGrid(void);	// �O���b�h�܂ł̈ړ�
	void Debug(void);	// �f�o�b�O����
	void MotionFinishCheck(void);	// ���[�V�������I���������m�F
	void MovePositionDest(void);

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu

	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	int m_motionType;	//���[�V�����̎��
	bool m_bMove;	// �ړ����t���O
	float m_jumpTime;	// �W�����v����

	// �ÓI�����o�ϐ�
	static CPlayer *s_pPlayer;	// ���g�̃|�C���^
};

#endif