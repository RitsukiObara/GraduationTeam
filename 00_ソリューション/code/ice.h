//*****************************************************
//
// �X�̏���[ice.h]
// Author:���R����
//
//*****************************************************
#ifndef _ICE_H_
#define _ICE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"
#include "ocean.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;
class CFan3D;
class CIceState;
class CObjectX;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIce : public CGameObject
{
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_FLOWS,	// ����Ă���
		STATE_MAX
	};
	enum E_Type
	{// ���
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_HARD,	// �d���X
		TYPE_MAX
	};
	enum E_TypeShake
	{// �h��̎��
		SHAKE_NONE = 0,	// �������Ă��Ȃ����
		SHAKE_SINK_NORMAL,	// �ʏ�̒���
		SHAKE_SINK_BIG,	// �傫�Ȓ���
		SHAKE_RETURN,	// �ʏ�߂�
		SHAKE_RETURN_BIG,	// �傫�Ȗ߂�
		SHAKE_MAX
	};

	CIce(int nPriority = 2);	// �R���X�g���N�^
	~CIce();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateMesh(void);	// ���b�V���̐���
	void DeleteMesh(void);	// ���b�V���̔j��
	void SetTransform(float fRadius);	// �g�����X�t�H�[���ݒ�
	virtual bool IsCanPeck(void) { return true; };	// �˂����邩�̔����Ԃ�
	void ChangeState(CIceState *pState);
	bool IsOnTopAnyObject(void);	// �������炪����Ă锻��
	void SetColor(D3DXCOLOR col);	// �F�̐ݒ�
	void StartFlash(void);			// ���鏈���̊J�n

	// �ϐ��擾�E�ݒ�֐�
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// �T���\�t���O
	bool IsCanFind(void) { return m_bCanFind; }
	virtual void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// �j��t���O
	bool IsBreak(void) { return m_bBreak; }
	void EnablePeck(bool bPeck) { m_bPeck = bPeck; }	// �˂����ꂽ�t���O
	bool IsPeck(void) { return m_bPeck; }
	void EnableSink(bool bSink) { m_bSink = bSink; }	// ���ރt���O
	bool IsSink(void) { return m_bSink; }
	void EnableStop(bool bStop) { m_bStop = bStop; }	// �~�܂�t���O
	bool IsStop(void) { return m_bStop; }
	CFan3D *GetFan3D(void) { return m_pUp; }	// �㑤�̐�|���S���擾
	void SetShake(E_TypeShake shake) { m_shake = shake; }	// �h��
	E_TypeShake GetShake(void) { return m_shake; }
	void SetRippleFrag(COcean::E_Stream stream, bool bValue) { m_abRipleFrag[stream] = bValue; }	// ���ꂲ�Ƃ̃t���O�擾
	bool GetRippleFrag(COcean::E_Stream stream) { return m_abRipleFrag[stream]; }

	// �ÓI�����o�֐�
	static CIce *Create(E_Type type = E_Type::TYPE_NORMAL, E_State state = E_State::STATE_FLOWS);	// ��������
	static int GetNumAll(void) { return s_nNumAll; }	// �����擾
	static std::vector<CIce*> GetInstance(void) { return m_Vector; }

private:
	// �����o�֐�
	void FollowWave(void);								// �g�ɒǏ]���鏈��
	void SearchOnThis(void);							// ���g�ɏ���Ă���̂̌��o
	void GetOnTopObject(vector<CGameObject*> &rVector);	// ��ɏ���Ă���̂̌��o
	void Shake(void);									// �h��̏���
	void Ripples(void);									// �����g�̏���

	void Flash(void);		// �����ƌ��鏈��

	// �����o�ϐ�
	E_State m_state;	// ���
	bool m_bCanFind;	// �T���ł���t���O
	bool m_bBreak;		// ����t���O
	bool m_bPeck;		// �˂����ꂽ�t���O
	bool m_bSink;		// ���ރt���O
	bool m_bStop;		// ��~���Ă��邩�ǂ���

	float m_fHeightFromOcean;		// �C����̍���
	float m_fHeightDestFromOcean;	// �C����̖ڕW����

	E_TypeShake m_shake;		// �h��̃^�C�v
	float m_fTimerReturnShake;	// �h�ꂩ��߂�^�C�}�[
	
	CFan3D *m_pUp;			// �㑤�ɓ\���|���S��
	CMeshCylinder *m_pSide;	// �T�C�h�̃V�����_�[

	CIceState *m_pState;	// �X�e�C�g�̃|�C���^

	bool m_abRipleFrag[COcean::E_Stream::STREAM_MAX];	// ���ꂲ�Ƃ̃t���O
	float m_fTimerRipples;								// �����g�̃^�C�}�[
	float m_fSpawnTimeRipples;							// �����g�̐�������

	float m_fTimerFlash;		// ���̃^�C�}�[
	float m_fTimeStartFlash;	// ���̊J�n�܂ł̎���
	int m_nCntAnimFlash;		// ���̃A�j���[�V�����J�E���^�[
	int m_nPaternAnim;			// �A�j���[�V�����p�^�[��

	// �ÓI�����o�ϐ�
	static int s_nNumAll;	// ����
	static std::vector<CIce*> m_Vector;	// ���g�̃|�C���^
};

class CIceState
{// �X�̊��X�e�C�g
public:
	CIceState() {};	// �R���X�g���N�^
	~CIceState() {};	// �f�X�g���N�^

	virtual void Init(CIce *pIce) = 0;	// ������
	virtual void Uninit(CIce *pIce) = 0;	// �I��
	virtual void Update(CIce *pIce) = 0;	// �X�V

private:
};

class CIceStaeteNormal : public CIceState
{// �X�̒ʏ�X�e�C�g
public:
	CIceStaeteNormal() : m_nIdxDriftV(0), m_nIdxDriftH(0) {};	// �R���X�g���N�^
	~CIceStaeteNormal() {};	// �f�X�g���N�^

	// �����o�֐�
	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
	// �����o�֐�
	bool CheckFailGetIndex(CIce *pIce);	// �ԍ��擾�Ɏ��s���Ă��邩�̊m�F
	void MoveToGrid(CIce *pIce);	// �O���b�h�̈ʒu�Ɍ������Ĉړ����鏈��
	void MoveObjectOnIce(D3DXVECTOR3 vecMove, CIce *pIce);	// ����Ă�I�u�W�F�N�g�𓮂���

	// �����o�ϐ�
	int m_nIdxDriftV;	// �Y�����Ă�O���b�h�̏c�ԍ�
	int m_nIdxDriftH;	// �Y�����Ă�O���b�h�̉��ԍ�
};

class CIceStaeteBreak : public CIceState
{// �X�̕���X�e�C�g
public:
	CIceStaeteBreak() : m_fTimerRepair(0.0f) {};	// �R���X�g���N�^
	~CIceStaeteBreak() {};	// �f�X�g���N�^

	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
	// �萔��`
	static const int NUM_ICE_BREAK = 5;	// ��ꂽ�X�̐�
	
	// �����o�֐�
	void UpdateRepair(CIce *pIce);	// �C������X�V
	void FollowIce(CIce *pIce);	// �X�̒Ǐ]

	// �����o�ϐ�
	vector<CObjectX*> m_aPeaceIce;	// �X�̔j�Ђ̃|�C���g
	float m_fTimerRepair;	// �C���^�C�}�[
};

class CIceStateFlow : public CIceState
{// �X�̗���X�e�C�g
public:
	CIceStateFlow() : m_bDrift(false), m_nIdxDriftV(0), m_nIdxDriftH(0) {};	// �R���X�g���N�^
	~CIceStateFlow() {};	// �f�X�g���N�^

	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
	// �֐��|�C���^�^�̒�`
	typedef bool (CIceStateFlow::*DirectionFunc)(CIce *pIce,int nIdxV,int nIdxH, vector<CIce*> &rpHitIce);

	// �����o�֐�
	void UpdateSearchIce(CIce *pIce);	// �X��T���Ă鎞�̍X�V
	void UpdateDriftIce(CIce *pIce);	// �Y�����鎞�̍X�V
	void CollideIce(CIce *pIce);	// �X�Ƃ̔���
	bool CheckUp(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// ������̊m�F
	bool CheckRight(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// �E���̊m�F
	bool CheckDown(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// �������̊m�F
	bool CheckLeft(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce);	// �����̊m�F

	// �����o�[�ϐ�
	int m_bDrift;	// �Y�����Ă���t���O
	int m_nIdxDriftV;	// �Y������O���b�h�̏c�ԍ�
	int m_nIdxDriftH;	// �Y������O���b�h�̉��ԍ�
};

class CIceStaeteSink : public CIceState
{// �X�̒��ރX�e�C�g
public:
	CIceStaeteSink() {};	// �R���X�g���N�^
	~CIceStaeteSink() {};	// �f�X�g���N�^

	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
};

#endif

