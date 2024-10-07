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
#include "rikishi.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CRikishi
{
public:
	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float Hit(E_Attack typeAttack, float fDamage) override;

	// �ÓI�����o�֐�
	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }

private:
	// �����o�֐�
	void Input(void);	// ����
	void Attack(E_Attack typeAttack) override;
	void HitAttack(E_Attack typeAttack) override;

	// �����o�ϐ�
	float m_aTimerAttack[CRikishi::ATTACK_MAX];	// �U���J�E���^�[
	CPolygon3D *m_pGuide;	// �K�C�h�\��

	// �ÓI�����o�ϐ�
	static CPlayer *m_pPlayer;	// ���g�̃|�C���^
};

#endif