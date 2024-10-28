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
#include "number.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_PLACE	(10)	// �ő包��

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

	// �ÓI�����o�֐�
	static CUI_Combo* Create();	// ����

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	D3DXCOLOR m_Col;	//�F�Ǘ�
	int m_nValue;		//����
	int m_nCombo;			// ���݂̃X�R�A
	float m_fScaleNumber;	// �����̃X�P�[��
	vector<CNumber*> m_aNumber;	// �����̔z��
	E_State m_State;	//�G��|�������̃X�R�A��ԕϐ�
	int m_nCntState;	//��ԊǗ�����
};

#endif
