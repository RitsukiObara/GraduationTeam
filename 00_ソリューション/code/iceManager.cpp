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
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "manager.h"
#include "gameManager.h"
#include "player.h"
#include "inputmouse.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float RATE_HEX_X = 0.13f;	// �Z�p�`�̊���X
const float RATE_HEX_Z = 0.18f;	// �Z�p�`�̊���Z

const float WIDTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_X;	// �O���b�h�̕�
const float DEPTH_GRID = Grid::SIZE - Grid::SIZE * RATE_HEX_Z;	// �O���b�h�̉��s��
const float OCEAN_FLOW_MIN = 1.00f;								// �C���̑��x�ŏ�
const float OCEAN_FLOW_MAX = 5.00f;								// �C���̑��x�ő�

const float RANGE_SELECT_ICE = D3DX_PI;	// �X��I������Ƃ��̊p�x�͈̔�

const D3DXCOLOR COL_ICE[CIceManager::E_Pecker::PECKER_MAX] =	// �˂����l�ɂ��F
{
	D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
};
const D3DXCOLOR COL_ICE_DEFAULT = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�t�H���g�F

const float POW_CAMERAQUAKE_DEFAULT = 0.1f;		// �J�����̗h��̃f�t�H���g�l
const int FRAME_CAMERAQUAKE_DEFAULT = 30;		// �J�����̗h��̃f�t�H���g�t���[����
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority), m_nNumGridVirtical(0), m_nNumGridHorizontal(0), m_dirStream(COcean::STREAM_UP)
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

	// �C����������
	m_dirStream = COcean::STREAM_DOWN;
	m_dirStreamNext = COcean::STREAM_DOWN;
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
	// �����Ȓ�~�X�̌��o
	SearchInvailStopIce();

	// �X�̏�ԊǗ�
	ManageStateIce();
	
	// �����g�̑������蓖��
	BindRippleElements();

#ifdef _DEBUG
	Debug();
#endif
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
#ifdef _DEBUG
			if (i == 0 ||
				i == m_nNumGridVirtical - 1 ||
				j == 0 ||
				j == m_nNumGridHorizontal - 1)
			{
				CEffect3D::Create(m_aGrid[i][j].pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
#endif

			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// �������X�ɋ��܂�Ă邩�̔���
			if (JudgeBetweenPeck(i, j))
				continue;

			m_aGrid[i][j].pIce->EnableBreak(false);
			m_aGrid[i][j].pIce->EnableCanFind(true);
			m_aGrid[i][j].pIce->SetColor(COL_ICE_DEFAULT);
		}
	}
}

//=====================================================
// �����Ȓ�~�X�̌��o
//=====================================================
void CIceManager::SearchInvailStopIce(void)
{
	vector<CIce*> apIce;	// �����Ȓ�~�X�̔z��

	// �q�����ĂȂ��X�̌��o
	SearchNotConnectIce(apIce);

	for (auto it : apIce)	// �X�𗬂��ݒ�ɂ���
		it->ChangeState(new CIceStateFlow);
}

//=====================================================
// �q�����ĂȂ��X�̌��o
//=====================================================
void CIceManager::SearchNotConnectIce(vector<CIce*> &rpIce)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsCanPeck())
				continue;

			// ���Ȃ��u���b�N���s���M������
			DisableFromHardIce(i, j,false);
		}
	}

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (!m_aGrid[i][j].pIce->IsCanFind())
				continue;

			if (m_aGrid[i][j].pIce->IsPeck())
				continue;

			// �T���ς݂łȂ��X��ǉ�
			rpIce.push_back(m_aGrid[i][j].pIce);
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

	// ���鏈���̏�����
	pIce->StartFlash();

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
		pos.x += WIDTH_GRID * 0.5f;

	// �X�̃g�����X�t�H�[���ݒ�
	pIce->SetPosition(pos);
	pIce->SetTransform(Grid::SIZE);

	// ���鏈���̏�����
	pIce->StartFlash();

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
// �X�����邩�̃`�F�b�N
//=====================================================
bool CIceManager::CheckPeck(int nNumV, int nNumH, float fRot, D3DXVECTOR3 pos, E_Direction *pDir, CIce **ppIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	CIce* pIcePeck = nullptr;

	// �����ɍ��킹�ĕX��I��
	float fDiffMin = D3DX_PI * 2;
	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		// �X�ƃX�e�B�b�N�p�x�̔�r
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();
		// �����x�N�g���̊p�x���擾
		D3DXVECTOR3 vecDiff = posIce - pos;
		float fRotToTarget = atan2f(vecDiff.x, vecDiff.z);

		// �����p�x���͈͓����ǂ����擾
		float fRotDiff = fRotToTarget - fRot;
		universal::LimitRot(&fRotDiff);

		if (RANGE_SELECT_ICE * RANGE_SELECT_ICE > fRotDiff * fRotDiff)
		{// �Œ���I�ׂ�X�̔���
			if (fRotDiff * fRotDiff < fDiffMin * fDiffMin)
			{// �ŏ��̊p�x�Ȃ̂�ۑ�
				fDiffMin = fRotDiff;
				pIcePeck = apIce[i];

				if (pDir != nullptr)	// �ԍ��ۑ�
					*pDir = (E_Direction)i;

				*ppIce = pIcePeck;	// �I��ł�X�ۑ�
			}
		}
	}

	// �ԍ����擾
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// �˂�����X���̃`�F�b�N
	return CanPeck(pIcePeck, nNumBreakV, nNumBreakH);
}

//=====================================================
// �X����
//=====================================================
bool CIceManager::PeckIce(int nNumV, int nNumH, float fRot,D3DXVECTOR3 pos, bool *pResultBreak)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;
	vector<CIce*> apIce = GetAroundIce(nNumV, nNumH);

	int nNumBreakV = nNumV;
	int nNumBreakH = nNumH;

	CIce* pIcePeck = nullptr;

	// �����ɍ��킹�ĕX��I��
	float fDiffMin = D3DX_PI * 2;

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		// �X�ƃX�e�B�b�N�p�x�̔�r
		D3DXVECTOR3 posIce = it->GetPosition();

		// �����x�N�g���̊p�x���擾
		D3DXVECTOR3 vecDiff = posIce - pos;
		float fRotToTarget = atan2f(vecDiff.x, vecDiff.z);

		// �����p�x���͈͓����ǂ����擾
		float fRotDiff = fRotToTarget - fRot;
		universal::LimitRot(&fRotDiff);

		if (RANGE_SELECT_ICE * RANGE_SELECT_ICE > fRotDiff * fRotDiff)
		{// �Œ���I�ׂ�X�̔���
			if (fRotDiff * fRotDiff < fDiffMin * fDiffMin)
			{
				fDiffMin = fRotDiff;
				pIcePeck = it;
			}
		}
	}

	if (pIcePeck == nullptr)
		return false;

	// �ԍ����擾
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// �˂�����X���̃`�F�b�N
	if (!CanPeck(pIcePeck, nNumBreakV, nNumBreakH))
		return false;

	// �X��˂���������ɂ���
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
		CSound::GetInstance()->Play(CSound::LABEL_SE_BREAK_ICE);
	}

	// �X�T���̍ċA�֐�
	FindIce(nNumBreakV, nNumBreakH, 0, pIceStand, apIce,false);

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
			DisableFromHardIce(i, j);
		}
	}

	// �T���t���O�̖�����
	DisableFind();

	// ����u���b�N���܂Ƃ܂�ɂ���
	SummarizeIce(nNumBreakV, nNumBreakH);

	// �X������t���O�������Ă�����X����
	bool bResultBreak = BreakIce();

	if (pResultBreak != nullptr)
		*pResultBreak = bResultBreak;

	return true;
}

//=====================================================
// �ԍ��ł�������
//=====================================================
bool CIceManager::PeckIce(int nIdxV, int nIdxH)
{
	if (nIdxV < 0 || nIdxV >= m_nNumGridVirtical ||
		nIdxH < 0 || nIdxH >= m_nNumGridHorizontal)
		return false;

	vector<CIce*> apIce = GetAroundIce(nIdxV, nIdxH);

	int nNumBreakV = nIdxV;
	int nNumBreakH = nIdxH;

	CIce* pIcePeck = m_aGrid[nIdxV][nIdxH].pIce;

	if (pIcePeck == nullptr)
		return false;

	// �ԍ����擾
	GetIceIndex(pIcePeck, &nNumBreakV, &nNumBreakH);

	// �˂�����X���̃`�F�b�N
	if (!CanPeck(pIcePeck, nNumBreakV, nNumBreakH))
		return false;

	// �X��˂���������ɂ���
	if (pIcePeck)
	{
		pIcePeck->EnablePeck(true);
		pIcePeck->ChangeState(new CIceStaeteBreak);
		CSound::GetInstance()->Play(CSound::LABEL_SE_BREAK_ICE);
	}

	// �X�T���̍ċA�֐�
	FindIce(nNumBreakV, nNumBreakH, 0, pIcePeck, apIce, false);

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
			DisableFromHardIce(i, j);
		}
	}

	// �T���t���O�̖�����
	DisableFind();

	// ����u���b�N���܂Ƃ܂�ɂ���
	SummarizeIce(nNumBreakV, nNumBreakH);

	// �X������t���O�������Ă�����X����
	BreakIce();

	return true;
}

//=====================================================
// �˂�����X���̃`�F�b�N
//=====================================================
bool CIceManager::CanPeck(CIce* pIce, int nNumV, int nNumH)
{
	if (pIce == nullptr)
		return false;	// �k����������˂��Ȃ�

	if (!pIce->IsCanPeck())
		return false;	// �˂����Ȃ��u���b�N�Ȃ�˂��Ȃ�

	if (pIce->IsPeck())
		return false;	// ���ɓ˂����Ă�����˂��Ȃ�

	// �Ȃɂ��������Ă���˂��Ȃ�
	if (pIce->IsOnTopAnyObject())
		return false;

	return true;
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

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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
void CIceManager::Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH, float fRate)
{
	if (nIdxV < 0 || nIdxV >= m_nNumGridVirtical ||
		nIdxH < 0 || nIdxH >= m_nNumGridHorizontal)
		return;

	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = m_aGrid[nIdxV][nIdxH].pos;

	universal::LimitDistCylinderInSide(WIDTH_GRID * fRate, pPos, posGrid);
}

//=====================================================
// �O�ɏo���Ȃ��悤�ɂ��锻��
//=====================================================
void CIceManager::Collide(D3DXVECTOR3 *pPos, CIce *pIce, float fRate)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIce->GetPosition();

	universal::LimitDistCylinderInSide(WIDTH_GRID * fRate, pPos, posGrid);
}

//=====================================================
// �X���Ɏ��߂鏈��
//=====================================================
void CIceManager::LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (pPos == nullptr)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	D3DXVECTOR3 posIce = m_aGrid[nNumV][nNumH].pIce->GetPosition();

	pPos->y = posIce.y;
}

//=====================================================
// �ł��߂��X�̎擾
//=====================================================
CIce *CIceManager::GetNearestIce(D3DXVECTOR3 pos, int *pNumV, int *pNumH)
{
	float fDistMin = FLT_MAX;
	CIce *pIceNearest = nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			if (m_aGrid[i][j].pIce->IsPeck())
				continue;

			CIce *pIce = m_aGrid[i][j].pIce;

			float fDiff = 0.0f;

			if (universal::DistCmpFlat(pos, pIce->GetPosition(), fDistMin, &fDiff))
			{
				fDistMin = fDiff;

				pIceNearest = pIce;

				if (pNumV != nullptr && pNumH != nullptr)
				{
					*pNumV = i;
					*pNumH = j;
				}
			}
		}
	}

	return pIceNearest;
}

//=====================================================
// �X�̒T��
//=====================================================
bool CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand, vector<CIce*> apIceLast, bool bBreakLast)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

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

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (!apIce[i]->IsCanPeck())
			continue;

		FindIce(aV[i], aH[i], nIdx, pIceStand, apIce, true);
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
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, pIce->GetPosition());
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, pIce->GetPosition());
		pIce->Uninit();
		return;
	}

	bool bOk = SetIceInGrid(nIdxV, nIdxH, pIce);

	if (!bOk)
	{// �O���b�h�ɖ����������
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, pIce->GetPosition());
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, pIce->GetPosition());
		DeleteIce(pIce);
		pIce->Uninit();
	}
}

//=====================================================
// �d���X����M�����o���āA�j��M��������
//=====================================================
void CIceManager::DisableFromHardIce(int nNumV, int nNumH, bool bPeck)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

	if (m_aGrid[nNumV][nNumH].pIce == nullptr)
		return;

	// �T���ς݃t���O�𗧂Ă�
	m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
	m_aGrid[nNumV][nNumH].pIce->EnableBreak(false);

	// ���ӃO���b�h�̌v�Z
	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};
	Grid::CalcAroundGrids(nNumV, nNumH, aV, aH);

	vector<CIce*> apIce(DIRECTION_MAX);

	// �X�̃|�C���^�̕ۑ�
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (bPeck)
		{// �����u���b�N���܂߂邩�ǂ���
			if (apIce[i]->IsPeck())
				continue;
		}

		DisableBreak(aV[i], aH[i]);
	}
}

//=====================================================
// �v���C���[����M�����˂Ŕj��M������
//=====================================================
void CIceManager::DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck, vector<CIce*> apIce)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

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

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

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

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

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

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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

		if (!apIce[i]->IsCanPeck())
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
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

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

		if (!universal::LimitValue(&nV, m_nNumGridVirtical - 1, 0) &&
			!universal::LimitValue(&nH, m_nNumGridHorizontal - 1, 0))
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
bool CIceManager::BreakIce(void)
{
	bool bBreakAny = false;

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

			bBreakAny = true;
		}
	}

	if (bBreakAny)
	{// �J������h�炷
		CCamera *pCamera = CManager::GetCamera();
		
		if (pCamera != nullptr)
			pCamera->SetQuake(POW_CAMERAQUAKE_DEFAULT, POW_CAMERAQUAKE_DEFAULT, FRAME_CAMERAQUAKE_DEFAULT);
	}

	return bBreakAny;
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
// �˂������X�𒾂߂鏈��
//=====================================================
void CIceManager::BreakPeck(int nNumV, int nNumH)
{
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return;

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

			// ���݃p�[�e�B�N���̔���
			D3DXVECTOR3 posIce = apIce[i]->GetPosition();
			CParticle::Create(posIce, CParticle::TYPE::TYPE_BUBBLE_SINK);
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
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n�X�̏��=====================");
	pDebugProc->Print("\n�X�̑���[%d]", CIce::GetNumAll());

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	// �C���̌�����ύX
	if (pKeyboard->GetTrigger(DIK_LEFT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		m_dirStreamNext = (COcean::E_Stream)((m_dirStreamNext + 1) % COcean::E_Stream::STREAM_MAX);	// ���̊C���̌����ɂ���
	}

	if (pKeyboard->GetTrigger(DIK_RIGHT))
	{
		COcean* pOcean = COcean::GetInstance();
		pOcean->SetOceanSpeedState(pOcean->OCEAN_STATE_DOWN);	// �C���̑��x��������
		m_dirStreamNext = (COcean::E_Stream)((m_dirStreamNext + COcean::E_Stream::STREAM_MAX - 1) % COcean::E_Stream::STREAM_MAX);	// ���̊C���̌����ɂ���
	}

	pDebugProc->Print("\n���݂̊C���̌���[%d]", m_dirStreamNext);

	//-------------------------------------
	// �f�o�b�O�ŕX��������
	//-------------------------------------
	CInputMouse *pMouse = CInputMouse::GetInstance();

	if (pMouse == nullptr)
		return;

	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	std::map<CObject3D*, int> mapIcon;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			D3DXVECTOR3 posIce = m_aGrid[i][j].pIce->GetPosition();

			bool bHit = universal::CalcRaySphere(posNear, vecDiff, posIce, Grid::SIZE * 0.5f);

			if (!bHit)
				continue;

			debug::Effect3DShort(posIce, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

			if (pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
			{// �N���b�N������p�[�c�ԍ��̌���
				PeckIce(i, j);
			}
		}
	}
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

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// �����̌v�Z
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			pos.y = posGrid.y;

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
	if (pIce == nullptr)
		return false;

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
	if (nNumV < 0 || nNumV >= m_nNumGridVirtical ||
		nNumH < 0 || nNumH >= m_nNumGridHorizontal)
		return false;

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
// �����̕X���擾
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
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// �Ȃɂ�����Ă���L�����Z��

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// �����Ă���L�����Z��

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// ����̕X���擾
//=====================================================
CIce* CIceManager::GetLeftUpIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = m_nNumGridVirtical - 1; i >= 0; i--)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// �X����������ԍ���ۑ�
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// �Ȃɂ�����Ă���L�����Z��

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// �����Ă���L�����Z��

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// �E���̕X���擾
//=====================================================
CIce* CIceManager::GetRightDownIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = m_nNumGridHorizontal - 1; j >= 0; j--)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// �X����������ԍ���ۑ�
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// �Ȃɂ�����Ă���L�����Z��

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// �����Ă���L�����Z��

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// �E��̕X���擾
//=====================================================
CIce* CIceManager::GetRightUpIdx(int *pNumV, int *pNumH)
{
	if (pNumV == nullptr || pNumH == nullptr)
		return nullptr;

	for (int i = m_nNumGridVirtical - 1; i >= 0; i--)
	{
		for (int j = m_nNumGridHorizontal - 1; j >= 0; j--)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{// �X����������ԍ���ۑ�
				if (m_aGrid[i][j].pIce->IsOnTopAnyObject())
					continue;	// �Ȃɂ�����Ă���L�����Z��

				if (m_aGrid[i][j].pIce->IsPeck())
					continue;	// �����Ă���L�����Z��

				*pNumV = i;
				*pNumH = j;
				return m_aGrid[i][j].pIce;
			}
		}
	}

	return nullptr;
}

//=====================================================
// �O���b�h�����̔���
//=====================================================
bool CIceManager::IsInGrid(D3DXVECTOR3 pos, float fRate)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			// �����̌v�Z
			D3DXVECTOR3 posGrid = m_aGrid[i][j].pos;

			pos.y = posGrid.y;

			D3DXVECTOR3 vecDiff = posGrid - pos;

			float fDist = D3DXVec3Length(&vecDiff);

			if (fDist < WIDTH_GRID * fRate)
			{// �X�̃T�C�Y���̔��a��菬�������������Ă锻��
				return true;
			}
		}
	}

	return false;
}

//=====================================================
// �����_���ȕX�̎擾
//=====================================================
CIce* CIceManager::GetRandomIce(int *pNumV, int *pNumH)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return nullptr;

	// �~�܂��ĂȂ��X�����O
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && ice->IsPeck(); });

	// �T�C�Y���烉���_���ŕX���w��
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

	// �ԍ��̕ۑ�
	if (pNumV != nullptr && pNumH != nullptr)
	{
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr != nullptr)
		{
			pIceMgr->GetIceIndex(pIce, pNumV, pNumH);
		}
	}

	return pIce;
}

//=====================================================
// �`�揈��
//=====================================================
void CIceManager::Draw(void)
{

}

//=====================================================
// �����z�u�ǂݍ��ݏ���
//=====================================================
void CIceManager::Load(const char* pPath)
{
	bool bMulti = gameManager::IsMulti();	// �}���`�t���O�擾

	bool bLoad = false;
	int nGridV = 0;
	std::ifstream ifs(pPath);

	if (ifs.is_open())
	{
		std::string strLine;
		while (std::getline(ifs, strLine))
		{
			std::istringstream iss(strLine);
			std::string key;
			iss >> key;
			
			if (strLine.length() == 0)
				continue;	// �ǂݍ��񂾕�������Ȃ�ʂ�Ȃ�

			if (key == "SETICE")
			{
				bLoad = true;
				continue;
			}

			if (key == "END_SETICE")
			{// �I��
				break;
			}

			// �z�u�ǂݍ���
			if (bLoad)
			{// SETICE�ǂݍ���
				int nGridH = 0;
				for (int cnt = 0; cnt < (int)strLine.size(); cnt++)
				{
					char cData = strLine[cnt];
					if (cData != ' ')
					{// �������琔��������
						if (cData == '1')
						{// �ʏ�X
							CreateIce(nGridV, nGridH);
						}
						else if (cData == '2')
						{// �d���X
							if(bMulti)	// �}���`�p�̍d���X
								CreateIce(nGridV, nGridH, CIce::E_Type::TYPE_HARDMULTI);
							else	// �V���O���p�̍d���X
								CreateIce(nGridV, nGridH, CIce::E_Type::TYPE_HARD);
						}
						nGridH++;
					}
				}
				nGridV++;	// �s�J�E���g���₷
			}
		}
		ifs.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}
}

//=====================================================
// �����g�̑��������蓖�Ă鏈��
//=====================================================
void CIceManager::BindRippleElements(void)
{
	// �S�X�̃`�F�b�N
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce == nullptr)
				continue;

			// ���[�̌��o
			if (j == 0)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_RIGHT, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_RIGHT, m_aGrid[i][j - 1].pIce == nullptr);

			// �E�[�̌��o
			if (j == m_nNumGridHorizontal - 1)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_LEFT, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_LEFT, m_aGrid[i][j + 1].pIce == nullptr);

			// ���[�̌��o
			if (i == 0)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_UP, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_UP, m_aGrid[i - 1][j].pIce == nullptr);

			// ��[�̌��o
			if (i == m_nNumGridVirtical - 1)
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_DOWN, true);
			else
				m_aGrid[i][j].pIce->SetRippleFrag(COcean::E_Stream::STREAM_DOWN, m_aGrid[i + 1][j].pIce == nullptr);
		}
	}
}