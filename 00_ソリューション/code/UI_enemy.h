//*****************************************************
//
// �G���\��UI[UI_enemy.h]
// Author:���R����
//
//*****************************************************

#ifndef _UIENEMY_H_
#define _UIENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "UI.h"

//*****************************************************
// �O���錾
//*****************************************************
class CNumber;
class CIcon;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIEnemy : public CObject
{// �G�\��UI
public:
	CUIEnemy();	// �R���X�g���N�^
	~CUIEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddEnemy(void);	// �G�̒ǉ�
	void DeleteEnemy(void);	// �G�̍폜

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�
	static CUIEnemy *Create(void);
	static CUIEnemy *GetInstance(void) { return s_pUIEnemy; }

private:
	// �����o�֐�
	void ManageIcon(void);	// �A�C�R���̊Ǘ�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	vector<CIcon*> m_apIcon;	// �A�C�R���̔z��

	// �ÓI�����o�ϐ�
	static CUIEnemy *s_pUIEnemy;	// ���g�̃|�C���^
};

class CIcon : public CUI
{// �G�\���A�C�R��
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_FALL,	// �������
		STATE_MAX
	};

	CIcon() : CUI(), m_state(E_State::STATE_NONE) {};	// �R���X�g���N�^
	~CIcon() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void) { CUI::Uninit(); };
	void Update(void);
	void Draw(void) { CUI::Draw(); };
	void StartFall(void);

	// �ϐ��擾�E�ݒ�֐�
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }

	// �ÓI�����o�֐�
	static CIcon *Create(void);

private:
	// �����o�֐�
	void UpdateFall(void);	// �������̍X�V

	// �����o�ϐ�
	E_State m_state;	// ���
	D3DXVECTOR3 m_move;	// �ړ���
};

#endif
