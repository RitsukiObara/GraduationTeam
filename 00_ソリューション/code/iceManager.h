//*****************************************************
//
// �X�}�l�[�W���[[iceManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _ICEMANAGER_H_
#define _ICEMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "ice.h"

//*****************************************************
// �O���錾
//*****************************************************
class CIce;	// �X
class CFlowIce;	// ���X�V�X�e��

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIceManager : public CObject
{
public:
	// �񋓌^��`
	enum E_Direction
	{// ����
		DIRECTION_RIGHTUP = 0,	// �E��
		DIRECTION_RIGHT,	// �E
		DIRECTION_RIGHTDOWN,	// �E��
		DIRECTION_LEFTDOWN,	// ����
		DIRECTION_LEFT,	// ��
		DIRECTION_LEFTUP,	// ����
		DIRECTION_MAX
	};
	enum E_Stream
	{// ��������
		STREAM_UP,	// ��
		STREAM_RIGHT,	// �E
		STREAM_DOWN,	// ��
		STREAM_LEFT,	// ��
		STREAM_MAX
	};

	CIceManager(int nPriority = 2);	// �R���X�g���N�^
	~CIceManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	CIce *CreateIce(int nGridV, int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// �X�̐���
	CIce *CreateFlowIce(int nGridV,int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// ���X�̐���
	void StopIce(CIce *pIce);	// �X�̒�~
	bool PeckIce(int nNumV, int nNumH, float fRot,D3DXVECTOR3 pos);	// �X����
	void AddIce(CIce *pIce, D3DXVECTOR3 pos);	// �X�̒ǉ�
	bool FindIce(int nNumV, int nNumH, int nIdx,CIce *pIceStand,vector<CIce*> apIceLast,bool bBreak);	// �A�C�X�̔���
	vector<CIce*> GetAroundIce(int nNumV, int nNumH);	// ���ӂ̕X���擾
	void DeleteIce(CIce *pIce);	// �w�肵���|�C���^�Ɠ����X���폜����
	void Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH);	// �O�ɏo���Ȃ����菈��
	void Collide(D3DXVECTOR3 *pPos, CIce *pice);	// �O�ɏo���Ȃ����菈��
	bool GetIdxGridFromPosition(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH,float fRate = 0.7f);	// �O���b�h�ԍ����ʒu����擾���鏈��
	bool IsInIce(D3DXVECTOR3 pos, CIce *pIce, float fRate = 0.7f);	// �X�ɏ���Ă邩�̔���
	bool SetIceInGrid(int nNumV, int nNumH, CIce *pIce);	// �O���b�h�ɕX��ݒ�
	void GetIceIndex(CIce *pIce, int *pNumV, int *pNumH);	// �X�̃O���b�h�ԍ����擾
	CIce* GetLeftDownIdx(int *pNumV, int *pNumH);	// �E���̕X�擾
	void DisableFind(void);	// �T���t���O�̖�����

	// �ϐ��擾�E�ݒ�֐�
	D3DXVECTOR3 GetGridPosition(int *pNumV, int *pNumH);
	CIce* GetGridIce(int* pNumV, int* pNumH);
	int GetNumGridV(void) { return m_nNumGridVirtical; }	// �c�̃O���b�h��
	int GetNumGridH(void) { return m_nNumGridHorizontal; }	// ���̃O���b�h��
	float GetOceanLevel(void) { return m_fOceanLevel; }	// �C�����x���̎擾
	E_Stream GetDirStream(void) { return m_dirStream; }	// �C���̕����擾
	E_Stream GetDirStreamNext(void) { return m_dirStreamNext; }	// ���̊C���̕����擾
	vector<CIce*> GetFlows(void);	// ����Ă�X�̎擾

	void SetDirStream(E_Stream direction) { m_dirStream = direction; }	// �C���̕���
	void SetDirStreamNext(E_Stream directionNext) { m_dirStreamNext = directionNext; }	// ���̊C���̕���

	// �ÓI�����o�֐�
	static CIceManager *Create(int nNumV, int nNumH);
	static CIceManager *GetInstance() { return s_pIceManager; }	// �C���X�^���X�̎擾

private:
	// �񋓌^��`
	enum E_StateGrid
	{// �O���b�h�̏��
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_MID,	// �X�̐^�񒆂�������
		STATE_CORNER,	// �X�̊p��������
		STATE_MAX
	};
	// �\���̒�`
	struct S_Grid
	{// �O���b�h���
		D3DXVECTOR3 pos;	// �ʒu
		E_StateGrid state;	// ���
		CIce *pIce;	// �X�̃|�C���^
	};
	
	// �����o�֐�
	void SetGridPos(void);	// �O���b�h�̈ʒu���v�Z
	void ManageStateIce(void);	// �X�̏�ԊǗ�
	void SearchInvailStopIce(void);	// �����Ȓ�~�X�̌��o
	void SearchNotConnectIce(vector<CIce*> &rpIce);	// �q�����ĂȂ��X�̌��o
	bool JudgeBetweenPeck(int nNumV, int nNumH);	// �������X�ɋ��܂�Ă锻��
	bool CanPeck(CIce* pIce,int nNumV, int nNumH);	// ������X���̃`�F�b�N
	void DisableFromHardIce(int nNumV, int nNumH,bool bPeck = true);	// �d���X����M�����o���āA�j��M��������
	void DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck,vector<CIce*> apIce);	// �v���C���[����M�����o���āA�j��M��������
	void DisableBreak(int nNumV, int nNumH);	// �X�̔j�������
	void SummarizeIce(int nNumV, int nNumH);	// �X���܂Ƃ߂鏈��
	void SaveFlowIce(int nNumV, int nNumH, CFlowIce *pFlowIce);	// ���X�ɂ܂Ƃ߂鏈��
	void BreakIce(void);	// �X�̔j��
	bool CheckCorner(int nNumV, int nNumH);	// �p�̊m�F
	void BreakPeck(int nNumV, int nNumH);	// �˂������X�𒾂߂�
	void LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH);	// �X���Ɏ��߂鏈��
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nNumGridVirtical;	// �c�O���b�h�̐�
	int m_nNumGridHorizontal;	// ���O���b�h�̐�
	float m_fOceanLevel;	//	�C���̋���
	vector<vector<S_Grid>> m_aGrid;	// �O���b�h�̔z��
	E_Stream m_dirStream;	// �C���̕���
	E_Stream m_dirStreamNext;	// ���̊C���̕���

	// �ÓI�����o�ϐ�
	static CIceManager *s_pIceManager;	// ���g�̃|�C���^
};

namespace Grid
{
const float SIZE = 200.0f;	// �O���b�h�̃T�C�Y
inline void CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH)
{
	// �E��E�E��
	aV[CIceManager::DIRECTION_RIGHTUP] = nNumV + 1;
	aV[CIceManager::DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0) {  // �����̎�
		aH[CIceManager::DIRECTION_RIGHTUP] = nNumH + 1;
		aH[CIceManager::DIRECTION_RIGHTDOWN] = nNumH + 1;
		aH[CIceManager::DIRECTION_LEFTUP] = nNumH;
		aH[CIceManager::DIRECTION_LEFTDOWN] = nNumH;
	}
	else {  // ��̎�
		aH[CIceManager::DIRECTION_RIGHTUP] = nNumH;
		aH[CIceManager::DIRECTION_RIGHTDOWN] = nNumH;
		aH[CIceManager::DIRECTION_LEFTUP] = nNumH - 1;
		aH[CIceManager::DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// �����E���E
	aV[CIceManager::DIRECTION_LEFTUP] = nNumV + 1;
	aV[CIceManager::DIRECTION_LEFTDOWN] = nNumV - 1;
	aV[CIceManager::DIRECTION_RIGHT] = nNumV;
	aV[CIceManager::DIRECTION_LEFT] = nNumV;
	aH[CIceManager::DIRECTION_RIGHT] = nNumH + 1;
	aH[CIceManager::DIRECTION_LEFT] = nNumH - 1;
}
}

namespace stream
{
const D3DXVECTOR3 VECTOR_STREAM[CIceManager::E_Stream::STREAM_MAX] =
{
	{ 0.0f, 0.0f, 1.0f },   // ��
	{ 1.0f, 0.0f, 0.0f },   // �E
	{ 0.0f, 0.0f, -1.0f }, // ��
	{ -1.0f, 0.0f, 0.0f },  // ��
};
}

#endif

