//*****************************************************
//
// �X�e�[�W�I���Ǘ�����[SelectStageManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _SELECTSTAGEMANAGER_H_
#define _SELECTSTAGEMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStageManager.h"
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	CSelectStageManager();	// �R���X�g���N�^
	~CSelectStageManager() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);	// �ǂݍ��ݏ���

	// �ÓI�����o�֐�
	static CSelectStageManager *Create(void);	// ��������

private:
	// �\���̒�`
	struct S_InfoStage
	{// �X�e�[�W���
		string pathModel;	// ���f���̃p�X
		D3DXVECTOR3 pos;	// �ʒu
	};

	// �����o�֐�
	void LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage);	// �X�e�[�W���̓Ǎ�
	void SetStage(void);	// �X�e�[�W�̐ݒu
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	vector<S_InfoStage*> m_aInfoStage;	// �X�e�[�W���̔z��

	// �ÓI�����o�ϐ�
};

#endif