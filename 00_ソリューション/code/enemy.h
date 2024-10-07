//*****************************************************
//
// �G�̏���[enemy.h]
// Author:���R����
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "rikishi.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CRikishi
{
public:
	CEnemy(int nPriority = 4);	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	float Hit(E_Attack typeAttack, float fDamage) override;	// �q�b�g

	// �ÓI�����o�֐�
	static CEnemy *Create(void);
	static CEnemy *GetInstance(void) { return m_pEnemy; }

private:
	// �����o�֐�
	void Debug(void);	// �f�o�b�O
	void Attack(E_Attack typeAttack) override;	// �U��
	void UpdatePosition(void);	// �ʒu�X�V
	void UpdateAttack(void);	// �U���X�V
	void RandomAttack(void);	// �U�������_������
	void HitAttack(E_Attack typeAttack) override;

	// �ÓI�����o�ϐ�
	static CEnemy *m_pEnemy;	// ���g�̃|�C���^

	// �����o�ϐ�
	int m_aAtkNum[3];		// �e�U���̊m��
	int m_nMaxAtkStart;		// �U���J�n�l�̍ő�l
	float m_fLastAtkTime;	// �O��U������̌o�ߎ���
};

#endif