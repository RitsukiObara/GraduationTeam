//*****************************************************
//
// �w�i�X�̏���[BG_Ice.h]
// Author:����F�M
//
//*****************************************************
#ifndef _BG_ICE_H_
#define _BG_ICE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CBgIce : public CObjectX
{
public:
	typedef enum
	{
		TYPE_BIG = 0,
		TYPE_SMALL,
		TYPE_MAX
	}TYPE;

	CBgIce();	// �R���X�g���N�^
	~CBgIce();	// �f�X�g���N�^

	static CBgIce* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	TYPE m_type;	//���f���̃^�C�v�����ϐ�
};

#endif
