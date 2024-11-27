//*****************************************************
//
// ���e�̏���[fishshadow.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

//*****************************************************
// �N���X��`
//*****************************************************
class CFishShadow : public CObject3D
{
public:
	CFishShadow();	// �R���X�g���N�^
	~CFishShadow();	// �f�X�g���N�^

	static CFishShadow* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFishShadow* GetInstance(void) { return m_pFishShadow; }

private:

	// ���e�̎��
	enum FISHSHADOW
	{
		FISHSHADOW_SEALS = 0,	// �A�U���V
		FISHSHADOW_BEARS,	// ���낭��
		FISHSHADOW_MAX
	};

	void ResultState(void); // ���U���gUI�̏��
	void ResultClear(void); // ���U���g���N���A�̎�
	void ResultFail(void); // ���U���g�����s�̎�

	FISHSHADOW m_FishShadow;	// �I������
	D3DXVECTOR3 m_aPosDest[FISHSHADOW_MAX];	// �ڕW�̈ʒu
	static CFishShadow* m_pFishShadow;	// ���g�̃|�C���^
	bool m_bSound;
	int nCountMove;
};

#endif