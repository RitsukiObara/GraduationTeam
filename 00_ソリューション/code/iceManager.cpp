//*****************************************************
//
// �X�}�l�[�W���[[IceManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "particle.h"
#include "flowIce.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "meshfield.h"
#include "ocean.h"
#include "polygon3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// �Z�p�`�̊���X
const float RATE_HEX_Z = 0.13f;	// �Z�p�`�̊���Z

const float WIDTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_X;	// �O���b�h�̕�
const float DEPTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_Z;	// �O���b�h�̉��s��
const float OCEAN_FLOW_MIN = 1.00f;		// �C���̑��x�ŏ�
const float OCEAN_FLOW_MAX = 5.00f;	// �C���̑��x�ő�

const float RANGE_SELECT_ICE = D3DX_PI / 6;	// �X��I������Ƃ��̊p�x�͈̔�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority), m_nNumGridVirtical(0), m_nNumGridHorizontal(0), m_dirStream(E_Stream::STREAM_UP)
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
	/*CreateIce(3, 3);
	CreateIce(4, 3);
	CreateIce(5, 3);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(4, 6);
	CreateIce(6, 6);
	CreateIce(6, 7);
	CreateIce(6, 8);
	CreateIce(6, 9);*/

	// �C����������
	m_dirStream = E_Stream::STREAM_LEFT;
	m_dirStreamNext = E_Stream::STREAM_LEFT;
	m_fOceanLevel = OCEAN_FLOW_MAX;

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

#ifdef _DEBUG
			//CPolygon3D *pPolygon = CPolygon3D::Create(pos);
			
			//pPolygon->SetMode(CPolygon3D::MODE::MODE_BILLBOARD);
			//pPolygon->SetSize(WIDTH_GRID * 0.2f, WIDTH_GRID * 0.2f);
			//pPolygon->SetVtx();
#endif
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

	return pIce;
}

//=====================================================
// ���X�̐���
//=====================================================
CIce *CIceManager::CreateFlowIce(int nGridV, int nGridH, CIce::E_Type type)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create(type);

	if (pIce == nullptr)
		return nullptr;

	D3DXVECTOR3 pos;
	pos = { nGridH * WIDTH_GRID - WIDTH_GRID * m_nNumGridHorizontal * 0.5f,10.0f,nGridV * DEPTH_GRID - DEPTH_GRID * m_nNumGridVirtical * 0.5f };

	// �����s�������炸�炷
	if (nGridV % 2 == 0)
	{
		pos.x += WIDTH_GRID * 0.5f;
	}

	// �X�̃g�����X�t�H�[���ݒ�
	pIce->SetPosition(pos);
	pIce->SetTransform(Grid::SIZE);

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
void CIceManager::PeckIce(int nNumV, int nNumH, float fRot,D3DXVECTOR3 pos)
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

	CIce* pIcePeck = nullptr;

	// �����ɍ��킹�ĕX��I��
	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		// �X�ƃX�e�B�b�N�p�x�̔�r
		D3DXVECTOR3 posIce = it->GetPosition();
		bool bSelect = universal::IsInFanTargetYFlat(pos, posIce, fRot, RANGE_SELECT_ICE);

		if (bSelect)
		{// �X���I�ׂ���for�����I��
			pIcePeck = it;
			break;
		}
	}

	// �ԍ����擾
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// �X���������Ԃ��̃`�F�b�N
	if (pIcePeck == nullptr)
		return;

	if (!pIcePeck->IsCanPeck())
		return;	// �˂����Ȃ��u���b�N�Ȃ��̏�����ʂ�Ȃ�

	if (pIcePeck->IsPeck())
		return;	// ���ɓ˂����Ă�����ʂ�Ȃ�

	// �X��˂���������ɂ���
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
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
	DisableFromPlayer(nNumV, nNumH, pIcePeck, apIce);

	// �T���t���O�̖�����
	DisableFind();

	// ����u���b�N���܂Ƃ܂�ɂ���
	SummarizeIce(nNumBreakV, nNumBreakH);

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
void CIceManager::Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = m_aGrid[nIdxV][nIdxH].pos;

	universal::LimitDistCylinderInSide(WIDTH_GRID * 0.7f, pPos, posGrid);
}

//=====================================================
// �O�ɏo���Ȃ��悤�ɂ��锻��
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos, CIce *pIce)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIce->GetPosition();

	universal::LimitDistCylinderInSide(WIDTH_GRID * 0.7f, pPos, posGrid);
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
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// �ċA�֐��̐[�����C���N�������g
	nIdx++;

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

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
	int nIdxV = -1;
	int nIdxH = -1;
	D3DXVECTOR3 posIce = pIce->GetPosition();

	GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	if (nIdxV == -1 && nIdxH == -1)
	{
		pIce->Uninit();
		return;
	}

	bool bOk = SetIceInGrid(nIdxV, nIdxH, pIce);

	if (!bOk)
	{
		DeleteIce(pIce);
		pIce->Uninit();
	}
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
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

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
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

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
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

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
// �X���܂Ƃ߂�
//=====================================================
void CIceManager::SummarizeIce(int nNumV, int nNumH)
{
	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

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

		if (!apIce[i]->IsBreak())
			continue;

		// ���X�V�X�e���̐���
		CFlowIce *pFlowIce = CFlowIce::Create();

		// �ċA�֐��ŘA�����ĕX�𗬕X�V�X�e���ɕۑ�
		SaveFlowIce(aV[i], aH[i], pFlowIce);
	}
}

//=====================================================
// �X�𗬕X�ɕۑ�����V�X�e��
//=====================================================
void CIceManager::SaveFlowIce(int nNumV, int nNumH, CFlowIce *pFlowIce)
{
	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(DIRECTION_MAX);
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	// ���ӂ̃O���b�h�ԍ��̌v�Z
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	// �T���ς݂̃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

	// ���g�𗬕X�ɒǉ�
	pFlowIce->AddIceToArray(m_aGrid[nNumV][nNumH].pIce);

#ifdef _DEBUG
	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 100, 120, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

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
		
		if (!apIce[i]->IsBreak())
			continue;

		if (!apIce[i]->IsCanFind())
			continue;

		if (apIce[i]->IsPeck())
			continue;

		pFlowIce->AddIceToArray(apIce[i]);

		// �ċA�֐��ŘA�����ĕX�𗬕X�V�X�e���ɕۑ�
		SaveFlowIce(aV[i], aH[i], pFlowIce);
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

			m_aGrid[i][j].pIce->ChangeState(new CIceStateFlow);
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

	//CEffect3D::Create(m_aGrid[nNumV][nNumH].pos, 300.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

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
		//CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
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
	{// �������Ă���X���S�Ă��������̂��������
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
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n�X�̑���[%d]", CIce::GetNumAll());

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	// �C���̌�����ύX
	if (pKeyboard->GetTrigger(DIK_LEFT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		m_dirStreamNext = (E_Stream)((m_dirStreamNext + 1) % E_Stream::STREAM_MAX);	// ���̊C���̌����ɂ���
	}

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		m_dirStreamNext = (E_Stream)((m_dirStreamNext + E_Stream::STREAM_MAX - 1) % E_Stream::STREAM_MAX);	// ���̊C���̌����ɂ���
	}

	pDebugProc->Print("\n���݂̊C���̌���[%d]", m_dirStreamNext);
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
CIce* CIceManager::GetGridIce(int* pNumV, int* pNumH)
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
// �����Ă�X�̎擾
//=====================================================
vector<CIce*> CIceManager::GetFlows(void)
{
	vector<CIce*> apIceFlows;
	vector<CIce*> apIce = CIce::GetInstance();

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		if (!it->IsStop())	// ��~���Ă��Ȃ���Εۑ�
			apIceFlows.push_back(it);
	}

	return apIceFlows;
}

//=====================================================
// �ʒu����O���b�h�ԍ����擾���鏈��
//=====================================================
bool CIceManager::GetIdxGridFromPosition(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH, float fRate)
{
	if (pIdxV == nullptr || pIdxH == nullptr)
		return false;

	pos.y = 10.0f;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// �����̌v�Z
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			D3DXVECTOR3 vecDiff = posGrid - pos;

			float fDist = D3DXVec3Length(&vecDiff);

			if (fDist < WIDTH_GRID * fRate)
			{// �X�̃T�C�Y���̔��a��菬�������������Ă锻��
				*pIdxV = i;
				*pIdxH = j;

				return true;
			}
		}
	}

	return false;
}

//=====================================================
// �X������
//=====================================================
bool CIceManager::IsInIce(D3DXVECTOR3 pos, CIce *pIce, float fRate)
{
	// �����̌v�Z
	D3DXVECTOR3 posIce = pIce->GetPosition();

	D3DXVECTOR3 vecDiff = posIce - pos;
	vecDiff.y = 0.0f;

	float fDist = D3DXVec3Length(&vecDiff);

	if (fDist < WIDTH_GRID * fRate)
	{// �X�̃T�C�Y���̔��a��菬�������������Ă锻��
		return true;
	}

	return false;
}

//=====================================================
// �O���b�h�ɕX��ݒ�
//=====================================================
bool CIceManager::SetIceInGrid(int nNumV, int nNumH, CIce *pIce)
{
	if (m_aGrid.empty())
		return false;

	if (nNumV > (int)m_aGrid.size() - 1)
	{// �ォ���яo�Ă����̐���
		return false;
	}
	else if (nNumV < 0)
	{// �������яo�����̐���
		return false;
	}

	if (nNumH > (int)m_aGrid[nNumH].size() - 1)
	{// �E�����яo�Ă����̐���
		return false;
	}
	else if (nNumH < 0)
	{// �������яo�����̐���
		return false;
	}

	if (m_aGrid[nNumV][nNumH].pIce == nullptr || m_aGrid[nNumV][nNumH].pIce == pIce)
	{
		m_aGrid[nNumV][nNumH].pIce = pIce;

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================
// �X�̃O���b�h�ԍ����擾
//=====================================================
void CIceManager::GetIceIndex(CIce *pIce, int *pNumV, int *pNumH)
{
	if (pIce == nullptr || pNumV == nullptr || pNumV == nullptr)
		return;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (pIce == m_aGrid[i][j].pIce)
			{// �����X�̃|�C���^�������ꍇ�A�ԍ���ۑ����Ċ֐����I��
				*pNumV = i;
				*pNumH = j;
				return;
			}
		}
	}

	// �ǂ̃|�C���^�ɂ�����Ȃ������ꍇ��-1��Ԃ�
	*pNumV = -1;
	*pNumH = -1;
}

//=====================================================
// �E���̕X���擾
//=====================================================
CIce* CIceManager::GetLeftDownIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// �X����������ԍ���ۑ�
				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// �`�揈��
//=====================================================
void CIceManager::Draw(void)
{

}