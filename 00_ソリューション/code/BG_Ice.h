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

	//�w�i�X�̎��
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
	static void Load(char* pPath);

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
	void Move(void);
	void Flow(void);

private:

	//�w�i�X�̏��
	typedef enum
	{
		STATE_FLOW = 0,
		STATE_SINK,
		STATE_MAX
	}STATE;

	TYPE m_type;	// �^�C�v�����ϐ�
	STATE m_state;	// ��ԕ����ϐ�
	COcean::E_Stream m_streamOld;	// �C�����
	float fgravity_speed;	//	����ł������x;
	int m_nNumAll;	// ����
};

#endif
