//*****************************************************
//
// �����g��̏���[peckWave.h]
// Author:���R����
//
//*****************************************************
#ifndef _PECKWAVE_H_
#define _PECKWAVE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CPeckWave : public CPolygon3D
{
public:
	CPeckWave(int nPriority = 5);	// �R���X�g���N�^
	~CPeckWave();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPeckWave *Create(D3DXVECTOR3 pos,D3DXCOLOR col);

private:
};

#endif