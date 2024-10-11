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

//*****************************************************
// �O���錾
//*****************************************************
class CGame;
class CLight;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CGame : public CScene
{
public:
	static constexpr float MAX_TIME = 60.0f;	// �ő厞��

	enum STATE
	{// ���
		STATE_NONE,		// �������Ă��Ȃ����
		STATE_START,	// �J�n���
		STATE_NORMAL,	// �ʏ���
		STATE_RESULT,	// ���U���g���
		STATE_END,		// �I�����
		STATE_MAX
	};

	CGame();	// �R���X�g���N�^
	~CGame() {};	// �f�X�g���N�^

	static CGame *GetInstance(void) { return m_pGame; }
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static void SetState(STATE state) { m_state = state; }
	static STATE GetState(void) { return m_state; }
	void EnableStop(bool bStop) { m_bStop = bStop; }
	bool GetStop(void) { return m_bStop; }

	// �ϐ��擾�E�ݒ�֐�
	vector<CLight*> GetLight(void) { return m_aLight; };	// ���C�g�̔z��
	void SetDestColLight(D3DXCOLOR col) { m_colLight = col; }	// ���C�g�̐F
	void ResetDestColLight(void);	// ���C�g�F���Z�b�g
	D3DXVECTOR3 GetPosMid(void) { return m_posMid; };	// ���S���W
	void SetPosMid(D3DXVECTOR3 pos) { m_posMid = pos; };
	void AddPosMid(D3DXVECTOR3 pos) { m_posMid += pos; };
	float GetTimeSecond(void) { return m_pTimer->GetSecond(); }

private:
	// �����o�֐�
	void UpdateCamera(void);
	void ManageState(void);
	void ToggleStop(void);
	void Debug(void);
	void CreateLight(void);	// ���C�g�̐���
	void UpdateLight(void);	// ���C�g�̍X�V

	// �����o�ϐ�
	int m_nCntState;	// ��ԑJ�ڃJ�E���^�[
	bool m_bStop;	// ��~���
	vector<CLight*> m_aLight;	// �Q�[�����̃��C�g
	CTimer* m_pTimer;		// �^�C�}�[
	//CScore* m_pScore;		// �X�R�A
	D3DXCOLOR m_colLight;	// ���C�g�F
	D3DXVECTOR3 m_posMid;	// �v���C���[�ƓG�̒��S���W

	// �ÓI�����o�ϐ�
	static STATE m_state;	// ���
	static CGame *m_pGame;	// ���g�̃|�C���^
};

#endif