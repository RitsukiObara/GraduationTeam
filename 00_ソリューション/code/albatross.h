//*****************************************************
//
// �A�z�E�h���̏���[albatross.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _ALBATROSS_H_
#define _ALBATROSS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "iceManager.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAlbatross : public CMotion
{
public:
	CAlbatross(int nPriority = 4);	// �R���X�g���N�^
	~CAlbatross();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ManageMotion(void);	// ���[�V�����Ǘ�

	// �ÓI�����o�֐�
	static CAlbatross* Create(CIceManager::E_Stream dir);

private:
	enum E_Motion
	{// ���[�V����
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_FLY,		// ���
		MOTION_MAX
	};

	void Stream(CIceManager::E_Stream dir);

	D3DXVECTOR3 m_Move; // �ړ���
};

#endif
