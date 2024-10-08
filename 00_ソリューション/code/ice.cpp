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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_FLOWS = 1.0f;	// ����鑬�x
const string PATH_TEX = "data\\TEXTURE\\UI\\ice.png";	// �e�N�X�`���p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CIce::s_nNumAll = 0;

//=====================================================
// �R���X�g���N�^
//=====================================================
CIce::CIce(int nPriority) : CPolygon3D(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false)
{
	s_nNumAll++;
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
CIce *CIce::Create(E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		pIce = new CIce;

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
	// �p���N���X�̏�����
	CPolygon3D::Init();

	int nIdx = Texture::GetIdx(&PATH_TEX[0]);
	SetIdxTexture(nIdx);

	EnableCanFind(true);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CIce::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIce::Update(void)
{
	// �p���N���X�̍X�V
	CPolygon3D::Update();

	if (m_state == E_State::STATE_FLOWS)
	{// ������Ԃł͈ړ��𑱂���
		Flows();
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
	// �p���N���X�̕`��
	CPolygon3D::Draw();
}