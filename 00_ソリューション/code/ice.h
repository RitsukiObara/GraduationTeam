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
#include "polygon3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIce : public CPolygon3D
{
public:
	// �񋓌^��`
	enum E_State
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
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// �T���\�t���O
	bool IsCanFind(void) { return m_bCanFind; }
	void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// �j��t���O
	bool IsBreak(void) { return m_bBreak; }

	// �ÓI�����o�֐�
	static CIce *Create(E_State state = E_State::STATE_FLOWS);	// ��������
	static int GetNumAll(void) { return s_nNumAll; }	// �����擾

private:
	// �����o�֐�
	void Flows(void);	// ����鏈��

	// �����o�ϐ�
	E_State m_state;	// ���
	bool m_bCanFind;	// �T���ł���t���O
	bool m_bBreak;	// ����t���O

	// �ÓI�����o�ϐ�
	static int s_nNumAll;	// ����
};

#endif

