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
		DIRECTION_UP = 0,	// ��
		DIRECTION_RIGHT,	// �E
		DIRECTION_DOWN,	// ��
		DIRECTION_LEFT,	// ��
		DIRECTION_MAX
	};

	CIceManager(int nPriority = 2);	// �R���X�g���N�^
	~CIceManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	CIce *CreateIce(int nGridV,int nGridH);	// �X�̐���

	// �ϐ��擾�E�ݒ�֐�
	void StopIce(CIce *pIce);	// �X�̒�~
	void PeckIce(D3DXVECTOR3 pos, E_Direction direction);	// �X����
	void AddIce(CIce *pIce, D3DXVECTOR3 pos);	// �X�̒ǉ�
	void FindIce(int nNumV,int nNumH);	// �A�C�X�̔���

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
		E_StateGrid state;	// ���
		CIce *pIce;	// �X�̃|�C���^
	};
	
	// �����o�֐�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nNumGridVirtical;	// �c�O���b�h�̐�
	int m_nNumGridHorizontal;	// ���O���b�h�̐�
	vector<vector<S_Grid>> m_aGrid;

	// �ÓI�����o�ϐ�
	static CIceManager *s_pIceManager;	// ���g�̃|�C���^
};

namespace Grid
{
const float SIZE = 200.0f;	// �O���b�h�̃T�C�Y
}

#endif

