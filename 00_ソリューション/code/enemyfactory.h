//*****************************************************
//
// �G�t�@�N�g���[[enemyfactory.h]
// Author:���R����
//
//*****************************************************
#ifndef _ENEMYFCT_H_
#define _ENEMYFCT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyFct : public CObject
{
public:
	CEnemyFct(int nPriority = 2);	// �R���X�g���N�^
	~CEnemyFct();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(string path);	// �Ǎ�
	int GetNumEnemyInfo(void) { return (int)m_apInfoEnemy.size(); }	// �G���擾

	// �ÓI�����o�֐�
	static CEnemyFct *Create(void);	// ��������
private:
	// �\���̒�`
	struct S_InfoEnemy
	{// �G���
		float fDelaySpawn;	// �X�|�[���f�B���C
		int nType;			// ���
		int nPaternSpawn;	// �X�|�[���p�^�[��

		S_InfoEnemy::S_InfoEnemy() : fDelaySpawn(0.0f), nType(0), nPaternSpawn(0) {}
	};

	// �����o�֐�
	void LoadInfo(std::ifstream& file, string str,S_InfoEnemy *pInfoEnemy);	// ���̓Ǎ�
	void SetEnemy(void);	// �G�̐ݒ�

	// �����o�ϐ�
	float m_fTimerSpawn;				// �X�|�[���^�C�}�[
	vector<S_InfoEnemy*> m_apInfoEnemy;	// �G�̏��z��
};

#endif