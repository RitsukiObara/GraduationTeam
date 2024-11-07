//*****************************************************
//
// ���[�h�I������[selectmode.h]
// Author:�Ό��D�n
//
//*****************************************************
#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectMode : public CScene
{
public:
	enum MODE
	{
		MODE_SINGLE = 0,
		MODE_PARTY,
		//MODE_OPTION,
		MODE_MAX
	};

	CSelectMode();	// �R���X�g���N�^
	~CSelectMode();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// �����o�֐�
	void ChangeSelectMode(int move);	// ���[�h�ړ�����

	// �����o�ϐ�
	CUI *m_apModeUI[MODE_MAX];	// ���[�hUI
	CUI *m_pManual;				// �������
	float m_fCurTime;			// ���݂̑ҋ@����
	MODE m_selectMode;			// �I�����Ă��郂�[�h
};

#endif