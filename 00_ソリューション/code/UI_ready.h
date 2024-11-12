//*****************************************************
//
// �Q�[���X�^�[�g���mUI����[UI_ready.h]
// Author:�X��x��
//
//*****************************************************
#ifndef _UI_READY_H_
#define _UI_READY_H_

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
// �N���X�̒�`
//*****************************************************
class CUIready : public CGameObject
{
public:
	// �񋓌^��`
	enum E_Number
	{// �����̎��
		NUMBER_SECOND = 0,	// �b
		NUMBER_MAX
	};

	// �񋓌^��`
	enum STATE
	{// �e�N�X�`���̎��
		STATE_NUMBER = 0,	//�������
		STATE_GO,			//GO�w��
		STATE_MAX
	};

	CUIready();	// �R���X�g���N�^
	~CUIready();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CUIready* Create();	// ����

	// �ϐ��擾�E�ݒ�֐�
	int GetSecond() { return m_nSecond; }	// �b
	void SetSecond(int nSecond) { m_nSecond = nSecond; }
	void AddSecond(int nSecond) { m_nSecond += nSecond; }
	bool GetFlag() { return m_bStop; }	// ����t���O
	void SetFlag(bool bStop) { m_bStop = bStop; }
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��
	float GetScaleNumber(void) { return m_fScaleNumber; }
	void SetColor(E_Number number, D3DXCOLOR col);	// �F
	D3DXCOLOR GetColor(E_Number number);

	// �ÓI�����o�֐�
	static void SaveSecond(int nSecond);	// ���ԕۑ�
	static int LoadSecond(void);	// ���ԓǍ�

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	// �����o�ϐ�
	int m_nSecond;			// ���݂̎���(�b)
	float m_fScaleNumber;	// �����̃X�P�[��
	bool m_bStop;				// �^�C�}�[��~�̃t���O
	vector<CNumber*> m_aNumber;	// �����̔z��
	int m_nFrame;				//�t���[���v�Z
	STATE m_state;				//��ԕϐ�
	CUI* m_Go;	// GO�̔z��
	int m_StateCnt;	//��ԑJ�ڃJ�E���g
};

#endif
