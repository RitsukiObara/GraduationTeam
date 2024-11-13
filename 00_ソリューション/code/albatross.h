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
	// �񋓌^��`
	enum E_FlyDirection
	{// ��������
		FLYDIRECTION_UP,	// ��
		FLYDIRECTION_RIGHT,	// �E
		FLYDIRECTION_DOWN,	// ��
		FLYDIRECTION_LEFT,	// ��
		FLYDIRECTION_MAX
	};

	CAlbatross(int nPriority = 4);	// �R���X�g���N�^
	~CAlbatross();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void ManageMotion(void);	// ���[�V�����Ǘ�

	// �ÓI�����o�֐�
	static CAlbatross* Create(void);

private:
	enum E_Motion
	{// ���[�V����
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_FLY,		// ���
		MOTION_MAX
	};

	// �����o�֐�
	int m_DirStream;
	int m_DirStreamNext;
	E_FlyDirection m_FlyDirection;	// ��ԕ���
};

#endif
