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
	// �񋓌^��`
	enum E_Stream
	{// ��������
		STREAM_UP,	// ��
		STREAM_RIGHT,	// �E
		STREAM_DOWN,	// ��
		STREAM_LEFT,	// ��
		STREAM_MAX
	};

	COcean(int nPriority = 3);	// �R���X�g���N�^
	~COcean();	// �f�X�g���N�^

	static COcean* Create(void);
	static COcean* GetInstance(void) { return m_pOcean; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�
	E_Stream GetNowDirStream(void) { return (COcean::E_Stream)m_nRandKeep; }
	E_Stream GetNextDirStream(void) { return (COcean::E_Stream)m_nRandNextKeep; }

	void SetNextOceanRot(void);	// ���̊C���̌����ݒ�

private:

	// �����o�֐�
	void BgIceRotState(void); // �C���̌����ƃ��b�V���̌�����A�������鏈��
	void OceanCycleTimer(void); // �C�����������ԂŊǗ����鏈��
	void OceanChangeCheck(void);	// �C���̌����ύX���Ԃ��m�F����
	void BgIceSetPosUp(void);	// ���������o�Ă���w�i�X
	void BgIceSetPosDown(void);	// ����������o�Ă���w�i�X
	void BgIceSetPosL(void);	// ����������o�Ă���w�i�X
	void BgIceSetPosR(void);	// �E��������o�Ă���w�i�X
	float m_fSpeed;	// �C���̃X�s�[�h
	D3DXVECTOR3 m_fRot;	// �C���̌���
	static COcean* m_pOcean;	// ���g�̃|�C���^
	int m_nRandKeep;	// �����_���֐��̕ۑ��p
	int m_nRandNextKeep;	// ���̃����_���֐��̕ۑ��p
	float m_fProgressTime;	// �o�ߎ���
	float m_fRotChangeTime;	// �����ύX������܂ł̎���
	bool m_bRandState;	// �����_���ϐ��̏��
	int m_nBgiceCnt;	// �w�i�̕X���Ăяo�����J�E���g(��)
};
#endif