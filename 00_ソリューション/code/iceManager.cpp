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
#include "particle.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// �Z�p�`�̊���X
const float RATE_HEX_Z = 0.3f;	// �Z�p�`�̊���Z

const float WIDTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_X;	// �O���b�h�̕�
const float DEPTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_Z;	// �O���b�h�̉��s��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority), m_nNumGridVirtical(0), m_nNumGridHorizontal(0) ,m_dirStream(E_Direction::DIRECTION_RIGHTUP)
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
	CreateIce(3, 6,CIce::E_Type::TYPE_HARD);
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
			pos = { j * WIDTH_GRID - WIDTH_GRID * m_nNumGridHorizontal * 0.5f,10.0f,i * DEPTH_GRID - DEPTH_GRID * m_nNumGridVirtical * 0.5f };

			// �c�ŋ����񂾂����炸�炷
			if (i % 2 == 0)
			{
				pos.x += WIDTH_GRID * 0.5f;
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

	m_aGrid.clear();

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
	if (m_aGrid.empty())
		return;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// �������X�ɋ��܂�Ă邩�̔���
			if (JudgeBetweenPeck(i, j))
				continue;

			m_aGrid[i][j].pIce->EnableBreak(false);
			m_aGrid[i][j].pIce->EnableCanFind(true);
			m_aGrid[i][j].pIce->EnableAliveStandIce(false);
		}
	}
}

//=====================================================
// �������X�ɋ��܂�Ă邩�̔���
//=====================================================
bool CIceManager::JudgeBetweenPeck(int nNumV, int nNumH)
{
	return false;
}

//=====================================================
// �X�̐���
//=====================================================
CIce *CIceManager::CreateIce(int nGridV, int nGridH, CIce::E_Type type)
{
	if (m_aGrid[nGridV][nGridH].pIce != nullptr)
		return m_aGrid[nGridV][nGridH].pIce;

	CIce *pIce = nullptr;

	pIce = CIce::Create(type);

	if (pIce == nullptr)
		return nullptr;

	// �X�̃g�����X�t�H�[���ݒ�
	pIce->SetPosition(m_aGrid[nGridV][nGridH].pos);
	pIce->SetTransform(Grid::SIZE);

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

	pIce->SetState(CIce::E_State::STATE_NORMAL);

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
	}

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	switch (direction)
	{
	/*case CIceManager::DIRECTION_UP:
		nNumV++;
		break;*/
	case CIceManager::DIRECTION_RIGHT:
		nNumBreakH++;
		break;
	/*case CIceManager::DIRECTION_DOWN:
		nNumV--;
		break;*/
	case CIceManager::DIRECTION_LEFT:
		nNumBreakH--;
		break;
	default:
		break;
	}

	if (m_aGrid[nNumBreakV][nNumBreakH].pIce == nullptr)
		return;

	if (!m_aGrid[nNumBreakV][nNumBreakH].pIce->IsCanPeck())
		return;	// �˂����Ȃ��u���b�N�Ȃ��̏�����ʂ�Ȃ�

	// �X��˂���������ɂ���
	if (m_aGrid[nNumBreakV][nNumBreakH].pIce)
	{
		m_aGrid[nNumBreakV][nNumBreakH].pIce->EnablePeck(true);
		m_aGrid[nNumBreakV][nNumBreakH].pIce->ChangeState(new CIceStaeteBreak);
	}

	// �X�T���̍ċA�֐�
	FindIce(nNumBreakV, nNumBreakH, 0, pIceStand, apIce,false);

	// �T���t���O�̖�����
	DisableFind();

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsCanPeck())
				continue;

			// �T���t���O�̖�����
			DisableFind();

			// ���Ȃ��u���b�N���s���M������
			DisableFromHardIce(i, j, apIce);
		}
	}

	// �T���t���O�̖�����
	DisableFind();

	// �v���C���[����󂳂Ȃ��u���b�N�̗�����o��
	DisableFromPlayer(nNumV, nNumH, m_aGrid[nNumBreakV][nNumBreakH].pIce, apIce);

	// �X������t���O�������Ă�����X����
	BreakIce();
}

//=====================================================
// �T���ς݃t���O�̖�����
//=====================================================
void CIceManager::DisableFind(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			m_aGrid[i][j].pIce->EnableCanFind(true);
		}
	}
}

//=====================================================
// ���ӂ̕X�̎擾
//=====================================================
vector<CIce*> CIceManager::GetAroundIce(int nNumV, int nNumH)
{
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
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
	}

	return apIce;
}

//=====================================================
// �w�肵���X�Ɠ����|�C���^���k���ɂ���
//=====================================================
void CIceManager::DeleteIce(CIce *pIce)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce == pIce)
				m_aGrid[i][j].pIce = nullptr;
		}
	}
}

//=====================================================
// �O�ɏo���Ȃ��悤�ɂ��锻��
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos)
{
	if (pPos == nullptr)
		return;

	int nIdxV = 0;
	int nIdxH = 0;

	// �ʒu����A������O���b�h���v�Z
	GetIdxGridFromPosition(*pPos, &nIdxV, &nIdxH);

#ifdef _DEBUG
	D3DXVECTOR3 posIce = m_aGrid[nIdxV][nIdxH].pos;
	CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	CDebugProc::GetInstance()->Print("\n������O���b�h[%d,%d]", nIdxV, nIdxH);
#endif

	// ������O���b�h�̕X�Ƃ̔�����s��
	LimitInIce(pPos, nIdxV, nIdxH);
}

//=====================================================
// �X���Ɏ��߂鏈��
//=====================================================
void CIceManager::LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH)
{
	if (pPos == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	D3DXVECTOR3 posIce = m_aGrid[nNumV][nNumH].pIce->GetPosition();

	pPos->y = posIce.y;

	//if (pPos->x >= posIce.x + WIDTH_GRID * 0.5f)
	//	pPos->x = posIce.x + WIDTH_GRID * 0.5f;	// �E���̕␳
	//else if (pPos->x <= posIce.x - WIDTH_GRID * 0.5f)
	//	pPos->x = posIce.x - WIDTH_GRID * 0.5f;	// �����̕␳

	//if (pPos->z >= posIce.z + DEPTH_GRID * 0.5f)
	//	pPos->z = posIce.z + DEPTH_GRID * 0.5f;	// �����̕␳
	//else if (pPos->z <= posIce.z - DEPTH_GRID * 0.5f)
	//	pPos->z = posIce.z - DEPTH_GRID * 0.5f;	// ��O���̕␳
}

//=====================================================
// �X�̒T��
//=====================================================
bool CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand, vector<CIce*> apIceLast, bool bBreakLast)
{
	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return false;
	
	// �T���ς݃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	if (!m_aGrid[nNumV][nNumH].pIce->IsPeck())
	{
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);
	}

#ifdef _DEBUG
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// �ċA�֐��̐[�����C���N�������g
	nIdx++;

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	
	CalcAroundGrids(nNumV, nNumH, aV, aH);

	// �l�����X���Ȃ����T���ł��Ȃ���ԂȂ�I��
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsCanFind() == false)
			continue;

		FindIce(aV[i], aH[i], nIdx, pIceStand, apIce, true);
	}

	return false;
}

//=====================================================
// ���ӃO���b�h�̌v�Z
//=====================================================
void CIceManager::CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH)
{
	// �E��E�E��
	aV[DIRECTION_RIGHTUP] = nNumV + 1;
	aV[DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0) {  // �����̎�
		aH[DIRECTION_RIGHTUP] = nNumH + 1;
		aH[DIRECTION_RIGHTDOWN] = nNumH + 1;
		aH[DIRECTION_LEFTUP] = nNumH;
		aH[DIRECTION_LEFTDOWN] = nNumH;
	}
	else {  // ��̎�
		aH[DIRECTION_RIGHTUP] = nNumH;
		aH[DIRECTION_RIGHTDOWN] = nNumH;
		aH[DIRECTION_LEFTUP] = nNumH - 1;
		aH[DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// �����E���E
	aV[DIRECTION_LEFTUP] = nNumV + 1;
	aV[DIRECTION_LEFTDOWN] = nNumV - 1;
	aV[DIRECTION_RIGHT] = nNumV;
	aV[DIRECTION_LEFT] = nNumV;
	aH[DIRECTION_RIGHT] = nNumH + 1;
	aH[DIRECTION_LEFT] = nNumH - 1;
}

//=====================================================
// �����Ă���u���b�N�̊m�F
//=====================================================
bool CIceManager::CheckStandBlock(vector<CIce*> apIce, CIce *pIceStand, int nIdx)
{
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if ((apIce[i] == pIceStand || apIce[i]->IsAliveStandIce()) && nIdx != 1)
		{
			return true;
		}
	}

	return false;
}

//=====================================================
// �X�̒ǉ�
//=====================================================
void CIceManager::AddIce(CIce *pIce, D3DXVECTOR3 pos)
{

}

//=====================================================
// �d���X����M�����o���āA�j��M��������
//=====================================================
void CIceManager::DisableFromHardIce(int nNumV, int nNumH, vector<CIce*> apIce)
{
	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce->IsPeck())
		return;

	// �T���ς݃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

#ifdef _DEBUG
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	CalcAroundGrids(nNumV, nNumH, aV, aH);

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// �v���C���[����M�����˂Ŕj��M������
//=====================================================
void CIceManager::DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck, vector<CIce*> apIce)
{
	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce->IsPeck())
		return;

	// �T���ς݃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

#ifdef _DEBUG
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	CalcAroundGrids(nNumV, nNumH, aV, aH);

	// �l�����X���Ȃ����T���ł��Ȃ���ԂȂ�I��
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i] == pIcePeck)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// �j��M���̉���
//=====================================================
void CIceManager::DisableBreak(int nNumV, int nNumH)
{
	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	// �T���ς݃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

#ifdef _DEBUG
	CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	CalcAroundGrids(nNumV, nNumH, aV, aH);

	// �l�����X���Ȃ����T���ł��Ȃ���ԂȂ�I��
	bool bNothing = true;
	int nNumIce = 0;
	int nNumPeckIce = 0;
	bool bAliveStandBlock = false;

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValueInt(&nH, m_nNumGridHorizontal - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;
		}
	}

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		DisableBreak(aV[i], aH[i]);
	}
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
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (!m_aGrid[i][j].pIce->IsBreak())
				continue;

			m_aGrid[i][j].pIce->ChangeState(new CIceStaeteFlow);
			m_aGrid[i][j].pIce = nullptr;

			BreakPeck(i, j);
		}
	}
}

//=====================================================
// �p�̊m�F
//=====================================================
bool CIceManager::CheckCorner(int nNumV, int nNumH)
{
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumIce = 0;
	int nNumBreakIce = 0;

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		nNumIce++;

		if (it->IsBreak() || it->IsPeck())
			nNumBreakIce++;
	}

	if (nNumIce > 0)
	{
		if (nNumBreakIce == nNumIce)
			return false;
	}

	CEffect3D::Create(m_aGrid[nNumV][nNumH].pos, 300.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	return true;
}

//=====================================================
// ���ʕX�̊m�F
//=====================================================
bool CIceManager::CheckCommon(vector<CIce*> apIce, vector<CIce*> apIceLast, CIce* pIceStand, int nNumV, int nNumH,bool bBreakLast)
{
	bool bBreak = true;

	for (int i = 0; i < (int)apIceLast.size(); i++)
	{// �O�̕X�Ƌ��ʂ̕X�����Ă��邩�̃`�F�b�N���s��
		if (apIceLast[i] == nullptr)
			continue;

		for (int j = 0; j < (int)apIce.size(); j++)
		{
			if (apIce[j] == nullptr)
				continue;

			if (apIce[j] != apIceLast[i])
				continue;	// �����|�C���^���ǂ���

			if (apIce[j] == pIceStand)
			{// �����Ă���u���b�N�ɓ��������狭���I�ɐM����r��
				m_aGrid[nNumV][nNumH].pIce->EnableAliveStandIce(true);

				return false;
			}

			// ��ł����Ȃ��u���b�N����������j�󂵂Ȃ�����
			if (!apIceLast[i]->IsPeck() && !apIceLast[i]->IsBreak())
				bBreak = false;
		}
	}

#ifdef _DEBUG
	if (!bBreak)
	{
		CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
#endif

	return bBreak;
}

//=====================================================
// �˂������X�𒾂߂鏈��
//=====================================================
void CIceManager::BreakPeck(int nNumV, int nNumH)
{
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumIce = 0;
	int nNumPeck = 0;

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		nNumIce++;

		if (apIce[i]->IsPeck())
		{
			nNumPeck++;
			DeleteIce(apIce[i]);
			apIce[i]->EnableSink(true);
		}
	}

	if (nNumIce == nNumPeck)
	{
		if (m_aGrid[nNumV][nNumH].pIce != nullptr)
		{
			m_aGrid[nNumV][nNumH].pIce->Uninit();
			m_aGrid[nNumV][nNumH].pIce = nullptr;
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
D3DXVECTOR3 CIceManager::GetGridPosition(int *pNumV, int *pNumH)
{
	if (m_aGrid.empty())
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (*pNumV > (int)m_aGrid.size() - 1)
	{// �ォ���яo�Ă����̕␳
		*pNumV = m_aGrid.size() - 1;
	}
	else if (*pNumV < 0)
	{// �������яo�����̕␳
		*pNumV = 0;
	}

	if (*pNumH > (int)m_aGrid[*pNumV].size() - 1)
	{// �E�����яo�Ă����̕␳
		*pNumH = m_aGrid[*pNumV].size() - 1;
	}
	else if (*pNumH < 0)
	{// �������яo�����̕␳
		*pNumH = 0;
	}

	return m_aGrid[*pNumV][*pNumH].pos;
}

//=====================================================
// �O���b�h�I�u�W�F�N�g�̎擾
//=====================================================
CIce* CIceManager::GetGridObject(int* pNumV, int* pNumH)
{
	if (m_aGrid.empty())
		return nullptr;

	if (*pNumV > (int)m_aGrid.size() - 1)
	{// �ォ���яo�Ă����̕␳
		*pNumV = m_aGrid.size() - 1;
	}
	else if (*pNumV < 0)
	{// �������яo�����̕␳
		*pNumV = 0;
	}

	if (*pNumH > (int)m_aGrid[*pNumV].size() - 1)
	{// �E�����яo�Ă����̕␳
		*pNumH = m_aGrid[*pNumV].size() - 1;
	}
	else if (*pNumH < 0)
	{// �������яo�����̕␳
		*pNumH = 0;
	}

	return m_aGrid[*pNumV][*pNumH].pIce;
}

//=====================================================
// �ʒu����O���b�h�ԍ����擾���鏈��
//=====================================================
void CIceManager::GetIdxGridFromPosition(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH)
{
	if (pIdxV == nullptr || pIdxH == nullptr)
		return;

	//pos.z -= DEPTH_GRID * 0.5f;

	//*pIdxV = (int)((pos.z + DEPTH_GRID * m_nNumGridVirtical * 0.5f) / DEPTH_GRID);
	//
	//if(*pIdxV % 2 == 0)
	//	pos.x += WIDTH_GRID * 0.5f;

	//*pIdxH = (int)((pos.x + WIDTH_GRID * m_nNumGridVirtical * 0.5f) / WIDTH_GRID);

	pos.y = 10.0f;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// �����̌v�Z
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			D3DXVECTOR3 vecDiff = posGrid - pos;

			float fDist = D3DXVec3Length(&vecDiff);

			if (fDist < WIDTH_GRID * 0.5f)
			{// �X�̃T�C�Y���̔��a��菬�������������Ă锻��
				*pIdxV = i;
				*pIdxH = j;

				return;
			}
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CIceManager::Draw(void)
{

}