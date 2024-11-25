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

	// �擾�E�ݒ�֐�
	CUI *GetIcon(int nIdx) { return m_apIconPlayer[nIdx]; }	// �A�C�R���̎擾

	// �ÓI�����o�֐�
	static CUIPlayer *Create();

private:
	CUI *m_apIconPlayer[NUM_PLAYER];	// �v���C���[�A�C�R��
};

#endif

