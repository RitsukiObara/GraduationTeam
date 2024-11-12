//*****************************************************
//
// �Q�[������[playerselect.h]
// Author:�x�씋��
//
//*****************************************************
#ifndef _PLAYERSELECT_H_
#define _PLAYERSELECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PLAYER	(4)

//*****************************************************
// �O���錾
//*****************************************************
class CPlayerSelect;
class CUI;
class CPlayer;
class CInputManager;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerSelect : public CScene
{
public:
	enum E_StandyrState
	{// �v���C���[�̏������
		STANDBY_WATE,
		STANDBY_OK,
		STANDBY_PLAY,
		STANDBY_MAX
	};

	CPlayerSelect();	// �R���X�g���N�^
	~CPlayerSelect() {};	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

	// �ϐ��擾�E�ݒ�֐�

private:
	// �����o�֐�
	void Input(void);	// ����
	void CreatePlayer(int nIdx);	// �v���C���[�̐���
	void Ready(int nIdx);	// ����
	void CheckStart(void);	// �J�n���邩�̊m�F
	void StartFade(void);	// �t�F�[�h�̊J�n
	void Debug(void);

	// �����o�ϐ�
	int m_nNumPlayer;	// �v���C���[�̐�
	E_StandyrState m_StandbyState[MAX_PLAYER];	// �v���C���[�̏��
	CUI *m_apPlayerUI[MAX_PLAYER];	// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^
	CUI *m_apStateUI[MAX_PLAYER];	// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^
	CPlayer *m_apPlayer[MAX_PLAYER];// �v���C���[�̃|�C���^
	CInputManager *m_apInputMgr[MAX_PLAYER];	// ���̓}�l�[�W���[�̔z��

	// �ÓI�����o�ϐ�
};

#endif