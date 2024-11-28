//*****************************************************
//
// ���e�̏���[fishshadow.h]
// Author:��؈�^�@���R����
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace fishshadow
{
const float TIME_VANISH = 3.0f;								// ���ł܂ł̎���
const float DELAY_AFTER_VANISH = 1.0f;						// �e����������̃f�B���C
const float TIME_APPER = DELAY_AFTER_VANISH + TIME_VANISH;	// ���e���o�鎞��
}

//*****************************************************
// �N���X��`
//*****************************************************
class CFishShadow : public CPolygon3D
{
public:
	CFishShadow(int nPriority = 3);	// �R���X�g���N�^
	~CFishShadow();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CFishShadow* Create(int nPatern);

private:
	// �����o�֐�
	void InitSpawn(int nPatern);	// �X�|�[���̏�����

	// �����o�ϐ�
	float m_nTimerVanish;	// ������܂ł̃^�C�}�[
};

#endif