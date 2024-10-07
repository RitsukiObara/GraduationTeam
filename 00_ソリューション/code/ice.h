//*****************************************************
//
// �X�̏���[ice.h]
// Author:���R����
//
//*****************************************************

#ifndef _ICE_H_
#define _ICE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIce : public CObjectX
{
public:
	// �񋓌^��`
	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_STOP,	// ��~���
		STATE_FLOWS,	// ����Ă���
		STATE_MAX
	};

	CIce(int nPriority = 2);	// �R���X�g���N�^
	~CIce();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�
	static CIce *Create(STATE state = STATE::STATE_FLOWS);

private:
	// �����o�֐�
	void Flows(void);	// ����鏈��

	// �����o�ϐ�
	STATE m_state;	// ���
};

#endif

