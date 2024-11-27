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

	void ResultState(void); // ���e��ԊǗ�
	void FishShadowSeals(void); // �A�U���V�̋��e����
	void FishShadowBears(void); // ���낭�܂̋��e����

	FISHSHADOW m_FishShadow;	// �I������
	CObject3D* m_apFishShadow[FISHSHADOW_MAX];	// ���e��3D�I�u�W�F�N�g
	D3DXVECTOR3 m_aPosDest[FISHSHADOW_MAX];	// �ڕW�̈ʒu
	static CFishShadow* m_pFishShadow;	// ���g�̃|�C���^
};

#endif