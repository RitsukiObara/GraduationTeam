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

	// �O���b�h�̈ʒu�ݒ�
	SetGridPos();

	// ���}�b�v����
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
// �O���b�h�̈ʒu��ݒ�
//=====================================================
void CIceManager::SetGridPos(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			D3DXVECTOR3 pos;
			pos = { j * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f ,10.0f,i * Grid::SIZE * 0.67f - Grid::SIZE * m_nNumGridVirtical * 0.5f };

			// �c�ŋ����񂾂����炸�炷
			if (i % 2 == 0)
			{
				pos.x += Grid::SIZE * 0.5f;
			}

			m_aGrid[i][j].pos = pos;
		}
	}
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

	// �X�̃g�����X�t�H�[���ݒ�
	pIce->SetPosition(m_aGrid[nGridV][nGridH].pos);
	pIce->SetSize(Grid::SIZE * 0.5f, Grid::SIZE * 0.5f);

	// �X��z��ɃZ�b�g
	m_aGrid[nGridV][nGridH].pIce = pIce;

	// �X�̒�~
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

	// �O���b�h�ɑ���������U��
}

//=====================================================
// �X����
//=====================================================
void CIceManager::PeckIce(int nNumV, int nNumH, E_Direction direction)
{
	if (m_aGrid[nNumV][nNumH].pIce != nullptr)
	{
		// ������X���������Ȃ��悤�ɂ���
		m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

		// �X�j��t���O�����Ă�
		m_bBreakIce = true;
	}

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;

	switch (direction)
	{
	/*case CIceManager::DIRECTION_UP:
		nNumV++;
		break;*/
	case CIceManager::DIRECTION_RIGHT:
		nNumH++;
		break;
	/*case CIceManager::DIRECTION_DOWN:
		nNumV--;
		break;*/
	case CIceManager::DIRECTION_LEFT:
		nNumH--;
		break;
	default:
		break;
	}

	// �X��˂���������ɂ���
	if (m_aGrid[nNumV][nNumH].pIce)
	{
		m_aGrid[nNumV][nNumH].pIce->EnablePeck(true);
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);
	}

	// �X�T���̍ċA�֐�
	FindIce(nNumV, nNumH, 0, pIceStand);

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

	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	// �E��
	aV[DIRECTION_RIGHTUP] = nNumV + 1;
	aV[DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0)
	{// �����̎�
		// �O���b�h�ԍ���ۑ�
		aH[DIRECTION_RIGHTUP] = nNumH + 1;
		aH[DIRECTION_RIGHTDOWN] = nNumH + 1;

		// ���ȂȂ�
		aH[DIRECTION_LEFTUP] = nNumH;
		aH[DIRECTION_LEFTDOWN] = nNumH;
	}
	else
	{// ��̎�

		// �O���b�h�ԍ���ۑ�
		aH[DIRECTION_RIGHTUP] = nNumH;
		aH[DIRECTION_RIGHTDOWN] = nNumH;

		// ���ȂȂ�
		aH[DIRECTION_LEFTUP] = nNumH - 1;
		aH[DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// ����
	aV[DIRECTION_LEFTUP] = nNumV + 1;
	aV[DIRECTION_LEFTDOWN] = nNumV - 1;

	// ���E
	aV[DIRECTION_RIGHT] = nNumV;
	aV[DIRECTION_LEFT] = nNumV;
	aH[DIRECTION_RIGHT] = nNumH + 1;
	aH[DIRECTION_LEFT] = nNumH - 1;

	// �l�����X���Ȃ����T���ł��Ȃ���ԂȂ�I��
	bool bNothing = true;

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		// �X�̃|�C���^�̕ۑ�
		apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;

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
		
		FindIce(aV[i], aH[i], nIdx, pIceStand);

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

			CEffect3D::Create(m_aGrid[i][j].pos, 50.0f, 5, col);
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n�X�̑���[%d]", CIce::GetNumAll());
}

//=====================================================
// �O���b�h�ʒu�̎擾
//=====================================================
D3DXVECTOR3 CIceManager::GetGridPosition(int nNumV, int nNumH)
{
	if (m_aGrid.empty())
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return m_aGrid[nNumV][nNumH].pos;
}

//=====================================================
// �`�揈��
//=====================================================
void CIceManager::Draw(void)
{

}