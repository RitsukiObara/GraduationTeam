//*****************************************************
//
// �GUI�̏���[UI_enemy.h]
// Author:����F�M
//
//*****************************************************
#ifndef _UI_ENEMY_H_
#define _UI_ENEMY_H_

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
class CUI_Enemy : public CGameObject
{
public:
	CUI_Enemy();	// �R���X�g���N�^
	~CUI_Enemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	// �ϐ��擾�E�ݒ�֐�
	int GetUI() { return m_nEnemy; }	// �G�̐�
	void SetUI(int nUi) { m_nEnemy = nUi; }
	void SetPosition(D3DXVECTOR3 pos) override;	// �ʒu
	void SetScaleNumber(float fScale);	// �����̃X�P�[��
	float GetScaleNumber(void) { return m_fScaleNumber; }
	//void SetColor(E_Number number, D3DXCOLOR col);	// �F
	//D3DXCOLOR GetColor(E_Number number);

	// �ÓI�����o�֐�
	static CUI_Enemy* Create(int m_nEnemy);	// ����

private:
	// �����o�֐�
	void UpdateNumber();	// �����̍X�V
	void TransformNumber();	// �����̃g�����X�t�H�[���ݒ�

	// �����o�ϐ�
	int m_nEnemy;			// ���݂̃X�R�A
	float m_fScaleNumber;	// �����̃X�P�[��
	vector<CNumber*> m_aNumber;	// �����̔z��
};

#endif
