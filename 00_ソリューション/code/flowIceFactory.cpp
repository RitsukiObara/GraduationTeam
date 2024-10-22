//*****************************************************
//
// ���X�t�@�N�g���[[flowIceFactory.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "flowIceFactory.h"
#include "iceManager.h"
#include "ice.h"
#include "flowIce.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_CREATE_FLOWICE = 5.0f;	// ���X����鎞��
const int NUM_ICE = 4;	// �K���ȏ����A�C�X��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CFlowIceFct::CFlowIceFct(int nPriority) : CObject(nPriority), m_fTimerCreateFlowIce(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFlowIceFct::~CFlowIceFct()
{

}

//=====================================================
// ��������
//=====================================================
CFlowIceFct *CFlowIceFct::Create(void)
{
	CFlowIceFct *pFct = nullptr;

	pFct = new CFlowIceFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// ����������
//=====================================================
HRESULT CFlowIceFct::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CFlowIceFct::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CFlowIceFct::Update(void)
{
	// �o�ߎ��ԉ��Z
	m_fTimerCreateFlowIce += CManager::GetDeltaTime();

	// ��莞�Ԃŗ��X�𗬂�
	if (m_fTimerCreateFlowIce > TIME_CREATE_FLOWICE)
	{
		// ���X�̐���
		CreateFlowIce();
	}
}

//=====================================================
// ���X�̐���
//=====================================================
void CFlowIceFct::CreateFlowIce(void)
{
	// ���X�V�X�e���̐���
	CFlowIce *pFlowIce = CFlowIce::Create();

	if (pFlowIce == nullptr)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �X�𐶐����A�V�X�e���ɒǉ�
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();

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
		pFlowIce->AddIceToArray(pIce);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CFlowIceFct::Draw(void)
{

}