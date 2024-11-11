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
#include "main.h"
#include "scene.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PLAYER	(4)

//*****************************************************
// �O���錾
//*****************************************************
class CPlayerSelect;
class CLight;
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerSelect : public CScene
{
public:
	enum E_State
	{// ���
		STATE_NONE,		// �������Ă��Ȃ����
		STATE_START,	// �J�n���
		STATE_NORMAL,	// �ʏ��� 
		STATE_RESULT,	// ���U���g���
		STATE_END,		// �I�����
		STATE_MAX
	};
	enum E_GameMode
	{// �Q�[���̃��[�h
		MODE_NONE = 0,	// ���ł��Ȃ����[�h
		MODE_SINGLE,	// �V���O�����[�h
		MODE_MULTI,		// �}���`
		MODE_MAX
	};
	enum E_StandyrState
	{// �v���C���[�̏������
		
		STANDBY_WATE,
		STANDBY_OK,
		STANDBY_PLAY,
		STANDBY_MAX
	};

	CPlayerSelect();	// �R���X�g���N�^
	~CPlayerSelect() {};	// �f�X�g���N�^

	static CPlayerSelect* GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(E_State state) { m_state = state; }
	static E_State GetState(void) { return m_state; }

	// �ϐ��擾�E�ݒ�֐�
	vector<CLight*> GetLight(void) { return m_aLight; };	// ���C�g�̔z��
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// ���C�g�̐F
	void ResetDestColLight(void);	// ���C�g�F���Z�b�g
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// ���S���W
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };

private:
	// �����o�֐�
	void UpdateCamera(void);
	void Debug(void);
	void CreateLight(void);	// ���C�g�̐���
	void UpdateLight(void);	// ���C�g�̍X�V

	// �����o�ϐ�
	vector<CLight*> m_aLight;	// �Q�[�����̃��C�g
	D3DXCOLOR m_colLight;	// ���C�g�F
	D3DXVECTOR3 m_posMid;	// �v���C���[�ƓG�̒��S���W
	E_GameMode m_GameMode;	// �Q�[�����[�h
	int m_nCountPlayer;
	E_StandyrState m_StandbyState[MAX_PLAYER];	// �v���C���[�̏��
	CUI* m_apPlayerUI[MAX_PLAYER];	// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^
	CUI* m_apStateUI[MAX_PLAYER];	// �v���C���[�p2D�I�u�W�F�N�g�̃|�C���^

	// �ÓI�����o�ϐ�
	static E_State m_state;	// ���
	static CPlayerSelect* m_pGame;	// ���g�̃|�C���^

};

#endif