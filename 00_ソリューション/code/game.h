//*****************************************************
//
// �Q�[������[game.h]
// Author:���R����
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"
#include "timer.h"
#include "score.h"
#include "UI_enemy.h"
#include "stageResultUI.h"
#include "pause.h"
#include "ocean_flow_UI.h"

//*****************************************************
// �O���錾
//*****************************************************
class CGame;
class CLight;
class CPause;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr int MAX_TIME = 120;	// �ő厞��

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

	CGame();	// �R���X�g���N�^
	~CGame() {};	// �f�X�g���N�^

	static CGame *GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(E_State state) { m_state = state; }
	static E_State GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }
	void ReleasePause(void) { m_pPause = nullptr; };	// �|�[�Y�̉��

	// �ϐ��擾�E�ݒ�֐�
	vector<CLight*> GetLight(void) { return m_aLight; };	// ���C�g�̔z��
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// ���C�g�̐F
	void ResetDestColLight(void);	// ���C�g�F���Z�b�g
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// ���S���W
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };
	int GetTimeSecond(void) { return m_pTimer->GetSecond(); }
	CScore* GetScore(void) { return m_pScore; };

private:
	// �����o�֐�
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void CreateLight(void);	// ���C�g�̐���
	void UpdateLight(void);	// ���C�g�̍X�V
	void UpdatePause(void);	// �|�[�Y�̍X�V

	// �����o�ϐ�
	int m_nCntState;	// ��ԑJ�ڃJ�E���^�[
	int m_nTimerCnt;	// �^�C�}�[�p�̃J�E���g
	bool m_bStop;	// ��~���
	vector<CLight*> m_aLight;	// �Q�[�����̃��C�g
	CTimer* m_pTimer;		// �^�C�}�[
	CScore* m_pScore;		// �X�R�A
	COceanFlowUI* m_pOceanFlowUI;		// �C��UI
	D3DXCOLOR m_colLight;	// ���C�g�F
	D3DXVECTOR3 m_posMid;	// �v���C���[�ƓG�̒��S���W
	CStageResultUI* m_pStageResultUI;	// �X�e�[�W�N���AUI
	CPause *m_pPause;	// �|�[�Y�̃|�C���^
	E_GameMode m_GameMode;	// �Q�[�����[�h

	// �ÓI�����o�ϐ�
	static E_State m_state;	// ���
	static CGame *m_pGame;	// ���g�̃|�C���^
};

#endif