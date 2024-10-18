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

	// �ϐ��擾�E�ݒ�֐�
	void SetState(E_State state) { m_state = state; }	// ���
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// �T���\�t���O
	bool IsCanFind(void) { return m_bCanFind; }
	virtual void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// �j��t���O
	bool IsBreak(void) { return m_bBreak; }
	void EnablePeck(bool bPeck) { m_bPeck = bPeck; }	// �˂����ꂽ�t���O
	bool IsPeck(void) { return m_bPeck; }
	void EnableAliveStandIce(bool bPeck) { m_bAliveStandBlock = bPeck; }	// �����Ă���X�ɒH�蒅�����t���O
	bool IsAliveStandIce(void) { return m_bAliveStandBlock; }
	void EnableSink(bool bSink) { m_bSink = bSink; }	// ���ރt���O
	bool IsSink(void) { return m_bSink; }
	CFan3D *GetFan3D(void) { return m_pUp; }	// �㑤�̐�|���S���擾

	// �ÓI�����o�֐�
	static CIce *Create(E_Type type = E_Type::TYPE_NORMAL, E_State state = E_State::STATE_FLOWS);	// ��������
	static int GetNumAll(void) { return s_nNumAll; }	// �����擾
	static std::vector<CIce*> GetInstance(void) { return m_Vector; }

private:
	// �����o�֐�
	void Flows(void);	// ����鏈��
	void FollowWave(void);	// �g�ɒǏ]���鏈��

	// �����o�ϐ�
	E_State m_state;	// ���
	bool m_bCanFind;	// �T���ł���t���O
	bool m_bBreak;	// ����t���O
	bool m_bPeck;	// �˂����ꂽ�t���O
	bool m_bAliveStandBlock;	// �����Ă���u���b�N�ɓ��B�����t���O
	bool m_bSink;	// ���ރt���O
	CFan3D *m_pUp;	// �㑤�ɓ\���|���S��
	CMeshCylinder *m_pSide;	// �T�C�h�̃V�����_�[
	CIceState *m_pState;	// �X�e�C�g�̃|�C���^

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
	CIceStaeteNormal() {};	// �R���X�g���N�^
	~CIceStaeteNormal() {};	// �f�X�g���N�^

	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
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
	
	// �����o�ϐ�
	vector<CObjectX*> m_aPeaceIce;	// �X�̔j�Ђ̃|�C���g
	float m_fTimerRepair;	// �C���^�C�}�[
};

class CIceStateFlow : public CIceState
{// �X�̗���X�e�C�g
public:
	CIceStateFlow() {};	// �R���X�g���N�^
	~CIceStateFlow() {};	// �f�X�g���N�^

	void Init(CIce *pIce) override;	// ������
	void Uninit(CIce *pIce) override;	// �I��
	void Update(CIce *pIce) override;	// �X�V

private:
	// �֐��|�C���^�^�̒�`
	typedef void (CIceStateFlow::*DirectionFunc)(void);

	// �����o�֐�
	void CollideIce(void);	// �X�Ƃ̔���
	void CheckUp(void);	// ������̊m�F
	void CheckRight(void);	// �E���̊m�F
	void CheckDown(void);	// �������̊m�F
	void CheckLeft(void);	// �����̊m�F
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

