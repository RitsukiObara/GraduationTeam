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

	CIceManager(int nPriority = 2);	// �R���X�g���N�^
	~CIceManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	CIce *CreateIce(int nGridV,int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// �X�̐���
	void StopIce(CIce *pIce);	// �X�̒�~
	void PeckIce(int nNumV, int nNumH, E_Direction direction);	// �X����
	void AddIce(CIce *pIce, D3DXVECTOR3 pos);	// �X�̒ǉ�
	bool FindIce(int nNumV, int nNumH, int nIdx,CIce *pIceStand,vector<CIce*> apIceLast,bool bBreak);	// �A�C�X�̔���
	vector<CIce*> GetAroundIce(int nNumV, int nNumH);	// ���ӂ̕X���擾

	// �ϐ��擾�E�ݒ�֐�
	void EnableBreakIce(bool bBreakIce) { m_bBreakIce = bBreakIce; }	// �X�j��t���O
	bool IsBreakIce(void) { return m_bBreakIce; }
	D3DXVECTOR3 GetGridPosition(int *pNumV, int *pNumH);

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
	bool JudgeBetweenPeck(int nNumV, int nNumH);	// �������X�ɋ��܂�Ă锻��
	void DisableFind(void);	// �T���t���O�̖�����
	void DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck,vector<CIce*> apIce);	// �v���C���[����M�����o���āA�j��M��������
	void DisableBreak(int nNumV, int nNumH);	// �X�̔j�������
	void BreakIce(void);	// �X�̔j��
	void CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH);	// ���ӃO���b�h�̌v�Z
	bool CheckStandBlock(vector<CIce*> apIce, CIce *pIce,int nIdx);
	bool CheckCorner(int nNumV, int nNumH);	// �p�̊m�F
	bool CheckCommon(vector<CIce*> apIce, vector<CIce*> apIceLast,CIce* pIceStand, int nNumV, int nNumH,bool bBreakLast);
	void BreakPeck(int nNumV, int nNumH);	// �˂������X�𒾂߂�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nNumGridVirtical;	// �c�O���b�h�̐�
	int m_nNumGridHorizontal;	// ���O���b�h�̐�
	vector<vector<S_Grid>> m_aGrid;	// �O���b�h�̔z��
	bool m_bBreakIce;	// �X���󂷂��̃t���O

	// �ÓI�����o�ϐ�
	static CIceManager *s_pIceManager;	// ���g�̃|�C���^
};

namespace Grid
{
const float SIZE = 200.0f;	// �O���b�h�̃T�C�Y
}

#endif

