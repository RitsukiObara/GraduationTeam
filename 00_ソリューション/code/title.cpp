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
	m_state = STATE_NONE;
	m_pLogo = nullptr;
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
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.35f;
				}

				if (nCntUI == TITLE_UI_RIGHT)
				{//	�E���炭��X
					// �|���S���̐ݒ�
					m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(1.2f, 0.35f, 0.0f));
					m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
					m_aPosDest[nCntUI].x = 0.65f;
				}

				//if (nCntUI == TITLE_UI_LOGO)
				//{//	�^�C�g���ɖ߂�
				//	// �|���S���̐ݒ�
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(0.5f, 0.4f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.2f, MENU_HEIGHT - 0.1f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.25f;
				//}

				//if (nCntUI == TITLE_UI_ICE)
				//{//	�|�[�Y
				//	// �|���S���̐ݒ�
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(2.5f, 0.12f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT - 0.05f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.5f;
				//}

				//if (nCntUI == TITLE_UI_PENGUIN)
				//{//	�|�[�Y
				//	// �|���S���̐ݒ�
				//	m_apTitle_UI[nCntUI]->SetPosition(D3DXVECTOR3(2.5f, 0.12f, 0.0f));
				//	m_apTitle_UI[nCntUI]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT - 0.05f);
				//	m_apTitle_UI[nCntUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				//	//m_aPosDest[nCntUI] = m_apTitle_UI[nCntUI]->GetPosition();
				//	//m_aPosDest[nCntUI].x = 0.5f;
				//}

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
	Object::DeleteObject((CObject**)&m_pLogo);

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

	// ��ԊǗ�
	ManageState();

	// ����
	Input();

	// �F�̊Ǘ�
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (m_apTitle_UI[i] != nullptr)
		{
			D3DXCOLOR colDest = COL_INITIAL_MENU;

			if (i == m_menu)
			{
				colDest = COL_CURRENT_MENU;
			}

			D3DXCOLOR col = m_apTitle_UI[i]->GetCol();

			col += (colDest - col) * 0.3f;

			m_apTitle_UI[i]->SetCol(col);
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

		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}
	else if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		Sound::Play(CSound::LABEL_SE_PAUSE_ARROW);

		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
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

	switch (m_menu)
	{
	case CTitle::MENU_GAME:	// �Q�[��

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
// ��ԊǗ�
//====================================================
void CTitle::ManageState(void)
{
	if (m_state != STATE_OUT)
	{
		Input();
	}

	// �I���t���O�p
	int nEnd = 0;

	// �|���S����ڕW�ʒu�Ɍ����킹��
	for (int nCntMenu = 0; nCntMenu < TITLE_UI_MAX; nCntMenu++)
	{
		if (m_apTitle_UI[nCntMenu] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntMenu]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntMenu] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntMenu]->SetPosition(vecDiff);
			m_apTitle_UI[nCntMenu]->SetVtx();

			float fDiff = m_aPosDest[nCntMenu].x - vecDiff.x;

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_state == STATE_OUT)
			{// �I���̃��C��
				nEnd++;
			}
		}
	}

	if (nEnd == TITLE_UI_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// UI�̏�ԊǗ�
//====================================================
void CTitle::TitleUIState(void)
{
	// �|���S����ڕW�ʒu�Ɍ����킹��
	for (int nCntMenu = 0; nCntMenu < TITLE_UI_MAX; nCntMenu++)
	{
		if (m_apTitle_UI[nCntMenu] != nullptr)
		{
			D3DXVECTOR3 pos = m_apTitle_UI[nCntMenu]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntMenu] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apTitle_UI[nCntMenu]->SetPosition(vecDiff);
			m_apTitle_UI[nCntMenu]->SetVtx();

			float fDiff = m_aPosDest[nCntMenu].x - vecDiff.x;
		}
	}
}
