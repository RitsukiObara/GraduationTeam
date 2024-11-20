//*****************************************************
//
// �e�̏���[shadow.h]
// Author:���R����
//
//*****************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CShadow : public CPolygon3D
{
public:
	CShadow(int nPriority = 4);	// �R���X�g���N�^
	~CShadow() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu�̐ݒ�

	// �ÓI�����o�֐�
	static CShadow *Create(void);	// ��������

private:
	// �����o�֐�
	bool CollideIce(void);		// �X�Ƃ̔���
	void CollideOcean(void);	// �C�Ƃ̔���
};

#endif