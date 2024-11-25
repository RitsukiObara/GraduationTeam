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

	// �ÓI�����o�֐�
	static CUIPlayer *Create();

private:
};

#endif

