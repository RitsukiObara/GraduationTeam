//*****************************************************
//
// �X�J�C�{�b�N�X�̏���[IceManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIceManager::~CIceManager()
{

}

//=====================================================
// ��������
//=====================================================
CIceManager *CIceManager::Create(int nNumV, int nNumH)
{
	if (s_pIceManager == nullptr)
	{
		s_pIceManager = new CIceManager;

		if (s_pIceManager != nullptr)
		{
			s_pIceManager->m_nNumGridVirtical = nNumV;
			s_pIceManager->m_nNumGridHorizontal = nNumH;
			s_pIceManager->Init();
		}
	}

	return s_pIceManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CIceManager::Init(void)
{
	// �w�肳�ꂽ���������O���b�h�z������T�C�Y
	// �c�̃O���b�h�p��
	m_aGrid.resize(m_nNumGridVirtical);

	for (int i = 0; i < m_nNumGridVirtical; i++)
		m_aGrid[i].resize(m_nNumGridHorizontal);

	// �����̕X�̐���
	CIce *pIce = CreateIce(5, 9);

	CreateIce(5, 9);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(5, 3);
	CreateIce(4, 6);
	CreateIce(6, 6);

	StopIce(pIce);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CIceManager::Uninit(void)
{
	s_pIceManager = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CIceManager::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �X�̐���
//=====================================================
CIce *CIceManager::CreateIce(int nGridV, int nGridH)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create();

	if (pIce == nullptr)
		return nullptr;

	D3DXVECTOR3 pos;
	pos = { nGridH * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f ,10.0f,nGridV * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };
	pIce->SetPosition(pos);
	pIce->SetSize(Grid::SIZE * 0.5f, Grid::SIZE * 0.5f);

	StopIce(pIce);

	return pIce;
}

//=====================================================
// �X�̒�~
//=====================================================
void CIceManager::StopIce(CIce *pIce)
{
	if (pIce == nullptr)
		return;

	pIce->SetState(CIce::E_State::STATE_STOP);

	// ������O���b�h�̌v�Z
	D3DXVECTOR3 pos = GetPosition();

	// ������O���b�h�Ƃ��̎��ӂ̏�Ԃ�ݒ�

	// �^��

	// �[����
}

//=====================================================
// �X����
//=====================================================
void CIceManager::PeckIce(D3DXVECTOR3 pos)
{
	// �ꏊ����O���b�h���v�Z
	int nH = (pos.x + Grid::SIZE * m_nNumGridHorizontal * 0.5f) / Grid::SIZE * m_nNumGridHorizontal * 0.1f;
	int nV = (pos.z + Grid::SIZE * m_nNumGridVirtical * 0.5f) / Grid::SIZE * m_nNumGridVirtical * 0.1f;

	D3DXVECTOR3 posEffect;
	posEffect = { nH * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f,0.0f,nV * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };

	CEffect3D::Create(posEffect, 100.0f, 100, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CIceManager::Debug(void)
{
	// �O���b�h�̏ꏊ���f�o�b�O�\��
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// ���̃O���b�h
			m_aGrid[i][j];

			D3DXVECTOR3 pos;
			pos = { j * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f,0.0f,i * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };
			D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

			CEffect3D::Create(pos, 50.0f, 5, col);
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n�X�̑���[%d]", CIce::GetNumAll());
}

//=====================================================
// �`�揈��
//=====================================================
void CIceManager::Draw(void)
{

}