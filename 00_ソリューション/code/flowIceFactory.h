//*****************************************************
//
// ���X�t�@�N�g���[[flowIceFactory.h]
// Author:���R����
//
//*****************************************************
#ifndef _FLOWICEFCT_H_
#define _FLOWICEFCT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CFlowIceFct : public CObject
{
public:
	CFlowIceFct(int nPriority = 2);	// �R���X�g���N�^
	~CFlowIceFct();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CFlowIceFct *Create(void);	// ��������
private:
	// �\���̒�`
	struct S_InfoFlowIce
	{// ���X�̏��
		vector<vector<int>> aIdx;	// �ԍ��̕ۑ�
	};

	// �����o�֐�
	void Load(void);	// �Ǎ�
	void GetNumFlowIce(std::ifstream& file, string str, int *pNumV, int *pNumH, S_InfoFlowIce* pInfoFlowIce);
	void CreateFlowIce(void);	// ���X�̐���

	// �����o�ϐ�
	float m_fTimerCreateFlowIce;	// ���X�����^�C�}�[
	vector<S_InfoFlowIce*> m_apInfoFlowIce;	// ���X�̏��z��
};

#endif

