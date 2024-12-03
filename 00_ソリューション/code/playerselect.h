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
class CMeshCylinder;
class CFan3D;
class CCollisionSphere;
class CShadow;
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerSelect : public CScene
{
public:
	enum E_StandyrState
	{// �v���C���[�̏������
		STANDBY_WAIT,
		STANDBY_OK,
		STANDBY_PLAY,
		STANDBY_MAX
	};

	CPlayerSelect();	// �R���X�g���N�^
	~CPlayerSelect() {};	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	// �����o�֐�
	void CreateMesh(void);			// ���b�V���̐���
	void CreatePlayer(int nIdx, int nIdxInput);	// �v���C���[�̐���
	void Input(void);				// ����
	void LimitPlayerPos(void);		// �v���C���[�̈ʒu����
	void GravityPlayer(void);		// �v���C���[�̏d�͏���
	void Ready(int nIdx);			// ����
	void CheckStart(void);			// �J�n���邩�̊m�F
	void StartFade(void);			// �t�F�[�h�̊J�n
	void UpdateBillboard(void);		// �v���C���[�ԍ��r���{�[�h�ʒu�X�V
	void Debug(void);				// �f�o�b�O����

	// �����o�ϐ�
	int m_nNumPlayer;	// �v���C���[�̐�
	E_StandyrState m_StandbyState[MAX_PLAYER];	// �v���C���[�̏��
	CUI *m_apPlayerUI[MAX_PLAYER];				// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^
	CUI *m_apStateUI[MAX_PLAYER];				// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^
	map<CInputManager*, CPlayer*> m_mapPlayer;	// �v���C���[�̃}�b�v�R���e�i
	CInputManager *m_apInputMgr[MAX_PLAYER];	// ���̓}�l�[�W���[�̔z��
	vector<int> m_aIdxInput;					// ���͔ԍ��ۑ��p�̔z��
	CMeshCylinder *m_pCylinder;					// ���ʂ̃V�����_�[
	CFan3D *m_pFan;								// �㑤�̉~�|���S��
	CShadow* m_pShadow;							// �e�̃|�C���^
	CPolygon3D* m_apBillboard[MAX_PLAYER];		// �v���C���[�ԍ��r���{�[�h

	// �ÓI�����o�ϐ�
};

#endif