//*****************************************************
//
// �I�[�V��������[ocean.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"
#include "ocean.h"
#include "iceManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\BG\\field00.jpg"				// �e�N�X�`���t�@�C����
#define MOVE_SPEED					(1.0f)										// �ړ����x
#define MESH_LENGTH					(800.0f)									// ���b�V���̈�ӂ̒���
#define MESH_U						(254)											// ���̃u���b�N��
#define MESH_V						(254)											// �c�̃u���b�N��
#define SPLIT_TEX					(10)										// �e�N�X�`��������
#define CHENGE_LENGTH	(10000)	// ����ł��钸�_�܂ł̋���
#define ANGLE_SLIP	(0.7f)	// �������p�x
#define CMP_LENGTH	(1000.0f)	// ���肷�锼�a

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
COcean::COcean()
{
	m_fRot = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
COcean::~COcean()
{

}

//=====================================================
// ��������
//=====================================================
COcean* COcean::Create(void)
{
	if (m_pOcean == nullptr)
	{
		m_pOcean = new COcean;

		if (m_pOcean != nullptr)
		{
			m_pOcean->Init();
		}
	}

	return m_pOcean;
}

//=====================================================
// ����������
//=====================================================
HRESULT COcean::Init(void)
{
	CMeshField::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void COcean::Uninit(void)
{
	m_pOcean = nullptr;

	CMeshField::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void COcean::Update(void)
{
	float OceanFlowLevel = CIceManager::GetInstance()->GetOceanLevel();	//	�C�����x���̎擾

	CMeshField::Update();

	m_fRot += 0.007f * OceanFlowLevel;

	universal::LimitRot(&m_fRot);

	CMeshField::Wave(m_fRot);

}

//=====================================================
// �`�揈��
//=====================================================
void COcean::Draw(void)
{
	CMeshField::Draw();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

}