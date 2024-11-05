//*****************************************************
//
// �^�C�g������[title.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "title.h"
#include "object.h"
#include "inputManager.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "cameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "polygon2D.h"
#include "UI.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MOVE_FACT	(0.01f)	// �ړ����x
#define LINE_UNINIT	(0.2f)	// �I������܂ł̂������l

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// ���j���[���ڂ̏����F
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// ���j���[���ڂ̑I��F
	const D3DXVECTOR2 UI_SIZE[CTitle::TITLE_UI_MAX] = 
	{
		D3DXVECTOR2 (0.2f,0.2f),
		D3DXVECTOR2 (0.2f,0.2f),
		D3DXVECTOR2 (0.3f,0.3f),
		D3DXVECTOR2 (0.1f,0.1f),
		D3DXVECTOR2 (0.1f,0.15f),
		D3DXVECTOR2 (0.08f,0.15f),
		D3DXVECTOR2 (0.4f,0.1f),
		D3DXVECTOR2 (0.5f,0.5f),
	};
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{
	nCntFlash = 0;
	nCntAlpha = 0;
	nCntMove = 0;
	m_State = STATE_NONE;
	m_TitleState = TITLESTATE_ICEFLOW;
	m_Title_UI = TITLE_UI_LEFT;
	m_apMenu_UI = nullptr;

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{
		m_apTitle_UI[nCntUI] = nullptr;
	}
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTitle::Init(void)
{
	const char* aPath =
	{// ���j���[�̃e�N�X�`���p�X
		"data\\TEXTURE\\TITLE\\Start.png",
	};

	const char* aTitle[TITLE_UI_MAX] =
	{// �^�C�g��UI�̃e�N�X�`���p�X
		"data\\TEXTURE\\TITLE\\ice_block_Left_2.png",
		"data\\TEXTURE\\TITLE\\ice_block_Right_2.png",
		"data\\TEXTURE\\TITLE\\ice_block.png",
		"data\\TEXTURE\\TITLE\\title_ice.png",
		"data\\TEXTURE\\TITLE\\penguin.png",
		"data\\TEXTURE\\TITLE\\turuhasi.png",
		"data\\TEXTURE\\TITLE\\title_full.png",
		"data\\TEXTURE\\TITLE\\pengui.png",
	};

	// �t�H�O�������Ȃ�
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}

	// �T�E���h�C���X�^���X�̎擾
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		// BGM�̍Đ�
		pSound->Play(pSound->LABEL_BGM_TITLE);
	}
	else if (pSound == nullptr)
	{
		return E_FAIL;
	}

	// �J�����ʒu�̐ݒ�
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 45.38f,84.71f,270.10f };
	pInfoCamera->posR = { -454.28f,331.03f,878.09f };

	// �w�i�I�u�W�F�N�g�̐���
	CObjectX* pArsenal = CObjectX::Create();

	if (pArsenal != nullptr)
	{
		int nIdx = CModel::Load("data\\MODEL\\other\\arsenal.x");
		pArsenal->BindModel(nIdx);
	}

	int nIdxTexture;

	// �Q�[���X�^�[�g�̃|���S���𐶐�
	if (m_apMenu_UI == nullptr)
	{
		m_apMenu_UI = CUI::Create();

		if (m_apMenu_UI != nullptr)
		{
			{//	�X�^�[�g���j���[
				// �|���S���̐ݒ�
				m_apMenu_UI->SetPosition(D3DXVECTOR3(0.5f, 0.75f, 0.0f));
				m_apMenu_UI->SetSize(0.2f, 0.15f);
				m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}

			// �e�N�X�`���̐ݒ�
			nIdxTexture = CTexture::GetInstance()->Regist(aPath);

			m_apMenu_UI->SetIdxTexture(nIdxTexture);
			m_apMenu_UI->SetVtx();
		}
	}

	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{// ���j���[���ڂ̃|���S���𐶐�
		if (m_apTitle_UI[nCntUI] == nullptr)
		{
			m_apTitle_UI[nCntUI] = CUI::Create();

			if (m_apTitle_UI[nCntUI] != nullptr)
			{
				if (nCntUI == TITLE_UI_LEFT)
				{//	�����炭��X
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(-0.2f, 0.4f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	�E���炭��X
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.36f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.6f;
				}

				if (nCntUI == TITLE_UI_ICEBLOCK)
				{//	�X�u���b�N����
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.28f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_ICE)
				{//	�X
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.8f, 0.23f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PENGUIN)
				{//	�y���M��
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.25f, 0.18f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PICKAXE)
				{//	��͂�
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.72f, 0.68f, 0.0f));
					m_apTitle_UI[nCntUI]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -0.5f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_LOGO)
				{//	�^�C�g�����S
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.48f, 0.33f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_FLASH)
				{//	�t���b�V��
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				// �e�N�X�`���̐ݒ�
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntUI]);

				m_apTitle_UI[nCntUI]->SetSize(UI_SIZE[nCntUI].x, UI_SIZE[nCntUI].y);
				m_apTitle_UI[nCntUI]->SetIdxTexture(nIdxTexture);
				m_apTitle_UI[nCntUI]->SetVtx();
			}
		}
	}

	CInputManager::Create();
	CScene::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTitle::Uninit(void)
{
	// ���j���[���ڂ̔j��
	if (m_apMenu_UI != nullptr)
	{
		m_apMenu_UI->Uninit();

		m_apMenu_UI = nullptr;
	}

	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
	{// �^�C�g��UI�̔j��
		if (m_apTitle_UI[nCnt] != nullptr)
		{
			m_apTitle_UI[nCnt]->Uninit();

			m_apTitle_UI[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CTitle::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	// ����
	Input();

	// �^�C�g��UI�̏�ԊǗ�
	TitleUIState();

	// �X������Ă����ԊǗ�
	IceFlowState();

	// ��ʂɃt���b�V���������ԊǗ�
	FlashState();

	// ���S����������
	LogoState();
}

//=====================================================
// �`�揈��
//=====================================================
void CTitle::Draw(void)
{
	CScene::Draw();
}

//=====================================================
// ���͏���
//=====================================================
void CTitle::Input(void)
{
	CInputManager *pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{// �t�F�[�h����
		Fade();
	}
}

//=====================================================
// �t�F�[�h����
//=====================================================
void CTitle::Fade(void)
{// �e��t�F�[�h
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE_NONE)
		return;

	pFade->SetFade(CScene::MODE_GAME);
}

//====================================================
// �^�C�g��UI�̏�ԊǗ�
//====================================================
void CTitle::TitleUIState(void)
{
	// �|���S����ڕW�ʒu�Ɍ����킹��
	for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
	{
		if (m_apTitle_UI[nCntUI] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntUI]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntUI] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntUI]->SetPosition(vecDiff);
			m_apTitle_UI[nCntUI]->SetVtx();

			float fDiff = m_aPosDest[nCntUI].x - vecDiff.x;
		}
	}
}

//====================================================
// �X������Ă����ԏ���
//====================================================
void CTitle::IceFlowState(void)
{
	// �X������Ă����Ԃ̎�
	if (m_TitleState == TITLESTATE_ICEFLOW)
	{
		D3DXVECTOR3 pos = m_apTitle_UI[TITLE_UI_LEFT]->GetPosition();

		// �ړI�̈ʒu�Ɍ��݂̈ʒu���߂���
		if (m_aPosDest[TITLE_UI_LEFT].x - 0.01f < pos.x)
		{
			m_apTitle_UI[TITLE_UI_LEFT]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_apTitle_UI[TITLE_UI_RIGHT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			m_TitleState = TITLESTATE_FLASH;
		}
	}
}

//====================================================
// ��ʂɃt���b�V�������鏈��
//====================================================
void CTitle::FlashState(void)
{
	// ��ʂɃt���b�V���������ԂɂȂ�����
	if (m_TitleState == TITLESTATE_FLASH ||
		m_TitleState == TITLESTATE_LOGO)
	{
		nCntFlash++;

		m_apTitle_UI[TITLE_UI_FLASH]->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));
		m_apTitle_UI[TITLE_UI_FLASH]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - nCntFlash * 0.005f));

		D3DXCOLOR col = m_apTitle_UI[TITLE_UI_FLASH]->GetCol();

		if (col.a < 0.5f)
		{
			m_TitleState = TITLESTATE_LOGO;
		}
	}
}

//====================================================
// ���S����������
//====================================================
void CTitle::LogoState(void)
{
	// ��ʂɃt���b�V���������ԂɂȂ�����
	if (m_TitleState == TITLESTATE_LOGO)
	{
		nCntAlpha++;
		nCntMove++;

		m_apMenu_UI->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_ICEBLOCK]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_ICE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_PENGUIN]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));
		m_apTitle_UI[TITLE_UI_PICKAXE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apTitle_UI[TITLE_UI_LOGO]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + nCntAlpha * 0.005f));

		for (int nCntUI = 0; nCntUI < TITLE_UI_MAX; nCntUI++)
		{
			if (nCntUI != TITLE_UI_PICKAXE)
			{
				// �|���S�����J�E���g���Ƃɓ�����
				if (nCntMove > 30 && nCntMove < 100)
				{
					m_aPosDest[nCntUI].y += 0.001f;
				}

				else if (nCntMove > 100 && nCntMove < 240)
				{
					m_aPosDest[nCntUI].y -= 0.001f;
				}

				else if (nCntMove > 240 && nCntMove < 380)
				{
					m_aPosDest[nCntUI].y += 0.001f;
				}

				else if (nCntMove > 380)
				{
					nCntMove = 100;
				}
			}
		}
	}
}
