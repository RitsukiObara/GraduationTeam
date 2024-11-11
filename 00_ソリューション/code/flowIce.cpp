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
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_DELETE = 20.0f;	// �X��������܂ł̎���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
vector<CFlowIce*> CFlowIce::s_vector;	// �i�[�p�̔z��

//=====================================================
// �R���X�g���N�^
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority), m_fTimerDelete(0.0f)
{
	s_vector.push_back(this);
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
	return S_OK;
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
	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++)
	{
		if (*itr == this)
		{
			s_vector.erase(itr);

			break;
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CFlowIce::Update(void)
{
	// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
	CheckSomeIceStop();

	// �폜�m�F
	CheckDelete();

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
// ��ɂ�����̂��~�߂�
//=====================================================
void CFlowIce::StopOnTopObject(void)
{

}

//=====================================================
// �S�X�̍폜
//=====================================================
void CFlowIce::DeleteAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		pIceManager->DeleteIce(m_apIce[i]);
		m_apIce[i]->Uninit();
	}

	m_apIce.clear();
}

//=====================================================
// �X�폜�̊m�F
//=====================================================
void CFlowIce::CheckDelete(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	m_fTimerDelete += CManager::GetDeltaTime();

	if (m_fTimerDelete > TIME_DELETE)
	{
		for (int i = 0; i < (int)m_apIce.size(); i++)
		{
			if (m_apIce[i] == nullptr)
				continue;

			// ��ʓ�����
			D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
			if (universal::IsInScreen(pos, nullptr))
				return;	// �ǂꂩ��ł���ʓ��ɂ���Ί֐����I��
		}

		// �����܂Œʂ�����X���폜
		DeleteAllIce();
		Uninit();
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

		//CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CFlowIce::Draw(void)
{

}