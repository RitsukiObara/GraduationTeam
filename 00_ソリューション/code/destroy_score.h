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
#include "UI_combo.h"

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

	CDestroyScore();	// �R���X�g���N�^
	~CDestroyScore();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void AddDestroyScore(CEnemy::TYPE type);
	void AddComboScore(void);	// �R���{�X�R�A���v�Z���ĉ��Z

	// �ÓI�����o�֐�
	static CDestroyScore* Create();	// ����

	void SetScore(void);
	void SetAlpha(float fAlpha);	// �����x
	void SetState(CUI_Combo::E_State state);	// ���
	void SetShiftPos(D3DXVECTOR3 shiftpos);	// ���炷��
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	int m_nScore;			// ���݂̓G��|�������̃X�R�A
	float m_fScaleNumber;	// �����̃X�P�[��
	CNumber3D* m_aNumber3D;	// �����̔z��
	CUI_Combo::E_State m_state;	//�G��|�������̃X�R�A��ԕϐ�
	D3DXVECTOR3 m_ShiftPos;		// ���炷��
};

#endif
