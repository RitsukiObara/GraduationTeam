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
m_pSide(nullptr),m_pUp(nullptr)
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
}

//=====================================================
// �I������
//=====================================================
void CIce::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);

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

	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIce::Update(void)
{
	if (m_state == E_State::STATE_FLOWS)
	{// ������Ԃł͈ړ��𑱂���
		Flows();
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = COcean::GetInstance()->GetHeight(pos,&move);

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos + D3DXVECTOR3(0.0f, HEIGHT_ICE, 0.0f));
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos);
	}
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