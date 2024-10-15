//*****************************************************
//
// �I�[�V�����̏���[ocean.h]
// Author:����F�M
//
//*****************************************************

#ifndef _OCEAN_H_
#define _OCEAN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshfield.h"

//*****************************************************
// �N���X��`
//*****************************************************
class COcean : public CMeshField
{
public:
	COcean();	// �R���X�g���N�^
	~COcean();	// �f�X�g���N�^

	static COcean* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	float m_fRot;
};
#endif