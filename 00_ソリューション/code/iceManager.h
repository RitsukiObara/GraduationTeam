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
// �N���X�̒�`
//*****************************************************
class CIceManager : public CObject
{
public:
	CIceManager(int nPriority = 2);	// �R���X�g���N�^
	~CIceManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ��擾�E�ݒ�֐�

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
	struct Grid
	{// �O���b�h���
		E_StateGrid state;	// ���
	};
	
	// �����o�֐�

	// �����o�ϐ�
	int m_nNumGridVirtical;	// �c�O���b�h�̐�
	int m_nNumGridHorizontal;	// ���O���b�h�̐�
	vector<vector<Grid>> m_aGrid;

	// �ÓI�����o�ϐ�
	static CIceManager *s_pIceManager;	// ���g�̃|�C���^
};

#endif

