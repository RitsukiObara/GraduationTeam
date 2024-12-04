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
	CUIPlayer(int nPriority = 5);	// �R���X�g���N�^
	~CUIPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void StartScatter(void);	// �U��΂�n�߂鏈��

	// �擾�E�ݒ�֐�
	CUI *GetIcon(int nIdx) { return m_aIcon[nIdx]->pPolygon; }	// �A�C�R���̎擾

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
	// �\���̒�`
	struct S_Icon
	{// �A�C�R��
		CUI *pPolygon;		// �|���S���̃|�C���^
		float fTimerFade;	// �A�C�R���̃t�F�[�h�^�C�}�[
		bool bFade;			// �t�F�[�h�t���O

		// �R���X�g���N�^
		S_Icon::S_Icon() : pPolygon(nullptr), fTimerFade(0.0f), bFade(false) {}
	};

	// �����o�֐�
	void UpdateScatter(void);	// �U��΂��Ԃ̍X�V
	void UpdateFadeIcon(void);	// �A�C�R���̃t�F�[�h

	// �����o�ϐ�
	vector<S_Icon*> m_aIcon;	// �v���C���[�A�C�R���̔z��
	E_State m_state;			// ���
	float m_fTimerScatter;		// �U��΂�^�C�}�[

	// �֐��|�C���^�^���`
	typedef void (CUIPlayer::*FuncUpdateState)(void);

	// �ÓI�����o�ϐ�
	static FuncUpdateState s_aFuncUpdateState[];	// ��ԍX�V�֐�

};

#endif

