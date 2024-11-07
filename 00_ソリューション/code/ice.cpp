//*****************************************************
//
// �X�̏���[ice.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ice.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "iceHard.h"
#include "objectX.h"
#include "iceManager.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";	// �e�N�X�`���p�X
const float SIZE_INIT = 100.0f;	// �����T�C�Y
const float HEIGHT_ICE = 50.0f;	// �X�̍���
const int NUM_CORNER = 6;	// �p�̐�
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// �㑤�̏�������

const float TIME_REPAIR_ICE = 10.0f;	// �X�̏C���ɂ����鎞��
const int MAX_SCALE = 50; // �X�P�[���̍ő�l
const int MIN_SCALE = 20; // �X�P�[���̍ŏ��l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// �j�ЕX�̃��f���p�X
const float SPEED_SINK = 5.0f;	// ���ޑ��x
const float HEIGHT_DELETE = -100.0f;	// �폜����܂ł̍���

const float LINE_STOP_ICE = 1.0f;	// �X���~�܂邵�����l
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::m_Vector = {};	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIce::CIce(int nPriority) : CGameObject(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false), m_bAliveStandBlock(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr)
{
	s_nNumAll++;
	m_Vector.push_back(this);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// ��������
//=====================================================
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		default:
			assert(false);
			break;
		}

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// ����������
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// ���b�V���̐���
	CreateMesh();

	// �g�����X�t�H�[���̏����ݒ�
	SetTransform(SIZE_INIT);

	ChangeState(new CIceStaeteNormal);

	return S_OK;
}

//=====================================================
// ���b�V���̐���
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(3, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			m_pUp->SetRotation(ROT_UP_INIT);
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);
		}
	}

	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

	SetTransform(Grid::SIZE);
}

//=====================================================
// ���b�V���̔j��
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// �I������
//=====================================================
void CIce::Uninit(void)
{
	// ���b�V���̍폜
	DeleteMesh();

	for (auto itr = m_Vector.begin(); itr < m_Vector.end(); itr++)
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
		{
			continue;
		}
		//Vector����폜
		m_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIce::Update(void)
{	
	if (!IsSink())	// ���ރt���O�������Ă��Ȃ��Ƃ��̂ݍs��
		FollowWave();	// �g�ɒǏ]���鏈��

	if (m_pState != nullptr)
		m_pState->Update(this);
}

//=====================================================
// �g�ɒǏ]���鏈��
//=====================================================
void CIce::FollowWave(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
}

//=====================================================
// �`�揈��
//=====================================================
void CIce::Draw(void)
{

}

//=====================================================
// �g�����X�t�H�[���ݒ�
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// ��̐�|���S���̐ݒ�
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// �T�C�h�̃V�����_�[�̐ݒ�
	m_pSide->SetRadius(fRadius);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);
}

//=====================================================
// �X�e�C�g�̕ύX
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// �ʏ�X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{
	pIce->EnableStop(true);

	// �Y�����Ă���ԍ����擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);
}

//=====================================================
// �I������
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{
	// �ԍ��擾�Ɏ��s���Ă��邩�̊m�F
	bool bMove = CheckFailGetIndex(pIce);

	if(bMove)
		MoveToGrid(pIce);	// �O���b�h�Ɍ������Ĉړ����鏈��
}

//=====================================================
// �ԍ��擾�Ɏ��s���Ă��邩�̊m�F
//=====================================================
bool CIceStaeteNormal::CheckFailGetIndex(CIce *pIce)
{
	if (m_nIdxDriftV == -1 ||
		m_nIdxDriftH == -1)
	{// �ԍ��擾�Ɏ��s���Ă���ꍇ�A�Ď擾
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr == nullptr)
			return false;

		pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);

		if (m_nIdxDriftV == 0 && m_nIdxDriftH == 0)
		{
			int n = 0;
		}

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// ����ł����s�����ꍇ�A�U��Ԃ�
	}

	return true;
}

//=====================================================
// �O���b�h�Ɍ������Ĉړ����鏈��
//=====================================================
void CIceStaeteNormal::MoveToGrid(CIce *pIce)
{
	// �O���b�h�̈ʒu���擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	D3DXVECTOR3 posIce = pIce->GetPosition();

	// �����x�N�g����Y�����x�ɐ��K��
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVECTOR3 vecDiff = posGrid - posIce;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeedFlow;

	// �X�̈ʒu�Ɉړ��ʂ����Z
	pIce->AddPosition(vecDiff);
}

//*******************************************************************************
// ����X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// �X�̃��b�V�����폜
	pIce->DeleteMesh();
	
	// �����ŕX�j�󎞂̃G�t�F�N�g�𔭐�
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// �X�j�Ђ𐶐�
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// ���f���̊��蓖��
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// �ʒu�������_���ݒ�
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// �X�P�[���������_���ݒ�
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// ��]
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// �z��ɓ����
		m_aPeaceIce.push_back(pPeace);
	}

	// �J�E���^�[��������
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// �I������
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// �X�̏C���̍X�V
	UpdateRepair(pIce);

	// �X�̒Ǐ]
	FollowIce(pIce);
}

//=====================================================
// �C���̍X�V
//=====================================================
void CIceStaeteBreak::UpdateRepair(CIce *pIce)
{
	// ����ł����ԂȂ�ʂ�Ȃ�
	if (pIce->IsSink())
		return;

	// �^�C�}�[�����炷
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// ��莞�Ԍo�߂ŁA�X���C������
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}
}

//=====================================================
// �X�̒Ǐ]
//=====================================================
void CIceStaeteBreak::FollowIce(CIce *pIce)
{
	// �X�̒Ǐ]
	for (auto it : m_aPeaceIce)
	{
		D3DXVECTOR3 pos = { it->GetPosition().x, pIce->GetPosition().y, it->GetPosition().z };

		it->SetPosition(pos);
	}

	// ���ޏ�Ԃ̎��A�{�̂̈ʒu��������
	if (pIce->IsSink())
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();
		posIce.y -= SPEED_SINK;
		pIce->SetPosition(posIce);

		if (posIce.y < HEIGHT_DELETE)
		{// ���܂Œ��񂾂�폜����
			pIce->Uninit();

			return;
		}
	}
}

//*******************************************************************************
// �����X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStateFlow::Init(CIce *pIce)
{
	pIce->EnableStop(false);

	// ���g�̃|�C���^���O���b�h����O��
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->DeleteIce(pIce);
}

//=====================================================
// �I������
//=====================================================
void CIceStateFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStateFlow::Update(CIce *pIce)
{
	if (!m_bDrift)
		UpdateSarchIce(pIce);	// �X��T���Ă��鎞�̍X�V
	else
		UpdateDriftIce(pIce);	// �Y�����Ă�Ƃ��̍X�V
}

//=====================================================
// �X��T���Ă���Ƃ��̍X�V
//=====================================================
void CIceStateFlow::UpdateSarchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->AddPosition(vecStream);

	// �X�Ƃ̔���
	CollideIce(pIce);
}

//=====================================================
// �Y������Ƃ��̍X�V
//=====================================================
void CIceStateFlow::UpdateDriftIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �O���b�h�̈ʒu�擾
	D3DXVECTOR3 posDrift = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);

	// �C���̃x�N�g���擾
	D3DXVECTOR3 posIce = pIce->GetPosition();
	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	D3DXVECTOR3 vecDiff = posDrift - posIce;

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecDiff, &vecDiff);
	vecDiff *= fSpeedFlow;
	pIce->AddPosition(vecDiff);

	// �O���b�h�̈ʒu�Ƃ̋������������l�����������~�߂�
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
	{
		return;
	}
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CIceStateFlow::CollideIce(CIce *pIce)
{
	// ������O���b�h�̎擾
	int nIdxV = 0;
	int nIdxH = 0;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	if (bOk)
	{
		// �ԍ���ۑ�
		m_nIdxDriftV = nIdxV;
		m_nIdxDriftH = nIdxH;
	}

	// �C���̕����ɍ��킹������֐��̌Ăяo��
	DirectionFunc directionFuncs[CIceManager::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	CIceManager::E_Stream stream = pIceManager->GetDirStream();
	
	// �Y������X����������A�t���O�𗧂ĂĕY���O���b�h�ԍ���ۑ�
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH);

#ifdef _DEBUG
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	CEffect3D::Create(posGrid, 100.0f, 5, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		if (nIdxH == 0 && nIdxV == 0)
		{
			int n = 0;
		}

		// �O���b�h�ɕX����ۑ�
		pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce);
		pIce->ChangeState(new CIceStaeteNormal);
	}

#ifdef _DEBUG
	CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif
}

//=====================================================
// ������̊m�F
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �E�����̊m�F
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �������̊m�F
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �������̊m�F
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFT] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//*******************************************************************************
// ���ރX�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// �I������
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}