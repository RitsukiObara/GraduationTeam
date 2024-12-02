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
#include "iceManager.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CBgIce : public CObjectX
{
public:
	CBgIce();	// �R���X�g���N�^
	~CBgIce();	// �f�X�g���N�^

	static CBgIce* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Load(char* pPath);

	void Move(void);
	void Flow(void);

private:
	// �����o�֐�
	void RippleCheck(void);

	//�w�i�X�̏��
	typedef enum
	{
		STATE_FLOW = 0,
		STATE_SINK,
		STATE_MAX
	}STATE;

	STATE m_state;			// ��ԕ����ϐ�
	float fgravity_speed;	// ����ł������x;
	float m_fspeed;			// ���ݎn�߂鑬�x
	int m_nRippleCount;		// �g��o���J�E���^
	bool m_binscrean;		// ��ʓ��ɓ��������`�F�b�N����ϐ�
};

//*****************************************************
// �萔��`
//*****************************************************
namespace bgice
{
	const int BGICE_MODEL = 3;	// �w�i�̕X����
	const int RIPPLE_DEFAULT = 150;	// �g��o���̊�{����
	const int RIPPLE_DEGREE = 90;	// �g��o�����Ԃ̂Ԃꕝ
}

#endif
