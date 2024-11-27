//*****************************************************
//
// �t�F�[�h�����i�X�������Ă���j[fade_fallice.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fade_fallice.h"
#include "blur.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
	std::vector<std::vector<POINT>> FALLICE_PATTERN =
	{
		{// 1
			{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0}
		},
		{// 2
			{0,1},{1,1},{2,1},{4,1},{5,1},{6,1},
		},
		{// 3
			{0,2},{1,2},{2,2},{3,1},{5,2},{6,2},{7,2},
		},
		{// 4
			{0,3},{1,3},{3,2},{4,2},{5,3},{6,3},
		},
		{// 5
			{0,4},{1,4},{2,3},{4,3},{6,4},{7,4},
		},
		{// 6
			{0,5},{2,4},{3,3},{5,4},{6,5},
		},
		{// 7
			{1,5},{3,4},{4,4},{5,5},
		},
		{// 8
			{2,5},{4,5},
		},
		{// 9
			{3,5},
		},
	};
	const int FALLICE_COUNT = 10;
	const float POLYGON_WIDTH = 216.0f;
	const float POLYGON_HEIGHT = 250.0f;
	const char* FALLICE_TEX_PATH = "data\\TEXTURE\\UI\\Fade_FallIce_NoAnti.png";
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CFade_FallIce* CFade_FallIce::m_pFade = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CFade_FallIce::CFade_FallIce()
{
	m_nCounterFrame = 0;
	m_nCounterPattern = 0;
	m_pTexture = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFade_FallIce::~CFade_FallIce()
{

}

//=====================================================
// ��������
//=====================================================
CFade_FallIce* CFade_FallIce::Create(void)
{
	if (m_pFade == nullptr)
	{
		m_pFade = new CFade_FallIce;

		if (m_pFade != nullptr)
		{
			m_pFade->Init();
		}
	}

	return m_pFade;
}

//=====================================================
// ����������
//=====================================================
HRESULT CFade_FallIce::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// �f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	m_pTexture = nullptr;
	if (m_pTexture == nullptr)
	{
		D3DXCreateTextureFromFile
		(pDevice, FALLICE_TEX_PATH, &m_pTexture);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CFade_FallIce::Uninit(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CFade_FallIce::Update(void)
{
	CFade::FADE fade = CFade::GetState();

	if (fade != FADE_NONE)
	{
		if (fade == FADE_IN)
		{//�t�F�[�h�C�����
			m_nCounterFrame++;

			if (m_nCounterFrame == FALLICE_COUNT)
			{
				// �J�E���^�ݒ�
				m_nCounterFrame = 0;
				m_nCounterPattern++;

				// ���̕X�o��
				LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// �f�o�C�X�̎擾
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = nullptr;
				if (pVtxBuff == nullptr)
				{
					//���_�o�b�t�@�̐���
					pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FALLICE_PATTERN[m_nCounterPattern].size(),
						D3DUSAGE_WRITEONLY,
						FVF_VERTEX_2D,
						D3DPOOL_MANAGED,
						&pVtxBuff,
						NULL);
				}

				//���_���̃|�C���^
				VERTEX_2D* pVtx;

				//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				for (auto itr = FALLICE_PATTERN[m_nCounterPattern].begin(); itr != FALLICE_PATTERN[m_nCounterPattern].end(); itr++)
				{
					// �z�u�ʒu�v�Z
					D3DXVECTOR3 pos;
					pos.x = ((((*itr).y % 2 == 0) ? 0 : 0.5f) + (*itr).x) * POLYGON_WIDTH;
					pos.y = SCREEN_HEIGHT - (POLYGON_HEIGHT - (POLYGON_WIDTH / 2.0f / sqrtf(3))) * (*itr).y;
					pos.z = 0.0f;

					// �|���S�����ݒ�
					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(pos.x - POLYGON_WIDTH * 0.5f, pos.y - POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(pos.x + POLYGON_WIDTH * 0.5f, pos.y - POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(pos.x - POLYGON_WIDTH * 0.5f, pos.y + POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(pos.x + POLYGON_WIDTH * 0.5f, pos.y + POLYGON_HEIGHT * 0.5f, 0.0f);

					//rhw�̐ݒ�
					pVtx[0].rhw = 1.0f;
					pVtx[1].rhw = 1.0f;
					pVtx[2].rhw = 1.0f;
					pVtx[3].rhw = 1.0f;

					//���_�J���[�ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

					//�e�N�X�`���ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					// ���_�o�b�t�@�ړ�
					pVtx += 4;
				}
				//���_�o�b�t�@���A�����b�N
				pVtxBuff->Unlock();

				// ���_�o�b�t�@��ۑ�
				m_icePolygon.push_back(pVtxBuff);

				if (m_nCounterPattern == FALLICE_PATTERN.size() - 1)
				{//�X�Ŗ��߂���
					SetState(FADE_OUT);
					m_nCounterFrame = 0;
					m_nCounterPattern = -1;

					if (IsTrans())
					{
						Blur::ResetBlur();

						//���[�h�ݒ�
						CManager::SetMode(GetModeNext());
					}
				}
			}
		}
		else if (fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			m_nCounterFrame++;

			if (m_nCounterFrame == FALLICE_COUNT)
			{
				// �J�E���^�ݒ�
				m_nCounterFrame = 0;
				m_nCounterPattern++;

				// �X�폜�i��X�����j
				if (m_icePolygon[m_nCounterPattern] != nullptr)
				{
					m_icePolygon[m_nCounterPattern]->Release();
					m_icePolygon[m_nCounterPattern] = nullptr;
				}
			}

			if (m_nCounterPattern == FALLICE_PATTERN.size() - 1)
			{//�X�������؂�����
				SetState(FADE_NONE);
				m_icePolygon.clear();
			}
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CFade_FallIce::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	for (int cntVtx = 0; cntVtx < (int)m_icePolygon.size(); cntVtx++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_icePolygon[cntVtx], 0, sizeof(VERTEX_2D));

		//�|���S���̕`��
		for (int cntSize = 0; cntSize < (int)FALLICE_PATTERN[cntVtx].size(); cntSize++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, cntSize * 4, 2);
		}
	}
}

//=====================================================
// �ݒ菈��
//=====================================================
void CFade_FallIce::SetFade(CScene::MODE modeNext, bool bTrans)
{
	CFade::FADE fade = CFade::GetState();
	if (fade == FADE_NONE)
	{
		SetState(FADE_IN);
		SetModeNext(modeNext);
		SetIsTrans(bTrans);
		m_nCounterFrame = 0;
		m_nCounterPattern = -1;
	}
}
