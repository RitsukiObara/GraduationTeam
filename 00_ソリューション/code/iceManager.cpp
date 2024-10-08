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

	CreateIce(3, 6);
	CreateIce(3, 5);
	CreateIce(3, 4);
	CreateIce(3, 3);
	CreateIce(4, 3);
	CreateIce(5, 3);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(4, 6);
	CreateIce(6, 6);

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

	// �X�̏�ԊǗ�
	ManageStateIce();
}

//=====================================================
// �X�̏�ԊǗ�
//=====================================================
void CIceManager::ManageStateIce(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{
				m_aGrid[i][j].pIce->EnableBreak(false);
				m_aGrid[i][j].pIce->EnableCanFind(true);
			}
		}
	}
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
	D3DXVECTOR3 pos = pIce->GetPosition();

	AddIce(pIce, pos);

	// �O���b�h�ɑ���������U��
}

//=====================================================
// �X����
//=====================================================
void CIceManager::PeckIce(D3DXVECTOR3 pos, E_Direction direction)
{
	// �ꏊ����O���b�h���v�Z
	int nH = (int)(((pos.x + Grid::SIZE * m_nNumGridHorizontal * 0.5f) / Grid::SIZE * m_nNumGridHorizontal) * 0.1f);
	int nV = (int)(((pos.z + Grid::SIZE * m_nNumGridVirtical * 0.5f) / Grid::SIZE * m_nNumGridVirtical) * 0.1f);

	D3DXVECTOR3 posEffect;
	posEffect = { nH * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f,0.0f,nV * Grid::SIZE - Grid::SIZE * m_nNumGridVirtical * 0.5f };

	if (m_aGrid[nV][nH].pIce != nullptr)
	{
		// ������X���������Ȃ��悤�ɂ���
		m_aGrid[nV][nH].pIce->EnableCanFind(false);

		// �X�j��t���O�����Ă�
		m_bBreakIce = true;
	}

	CIce *pIceStand = m_aGrid[nV][nH].pIce;

	switch (direction)
	{
	case CIceManager::DIRECTION_UP:
		nV++;
		break;
	case CIceManager::DIRECTION_RIGHT:
		nH++;
		break;
	case CIceManager::DIRECTION_DOWN:
		nV--;
		break;
	case CIceManager::DIRECTION_LEFT:
		nH--;
		break;
	default:
		break;
	}

	// �X��˂���������ɂ���
	if (m_aGrid[nV][nH].pIce)
	{
		m_aGrid[nV][nH].pIce->EnablePeck(true);
		m_aGrid[nV][nH].pIce->EnableBreak(true);
	}

	// �X�T���̍ċA�֐�
	FindIce(nV, nH, 0, pIceStand);

	// �X������t���O�������Ă�����X����
	if (m_bBreakIce)
		BreakIce();
}

//=====================================================
// �X�̒T��
//=====================================================
void CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand)
{
	if (m_aGrid[nNumV][nNumH].pIce != nullptr)
	{
		// �T���ς݃t���O�𗧂Ă�
		m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);

		CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		nIdx++;
	}

	vector<CIce*> apIce(DIRECTION_MAX);

	apIce[DIRECTION_UP] = m_aGrid[nNumV + 1][nNumH].pIce;
	apIce[DIRECTION_DOWN] = m_aGrid[nNumV - 1][nNumH].pIce;
	apIce[DIRECTION_RIGHT] = m_aGrid[nNumV][nNumH + 1].pIce;
	apIce[DIRECTION_LEFT] = m_aGrid[nNumV][nNumH - 1].pIce;
	
	// �l�����X���Ȃ����T���ł��Ȃ���ԂȂ�I��
	bool bNothing = true;

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if(apIce[i] == pIceStand && nIdx != 1)
		{// �����Ă���X�ɒH�蒅������X���󂳂Ȃ�
			m_bBreakIce = false;
		}

		if (apIce[i]->IsPeck())
			continue;

		if (apIce[i]->IsCanFind() == false)
			continue;
		
		switch (i)
		{
		case CIceManager::DIRECTION_UP:
			FindIce(nNumV + 1, nNumH, nIdx, pIceStand);
			break;
		case CIceManager::DIRECTION_RIGHT:
			FindIce(nNumV, nNumH + 1, nIdx, pIceStand);
			break;
		case CIceManager::DIRECTION_DOWN:
			FindIce(nNumV - 1, nNumH, nIdx, pIceStand);
			break;
		case CIceManager::DIRECTION_LEFT:
			FindIce(nNumV, nNumH - 1, nIdx, pIceStand);
			break;
		default:
			break;
		}

		bNothing = false;
	}

	if (bNothing)
	{
		return;
	}
}

//=====================================================
// �X�̒ǉ�
//=====================================================
void CIceManager::AddIce(CIce *pIce, D3DXVECTOR3 pos)
{
	if (pIce == nullptr)
		return;

	// �ꏊ����O���b�h���v�Z
	int nH = (int)((pos.x + Grid::SIZE * m_nNumGridHorizontal * 0.5f) / Grid::SIZE * m_nNumGridHorizontal * 0.1f);
	int nV = (int)((pos.z + Grid::SIZE * m_nNumGridVirtical * 0.5f) / Grid::SIZE * m_nNumGridVirtical * 0.1f);

	m_aGrid[nV][nH].pIce = pIce;
}

//=====================================================
// �X�̔j��
//=====================================================
void CIceManager::BreakIce(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{
				// ���锻��Ɠ˂������X����
				if (m_aGrid[i][j].pIce->IsBreak() || 
					m_aGrid[i][j].pIce->IsPeck())
				{
					m_aGrid[i][j].pIce->Uninit();
					m_aGrid[i][j].pIce = nullptr;
				}
			}
		}
	}
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
			D3DXCOLOR col = { (float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

			if (m_aGrid[i][j].pIce != nullptr)
			{
				if (m_aGrid[i][j].pIce->IsBreak())
				{
					col = { 0.0f,0.0f,1.0f,1.0f };
				}

				if (m_aGrid[i][j].pIce->IsPeck())
				{
					col = { 0.0f,1.0f,0.0f,1.0f };
				}
			}

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