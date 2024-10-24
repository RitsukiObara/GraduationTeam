//*****************************************************
//
// ���X�̏���[flowIce.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "flowIce.h"
#include "ice.h"
#include "iceManager.h"
#include "effect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_ICE = 4;	// �X�̐�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFlowIce::~CFlowIce()
{

}

//=====================================================
// ��������
//=====================================================
CFlowIce *CFlowIce::Create(void)
{
	CFlowIce* pFlowice = nullptr;

	pFlowice = new CFlowIce;

	if (pFlowice == nullptr)
		return nullptr;

	pFlowice->Init();

	return pFlowice;
}

//=====================================================
// ����������
//=====================================================
HRESULT CFlowIce::Init(void)
{
	// �X�̐���
	//CreateIce();

	return S_OK;
}

//=====================================================
// �X�̐���
//=====================================================
void CFlowIce::CreateIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int aV[NUM_ICE] =
	{// �c�̃O���b�h�ԍ��̔z��
		3,4,5,6
	};
	int aH[NUM_ICE] =
	{// ���̃O���b�h�ԍ��̔z��
		9,9,9,9
	};

	for (int i = 0; i < NUM_ICE; i++)
	{
		CIce *pIce = pIceManager->CreateIce(aV[i], aH[i]);
		pIce->ChangeState(new CIceStateFlow);
		m_apIce.push_back(pIce);
	}
}

//=====================================================
// �X��z��ɒǉ�
//=====================================================
void CFlowIce::AddIceToArray(CIce *pIce)
{
	m_apIce.push_back(pIce);
}

//=====================================================
// �I������
//=====================================================
void CFlowIce::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CFlowIce::Update(void)
{
	// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
	CheckSomeIceStop();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
//=====================================================
void CFlowIce::CheckSomeIceStop(void)
{
	for (auto it : m_apIce)
	{
		if (it->IsStop())
		{// �~�܂��Ă���Ȃ�S�Ă̕X���~�߂�
			StopAllIce();
			Uninit();	// ���g�̔j��
			break;
		}
	}
}

//=====================================================
// �S�Ă̕X���~�߂�
//=====================================================
void CFlowIce::StopAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIce)
	{
		it->ChangeState(new CIceStaeteNormal);

		pIceManager->AddIce(it, it->GetPosition());
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CFlowIce::Debug(void)
{
	for (auto it : m_apIce)
	{
		D3DXVECTOR3 posIce = it->GetPosition();

		CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CFlowIce::Draw(void)
{

}