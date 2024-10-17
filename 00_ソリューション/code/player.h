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
	static CPlayer* Create(void);
	static CPlayer* GetInstance(void) { return s_pPlayer; }

private:
	// �����o�֐�
	void Input(void);	// ����
	void MoveAnalog(void);	// �A�i���O�ړ�
	void InputMoveAnalog(void);	// �A�i���O�ړ�����
	void CollideIce(void);	// �X�Ƃ̔���
	void MoveGrid(void);	// �O���b�h�ړ�
	void MoveToGrid(void);	// �O���b�h�܂ł̈ړ�
	void LandCheck(void);	// ���n�m�F
	void InputPeck(void);	// �˂����̓���
	void Debug(void);	// �f�o�b�O����
	void MotionFinishCheck(void);	// ���[�V�������I���������m�F

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�
	bool m_bMove;	// �ړ����t���O
	bool m_bAnalog;	// �A�i���O����
	float m_jumpTime;	// �W�����v����

	// �ÓI�����o�ϐ�
	static CPlayer* s_pPlayer;	// ���g�̃|�C���^
};

#endif