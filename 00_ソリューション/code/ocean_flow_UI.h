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
	static COceanFlowUI* GetInstance(void) { return s_pOceanFlowUI; }

private:
	// �񋓌^��`
	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �t�F�[�h�C�����
		STATE_OUT,	// �t�F�[�h�A�E�g���
		STATE_MAX
	};

	// �����o�֐�
	void CreateDir(void);			// �����I�u�W�F�N�g����
	void CreateArrow(void);			// ��󐶐�
	void CreateCompass(void);		// �R���p�X����
	void OceanRotState(void);		// �C���̌����␳����
	void OceanLevelState(void);		// �C�����x����UI����
	void ShakeArrow(float fRate);	// ���̗h��

	CObjectX* m_pArrow;						// ���̃��f��
	CObjectX* m_pCompass;					// �R���p�X�̃��f��
	CObject3D* m_pDir;						// �����p�̃I�u�W�F�N�g
	STATE m_state;							// ���
	float m_fTimerShakeArrow;				// ���h�炷���߂̃^�C�}�[
	float m_fSpeedShakeArrow;				// ���h��鑬�x
	static COceanFlowUI* s_pOceanFlowUI;	// ���g�̃|�C���^
};

#endif