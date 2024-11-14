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
#include "iceManager.h"

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

	// �ϐ��擾�E�ݒ�֐�
	CIceManager::E_Stream GetNowDirStream(void) { return (CIceManager::E_Stream)m_nRandKeep; }
	CIceManager::E_Stream GetNextDirStream(void) { return (CIceManager::E_Stream)m_nRandNextKeep; }

	void SetNextOceanRot(void);	// ���̊C���̌����ݒ�

private:

	// �����o�֐�
	void OceanRotState(void); // �C���̌����ƃ��b�V���̌�����A�������鏈��
	void OceanCycleTimer(void); // �C�����������ԂŊǗ����鏈��
	void OceanChangeCheck(void);	// �C���̌����ύX���Ԃ��m�F����

	float m_fSpeed;	// �C���̃X�s�[�h
	D3DXVECTOR3 m_fRot;	// �C���̌���
	static COcean* m_pOcean;	// ���g�̃|�C���^
	int m_nRandKeep;	// �����_���֐��̕ۑ��p
	int m_nRandNextKeep;	// ���̃����_���֐��̕ۑ��p
	int m_nSetRotTime;	// �����ύX���������̃^�C�}�[����
	int m_nExecRotChangeTime;	// �����ύX������܂ł̃^�C�}�[����
	bool m_bRandState;	// �����_���ϐ��̏��
	bool m_bUse;	// �A�z�E�h�����g���Ă��邩
};
#endif