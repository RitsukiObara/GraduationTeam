//*****************************************************
//
// �G��|�������̃X�R�A����[destroy_score.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _DESTROY_SCORE_H_
#define _DESTROY_SCORE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"
#include "number.h"
#include "score.h"
#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CDestroyScore : public CScore
{
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_BESIDE = 0,	// ���ړ�
		STATE_VERTICAL,	// �c�ړ�
		STATE_WAIT,	// �ؗ����
		STATE_ADD,	// �X�R�A�ɉ��Z
		STATE_MAX
	};

	CDestroyScore();	// �R���X�g���N�^
	~CDestroyScore();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �ÓI�����o�֐�
	static CDestroyScore* Create(CEnemy::TYPE type);	// ����

private:
	void SetEnemyScore(CEnemy::TYPE type);	//�G�̎�ނ��Ƃ̃X�R�A

	E_State m_State;	//�G��|�������̃X�R�A��ԕϐ�
	int m_nCntState;	//��ԊǗ�����
	D3DXCOLOR m_Col;	//�F�Ǘ�
	int m_nScore;		//�X�R�A���l
	int m_nValue;		//����
};

#endif
