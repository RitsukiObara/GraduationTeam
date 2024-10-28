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
	// �񋓌^��`
	typedef enum
	{// ���
		TYPE_SEALS = 0,
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = 4);	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void TranslateByGrid(int nIdxV,int nIdxH);	// �O���b�h�ɂ��ړ�
	void FollowIce(void);	// �X�ɒǏ]

	// �ϐ��擾�E�ݒ�֐�
	void SetGridV(int nValue) { m_nGridV = nValue; }	// �O���b�h�̏c�ԍ�
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }	// �O���b�h�̉��ԍ�
	int GetGridVH(void) { return m_nGridH; }

	// �ÓI�����o�֐�
	static CEnemy* Create(int nType);	// ��������
	static std::vector<CEnemy*> GetArray(void) { return s_vector; }	// �z��̎擾

private:
	// �����o�֐�
	void InitGridIdx(void);	// �O���b�h�ԍ��̏�����
	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	int m_nGridV;	// ������O���b�h�̏c�ԍ�
	int m_nGridH;	// ������O���b�h�̉��ԍ�

	// �ÓI�����o�ϐ�
	static std::vector<CEnemy*> s_vector;	// ���g�̃|�C���^
};

#endif