//*****************************************************
//
// �e�N�X�`���̊Ǘ�[texture.h]
// Author:���R����
//
//*****************************************************

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_TEX	(128)
#define MAX_STRING	(256)

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTexture
{
public:
	CTexture();	// �R���X�g���N�^
	~CTexture();	// �f�X�g���N�^
	
	static CTexture *Create(void);
	HRESULT Load(void);
	void Unload(void);
	int Regist(const char *pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	static int GetNumAll(void) { return m_nNumAll; }
	static CTexture *GetInstance(void) { return m_pTexture; }

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];	// �e�N�X�`���̔z��
	static int m_nNumAll;	// ����
	char *m_apFilename[MAX_TEX];	// �t�@�C�����̔z��
	static CTexture *m_pTexture;	// ���g�̃|�C���^
};

#endif