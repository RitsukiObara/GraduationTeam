//*****************************************************
//
// ���X�̏���[flowIce.h]
// Author:���R����
//
//*****************************************************
#ifndef _ICESPEEDMANAGER_H_
#define _ICESPEEDMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CIce;

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_ICEFLOW	(40)	// ����Ă���X�̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CIceSpeedManager : public CObject
{
public:
	CIceSpeedManager(int nPriority = 2);	// �R���X�g���N�^
	~CIceSpeedManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Load(char* pPath);

	// �ϐ��ݒ�E�擾����
	void AddIceToArray(CIce* pIce);

	// �ÓI�����o�֐�
	static CIceSpeedManager* Create(void);	// ��������

private:
	// �����o�֐�
	void CreateIce(void);	// �X�̐���
	void CheckSomeIceStop(void);	// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
	void StopAllIce(void);	// �S�Ă̕X���~�߂�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	vector<CIce*> m_apIce;	// �X�̔z��
};

#endif