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
#include "number3D.h"
#include "score.h"
#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CDestroyScore : public CGameObject
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
	void SetEnemyScore(CEnemy::TYPE type);	//�G�̎�ނ��Ƃ̃X�R�A
	void SetScore(int nDigit = 6);
	void SetColor(D3DXCOLOR col);	// �F
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	D3DXCOLOR m_Col;	//�F�Ǘ�
	int m_nValue;		//����
	int m_nScore;			// ���݂̃R���{
	float m_fScaleNumber;	// �����̃X�P�[��
	CNumber3D* m_aNumber3D;	// �����̔z��
	E_State m_State;	//�G��|�������̃X�R�A��ԕϐ�
	int m_nCntState;	//��ԊǗ�����
};

#endif
