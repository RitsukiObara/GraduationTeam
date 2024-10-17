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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_FLOWS = 1.0f;	// ����鑬�x
const string PATH_TEX = "data\\TEXTURE\\UI\\ice.png";	// �e�N�X�`���p�X
const float SIZE_INIT = 100.0f;	// �����T�C�Y
const float HEIGHT_ICE = 50.0f;	// �X�̍���
const int NUM_CORNER = 6;	// �p�̐�
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// �㑤�̏�������

const float TIME_REPAIR_ICE = 10.0f;	// �X�̏C���ɂ����鎞��
const int MAX_SCALE = 50; // �X�P�[���̍ő�l
const int MIN_SCALE = 20; // �X�P�[���̍ŏ��l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_small.x";	// �j�ЕX�̃��f���p�X
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
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr)
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
			m_pUp->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (m_pSide == nullptr)
		m_pSide = CMeshCylinder::Create(NUM_CORNER);

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
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (COcean::GetInstance() == nullptr)
	{
		return;
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = COcean::GetInstance()->GetHeight(pos,&move) + HEIGHT_ICE;

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
// ����鏈��
//=====================================================
void CIce::Flows(void)
{
	AddPosition(D3DXVECTOR3(SPEED_FLOWS, 0.0f, 0.0f));
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
	// �^�C�}�[�����炷
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// ��莞�Ԍo�߂ŁA�X���C������
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}
}

//*******************************************************************************
// �����X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteFlow::Init(CIce *pIce)
{

}

//=====================================================
// �I������
//=====================================================
void CIceStaeteFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteFlow::Update(CIce *pIce)
{
	pIce->AddPosition(D3DXVECTOR3(-SPEED_FLOWS, 0.0f, 0.0f));
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