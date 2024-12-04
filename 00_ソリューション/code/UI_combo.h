//*****************************************************
//
// �R���{UI�̏���[UI_combo.h]
// Author:����F�M
//
//*****************************************************

#ifndef _UI_COMBO_H_
#define _UI_COMBO_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"
#include "number3D.h"
#include "enemy.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CDestroyScore;

class CUI_Combo : public CGameObject
{
public:
	// �񋓌^��`
	enum E_State
	{// ���
		STATE_BESIDE = 0,	// ���ړ�
		STATE_VERTICAL,	// �c�ړ�
		STATE_WAIT,	// �ؗ����
		STATE_ERASE,	// �����Ă���
		STATE_MAX
	};

	CUI_Combo();	// �R���X�g���N�^
	~CUI_Combo();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetCombo(int nDigit = 6);
	void AddCombo(CEnemy::TYPE type);

	// �ÓI�����o�֐�
	void SetColor(D3DXCOLOR col);	// �F
	int GetCombo(void) { return m_nCombo; }
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��
	static CUI_Combo* GetInstance(void);	// �C���X�^���X�̎擾

private:
	// �����o�֐�
	static CUI_Combo* Create();	// ����
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	CDestroyScore* m_pScore;	// �X�R�A
	D3DXCOLOR m_Col;	//�F�Ǘ�
	int m_nValue;		//����
	int m_nCombo;			// ���݂̃R���{
	float m_fScaleNumber;	// �����̃X�P�[��
	CNumber3D* m_aNumber3D;	// �����̔z��
	E_State m_State;	//�G��|�������̃X�R�A��ԕϐ�
	int m_nCntState;	//��ԊǗ�����
	D3DXVECTOR3 m_ShiftPos;	//���炷�ʒu

	static CUI_Combo* s_pCombo;	// �i�[�p�̔z��
};

#endif
