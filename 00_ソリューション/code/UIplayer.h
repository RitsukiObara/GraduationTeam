//*****************************************************
//
// �v�����[UI�̏���[UIplayer.h]
// Author:���R����
//
//*****************************************************

#ifndef _UIPLAYER_H_
#define _UIPLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIPlayer : public CObject
{
public:
	CUIPlayer(int nPriority = 2);	// �R���X�g���N�^
	~CUIPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void StartScatter(void);	// �U��΂�n�߂鏈��

	// �擾�E�ݒ�֐�
	CUI *GetIcon(int nIdx) { return m_apIconPlayer[nIdx]; }	// �A�C�R���̎擾

	// �ÓI�����o�֐�
	static CUIPlayer *Create();

private:
	// �񋓌^��`
	enum E_State 
	{// ���
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_SCATTER,	// �U��΂���
		STATE_MAX
	};

	// �����o�֐�
	void UpdateScatter(void);	// �U��΂��Ԃ̍X�V

	// �����o�ϐ�
	CUI *m_apIconPlayer[NUM_PLAYER];	// �v���C���[�A�C�R��
	E_State m_state;					// ���
	float m_fTimerScatter;				// �U��΂�^�C�}�[

	// �֐��|�C���^�^���`
	typedef void (CUIPlayer::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�

};

#endif

