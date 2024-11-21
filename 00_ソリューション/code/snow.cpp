//*****************************************************
//
// �Ꮘ��[snow.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "snow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "slow.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(7.0f)	// �ړ����x

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float SIZE_RATIO = 0.01f;	// �T�C�Y�{��
	const int MAX_ROT_MOVE = 1;		// �ő�������x
	const int MIN_ROT_MOVE = -3;	// �Œ�������x
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSnow::CSnow(int nPriority) : CPolygon3D(nPriority)
{
	m_nLife = 0;
	m_move = { 0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSnow::~CSnow()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSnow::Init(void)
{
	// �p���N���X�̏�����
	CPolygon3D::Init();

	SetMode(CPolygon3D::MODE_BILLBOARD);
	EnableZtest(true);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSnow::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSnow::Update(void)
{
	// �p���N���X�̍X�V
	CPolygon3D::Update();

	// ��������
	m_nLife--;

	float fWidth = GetWidth();
	float fHeight = GetHeight();

	D3DXVECTOR3 pos = GetPosition();

	CSlow* pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		// �ʒu�̍X�V
		pos += m_move * fScale;
	}
	else
	{
		// �ʒu�̍X�V
		pos += m_move;
	}

	// �ʒu�X�V
	SetPosition(pos);

	// ��������
	Move();

	if (m_nLife < 0)
	{// �����̍폜
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSnow::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CPolygon3D::Draw();

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// ��������
//=====================================================
CSnow* CSnow::Create(D3DXVECTOR3 pos, float fRadius, int nLife,D3DXVECTOR3 move)
{
	CSnow* pEffect3D = nullptr;

	if (pEffect3D == nullptr)
	{// �C���X�^���X����
		pEffect3D = new CSnow;

		if (pEffect3D != nullptr)
		{
			// ����������
			pEffect3D->Init();

			pEffect3D->SetPosition(pos);
			pEffect3D->SetSize(fRadius, fRadius);

			pEffect3D->SetMode(MODE_NORMAL);

			// �e�N�X�`���̓Ǎ�
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\snow.png");
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->SetVtx();
		}
	}

	return pEffect3D;
}

//=====================================================
// ��������
//=====================================================
void CSnow::Move(void)
{
	D3DXVECTOR3 rot = GetRotation();

	// �����̓����ݒ�
	float rot_move = (float)universal::RandRange(MAX_ROT_MOVE, MIN_ROT_MOVE) * SIZE_RATIO;

	// �ړ���
	rot.x += rot_move;
	//rot.y += rot_move;
	/*rot.z += rot_move;*/

	// �����̃m�[�}���C�Y
	universal::LimitRot(&rot.x);
	//universal::LimitRot(&rot.y);
	/*universal::LimitRot(&rot.z);*/

	// �����X�V
	SetRotation(rot);

	// ���_���ݒ�
	SetVtx();
}