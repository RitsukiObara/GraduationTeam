//*****************************************************
//
// �X�e�[�W���U���gUI�̏���[stageResultUI.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _OCEAN_FLOW_UI_H_
#define _OCEAN_FLOW_UI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObjectX;

//*****************************************************
// �N���X��`
//*****************************************************
class COceanFlowUI : public CObject
{
public:
	COceanFlowUI();	// �R���X�g���N�^
	~COceanFlowUI();	// �f�X�g���N�^

	static COceanFlowUI* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static COceanFlowUI* GetInstance(void) { return m_pOceanFlowUI; }

private:

	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �t�F�[�h�C�����
		STATE_OUT,	// �t�F�[�h�A�E�g���
		STATE_MAX
	};

	void OceanLevelState(void); // �C�����x����UI����

	CObjectX* m_pArrow;	// ���̃��f��
	STATE m_state;	// ���
	int OceanFlowKeep;	//	�C���̕ۑ�
	float OceanFlowLevel;	//	�C�����x���̕ۑ�
	static COceanFlowUI* m_pOceanFlowUI;	// ���g�̃|�C���^
};

#endif