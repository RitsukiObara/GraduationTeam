//*****************************************************
//
// �t�F�[�h�����i�X�������Ă���j[fade_fallice.h]
// Author:�Ό��D�n
//
//*****************************************************

#ifndef _FADE_FALLICE_H_
#define _FADE_FALLICE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fade.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CFade_FallIce : public CFade
{
public:
	CFade_FallIce();	// �R���X�g���N�^
	~CFade_FallIce();	// �f�X�g���N�^

	static CFade_FallIce* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetFade(CScene::MODE modeNext, bool bTrans = true) override;
	static CFade_FallIce* GetInstance(void) { return m_pFade; }
private:
	static CFade_FallIce* m_pFade;	// ���g�̃|�C���^
};

#endif
