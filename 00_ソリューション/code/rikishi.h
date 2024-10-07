//*****************************************************
//
// �͎m�̏���[rikishi.h]
// Author:���R����
//
//*****************************************************
#ifndef _RIKISHI_H_
#define _RIKISHI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CRikishi : public CMotion
{
public:
	// �񋓌^��`
	enum E_Attack
	{// �U���̎��
		ATTACK_RIGHT = 0,
		ATTACK_LEFT,
		ATTACK_BOTHHAND,
		ATTACK_MAX
	};
	enum E_Motion
	{// ���[�V����
		MOTION_GUARD = 0,
		MOTION_ATTACK_RIGHT,
		MOTION_ATTACK_LEFT,
		MOTION_ATTACK_BOTHHAND,
		MOTION_MAX
	};
	enum E_Parts
	{// �p�[�c�ԍ�
		IDX_WAIST = 0,
		IDX_BODY,
	};
	// �\���̒�`
	struct S_FragMotion
	{// ���[�V�����t���O
		bool bAttack[ATTACK_MAX];	// �U�����[�V����
		bool bGuard;	// �K�[�h���[�V����
	};
	
	CRikishi(int nPriority = 4);	// �R���X�g���N�^
	~CRikishi();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual float Hit(E_Attack typeAttack,float fDamage);
	virtual void Attack(E_Attack typeAttack);
	virtual void HitAttack(E_Attack typeAttack) = 0;
	void Guard(void);	// �K�[�h

	// �ϐ��擾�E�ݒ�֐�
	void SetFragMotion(S_FragMotion fragMotion) { m_fragMotion = fragMotion; }
	S_FragMotion GetFragMotion(void) { return m_fragMotion; }

	// �ÓI�����o�֐�
	static CRikishi *Create(void);

private:
	// �����o�֐�
	void ManageMotion(void);	// ���[�V�����Ǘ�
	void Event(EVENT_INFO *pEventInfo) override;

	// �����o�ϐ�
	S_FragMotion m_fragMotion;	// ���[�V�����t���O
};

namespace Rikishi
{
const float DISTANCE_RIKISHI = 100.0f;	// �͎m���m�̋���
const float DAMAGE_ATTACK = 100.0f;	// �U����
}

#endif