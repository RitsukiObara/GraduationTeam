//*****************************************************
//
// �I�[�V�����̏���[ocean.h]
// Author:����F�M�A��؈�^
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
	static COcean* GetInstance(void) { return m_pOcean; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	void OceanRotState(void); // �C���̌����ƃ��b�V���̌�����A�������鏈��
	void OceanCycleTimer(void); // �C�����������ԂŊǗ����鏈��

	float m_fSpeed;	// �C���̃X�s�[�h
	D3DXVECTOR3 m_fRot;	// �C���̌���
	static COcean* m_pOcean;	// ���g�̃|�C���^

};
#endif