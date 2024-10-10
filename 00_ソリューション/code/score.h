//*****************************************************
//
// �X�R�A�̏���[Score.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

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
class CScore : public CGameObject
{
public:
	// �񋓌^��`
	enum E_Number
	{// �����̎��
		NUMBER_MINUTE = 0,	// ��
		NUMBER_SECOND,	// �b
		NUMBER_MILLI,	// �~���b
		NUMBER_MAX
	};

	CScore();	// �R���X�g���N�^
	~CScore();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �ϐ��擾�E�ݒ�֐�
	float GetScore() { return m_fScore; }	// �X�R�A
	void SetScore(float fScore) { m_fScore = fScore; }
	void AddScore(float fScore) { m_fScore += fScore; }
	bool GetFlag() { return m_bStop; }	// ����t���O
	void SetFlag(bool bStop) { m_bStop = bStop; }
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��
	float GetScaleNumber(void) { return m_fScaleNumber; }
	void SetColor(E_Number number, D3DXCOLOR col);	// �F
	D3DXCOLOR GetColor(E_Number number);

	// �ÓI�����o�֐�
	static CScore* Create();	// ����
	static void SaveScore(float fSecond);	// �X�R�A�ۑ�
	static float LoadScore(void);	// �X�R�A�Ǎ�

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	// �����o�ϐ�
	float m_fScore;			// ���݂̃X�R�A
	float m_fScaleNumber;	// �����̃X�P�[��
	bool m_bStop;				// �^�C�}�[��~�̃t���O
	vector<CNumber*> m_aNumber;	// �����̔z��
};

#endif