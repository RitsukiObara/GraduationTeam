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
#define MENU_WIDTH	(0.2f)	// ���ڂ̕�
#define MENU_HEIGHT	(0.2f)	// ���ڂ̍���
#define MOVE_FACT	(0.01f)	// �ړ����x
#define LINE_UNINIT	(0.2f)	// �I������܂ł̂������l

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const D3DXCOLOR COL_INITIAL_MENU = { 0.4f,0.4f,0.4f,1.0f };	// ���j���[���ڂ̏����F
	const D3DXCOLOR COL_CURRENT_MENU = { 1.0f,1.0f,1.0f,1.0f };	// ���j���[���ڂ̑I��F
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{
	m_State = STATE_NONE;
	m_TitleState = TITLESTATE_ICEFLOW;

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{
		m_apMenu_UI[nCntMenu] = nullptr;
	}

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
	const char* aPath[MENU_MAX] =
	{// ���j���[�̃e�N�X�`���p�X
		"data\\TEXTURE\\UI\\menu00.png",
		"data\\TEXTURE\\UI\\menu01.png",
	};

	const char* aTitle[TITLE_UI_MAX] =
	{// �^�C�g��UI�̃e�N�X�`���p�X
		"data\\TEXTURE\\TITLE\\ice_block_Left_2.png",
		"data\\TEXTURE\\TITLE\\ice_block_Right_2.png",
		"data\\TEXTURE\\TITLE\\ice_block.png",
		"data\\TEXTURE\\TITLE\\title_full.png",
		"data\\TEXTURE\\TITLE\\title_ice.png",
		"data\\TEXTURE\\TITLE\\penguin.png",
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

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// ���j���[���ڂ̃|���S���𐶐�

		if (m_apMenu_UI[nCntMenu] == nullptr)
		{
			m_apMenu_UI[nCntMenu] = CUI::Create();

			if (m_apMenu_UI[nCntMenu] != nullptr)
			{
				if (nCntMenu == MENU_START)
				{//	�X�^�[�g���j���[
					// �|���S���̐ݒ�
					m_apMenu_UI[nCntMenu]->SetPosition(D3DXVECTOR3(-0.2f, 0.4f, 0.0f));
					m_apMenu_UI[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu_UI[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}

				if (nCntMenu == MENU_OPTION)
				{//	�I�v�V�������j���[
					// �|���S���̐ݒ�
					m_apMenu_UI[nCntMenu]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apMenu_UI[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu_UI[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				}

				// �e�N�X�`���̐ݒ�
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntMenu]);

				m_apMenu_UI[nCntMenu]->SetIdxTexture(nIdxTexture);
				m_apMenu_UI[nCntMenu]->SetVtx();
			}
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
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	�E���炭��X
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.65f;
				}

				if (nCntUI == TITLE_UI_ICEBLOCK)
				{//	�X�u���b�N����
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, -0.25f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT + 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}


				if (nCntUI == TITLE_UI_LOGO)
				{//	�^�C�g���ɖ߂�
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, -0.3f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.2f, MENU_HEIGHT - 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_ICE)
				{//	�|�[�Y
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.8f, -0.2f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH - 0.1f, MENU_HEIGHT - 0.1f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				if (nCntUI == TITLE_UI_PENGUIN)
				{//	�|�[�Y
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.25f, -0.15f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH - 0.1f, MENU_HEIGHT - 0.05f);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				}

				// �e�N�X�`���̐ݒ�
				nIdxTexture = CTexture::GetInstance()->Regist(aTitle[nCntUI]);

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
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apMenu_UI[nCnt] != nullptr)
		{
			m_apMenu_UI[nCnt]->Uninit();

			m_apMenu_UI[nCnt] = nullptr;
		}
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

	// ��ԊǗ�
	ManageState();

	// �^�C�g��UI�̏�ԊǗ�
	TitleUIState();

	// �F�̊Ǘ�
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (m_apMenu_UI[i] != nullptr)
		{
			D3DXCOLOR colDest = COL_INITIAL_MENU;

			if (i == m_Menu)
			{
				colDest = COL_CURRENT_MENU;
			}

			D3DXCOLOR col = m_apMenu_UI[i]->GetCol();

			col += (colDest - col) * 0.3f;

			m_apMenu_UI[i]->SetCol(col);
		}
	}
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

	// ���ڑI��
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_Menu = (MENU)((m_Menu + MENU_MAX - 1) % MENU_MAX);
	}
	else if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_Menu = (MENU)((m_Menu + 1) % MENU_MAX);
	}

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

	switch (m_Menu)
	{
	case CTitle::MENU_START:	// �X�^�[�g

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CTitle::MENU_OPTION:	// �ݒ�

		pFade->SetFade(CScene::MODE_TUTORIAL);

		break;
	default:
		break;
	}
}

//====================================================
// ���j���[UI��ԊǗ�
//====================================================
void CTitle::ManageState(void)
{
	if (m_State != STATE_OUT)
	{
		Input();
	}

	// �I���t���O�p
	int nEnd = 0;

	// �|���S����ڕW�ʒu�Ɍ����킹��
	for (int nCntUI = 0; nCntUI < MENU_MAX; nCntUI++)
	{
		if (m_apMenu_UI[nCntUI] != nullptr)
		{
			D3DXVECTOR3 pos = m_apMenu_UI[nCntUI]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntUI] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apMenu_UI[nCntUI]->SetPosition(vecDiff);
			m_apMenu_UI[nCntUI]->SetVtx();

			float fDiff = m_aPosDest[nCntUI].x - vecDiff.x;

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_State == STATE_OUT)
			{// �I���̃��C��
				nEnd++;
			}
		}
	}

	if (nEnd == TITLE_UI_MAX &&
		m_State == STATE_OUT)
	{
		Uninit();
	}
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
// �X������Ă�����
//====================================================
void CTitle::IceFlowState(void)
{

}
