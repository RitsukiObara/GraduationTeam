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
#include <vector>

class CPolygon2D;

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
	void ChangeOBRTexture(void);
private:
	int m_nCounterFrame;								// ���̗����J�E���^
	int m_nCounterPattern;								// �����Ƃ��p�^�[��
	std::vector<LPDIRECT3DVERTEXBUFFER9> m_icePolygon;	// �X�̃|���S��
	LPDIRECT3DTEXTURE9 m_pTexture;						// �X�̃e�N�X�`��
	static CFade_FallIce* m_pFade;						// ���g�̃|�C���^
};

#endif
