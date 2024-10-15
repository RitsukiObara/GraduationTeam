//*****************************************************
//
// �G�̏���[enemy.h]
// Author:����F�M
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CMotion
{
public:
	CEnemy(int nPriority = 4);	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CEnemy* Create(void);
	static std::vector<CEnemy*> GetInstance(void) { return m_Vector; }

private:
	// �����o�֐�
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�

	// �ÓI�����o�ϐ�
	static std::vector<CEnemy*> m_Vector;	// ���g�̃|�C���^
};

#endif