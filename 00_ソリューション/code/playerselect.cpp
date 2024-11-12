//*****************************************************
//
// �v���C���[�I������[playerselect.cpp]
// Author:�x�씋��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "playerselect.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "inputjoypad.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "debugproc.h"
#include "UIManager.h"
#include "polygon3D.h"
#include "texture.h"
#include "skybox.h"
#include "renderer.h"
#include "animEffect3D.h"
#include "pause.h"
#include "slow.h"
#include "meshfield.h"
#include "CameraState.h"
#include "particle.h"
#include "meshCube.h"
#include "blur.h"
#include "light.h"
#include "ice.h"
#include "iceManager.h"
#include "player.h"
#include "seals.h"
#include "flowIce.h"
#include "BG_Ice.h"
#include "flowIceFactory.h"
#include "UI.h"
#include "ocean.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(100)	// �I���܂ł̗]�C�̃t���[����
#define SPEED_TIME	(60)	// �^�C�}�[�������Ă������x

namespace
{
	const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// �Q�[�����b�V�����[�h�̃p�X
	const int NUM_LIGHT = 3;	// ���C�g�̐�
	const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ���C�g�̃f�t�H���g�F
	const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ���C�g�̐F���ς�鑬�x

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayerSelect* CPlayerSelect::m_pGame = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nCountPlayer = 0;
	m_posMid = { 0.0f,0.0f,0.0f };
	ZeroMemory(&m_StandbyState[0], sizeof(m_StandbyState));
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));

}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	m_pGame = this;

	m_colLight = COL_LIGHT_DEFAULT;

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	COcean::Create();

	// �t�H�O��������
	CRenderer* pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(true);
		pRenderer->SetStart(50000);
		pRenderer->SetEnd(70000);
		pRenderer->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	// �X���[�Ǘ��̐���
	CSlow::Create();

	// ���C�g�̐���
	CreateLight();

	Camera::ChangeState(new CFollowPlayer);


	// �X�}�l�[�W���[
	CIceManager::Create(15, 15);

	// �e�N�X�`���ԍ��擾
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	// UI����
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// �v���C���[�i���o�[UI�̐���
		m_apPlayerUI[nCount] = CUI::Create();
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(0.05f,0.05f);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + 0.25f * nCount, 0.8f, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + 0.25f * nCount, 0.0f), D3DXVECTOR2(0.25f + 0.25f * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		// �G���g���[UI�̐���
		m_apStateUI[nCount] = CUI::Create();
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.07f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + 0.25f * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayerSelect::Uninit(void)
{
	// �e��I�u�W�F�N�g�̔j��
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->Uninit();
			m_apStateUI[nCount] = nullptr;
		}
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->Uninit();
			m_apPlayerUI[nCount] = nullptr;
		}
		if (m_apPlayer[nCount] != nullptr)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = nullptr;
		}
	}
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	CInputManager* pInputManager = CInputManager::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CSound* pSound = CSound::GetInstance();


	// �V�[���̍X�V
	CScene::Update();

	
	// 1�l���G���g���[�����ꍇ
	if (m_StandbyState[0] != STANDBY_OK)
	{
		if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, m_nCountPlayer) == true)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\standby.png");

			// �v���C���[����
			m_apPlayer[m_nCountPlayer] = CPlayer::Create();
			m_apStateUI[m_nCountPlayer]->SetIdxTexture(nIdx);
			m_StandbyState[m_nCountPlayer] = STANDBY_PLAY;
			m_apPlayerUI[m_nCountPlayer]->SetSize(0.05f, 0.05f);
			m_apStateUI[m_nCountPlayer]->SetVtx();

			m_apPlayer[m_nCountPlayer]->SetMove(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
			m_apPlayer[m_nCountPlayer]->SetState(CPlayer::STATE_NORMAL);

			m_nCountPlayer++;

		}
		// �v���C���[�S���G���g���[�����ꍇ
		if (m_StandbyState[0] == STANDBY_PLAY && m_StandbyState[1] == STANDBY_PLAY && m_StandbyState[2] == STANDBY_PLAY && m_StandbyState[3] == STANDBY_PLAY)
		{
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Junbi_ok.png");

			for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
			{
				m_StandbyState[nCnt] = STANDBY_OK;
				m_apStateUI[nCnt]->SetIdxTexture(nIdx);

			}
		}
	}

	// �J�ڂł����Ԃ̏ꍇ
	if (m_StandbyState[0] == STANDBY_OK && m_StandbyState[1] == STANDBY_OK && m_StandbyState[2] == STANDBY_OK && m_StandbyState[3] == STANDBY_OK)
	{
		if (pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0) == true)
		{
			CFade* pFade = CFade::GetInstance();

			if (pFade == nullptr)
				return;

			if (pFade->GetState() != CFade::FADE_NONE)
				return;

			pFade->SetFade(CScene::MODE_GAME);
		}
	}

	// �J�����X�V
	UpdateCamera();

	// ���C�g�̍X�V
	UpdateLight();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
//=====================================================
void CPlayerSelect::UpdateCamera(void)
{
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// ���C�g�̐���
//=====================================================
void CPlayerSelect::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight* pLight = CLight::Create();

		if (pLight == nullptr)
			continue;

		D3DLIGHT9 infoLight = pLight->GetLightInfo();

		infoLight.Type = D3DLIGHT_DIRECTIONAL;
		infoLight.Diffuse = m_colLight;

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
		infoLight.Direction = vecDir;

		pLight->SetLightInfo(infoLight);

		m_aLight.push_back(pLight);
	}
}

//=====================================================
// ���C�g�̍X�V
//=====================================================
void CPlayerSelect::UpdateLight(void)
{
	for (auto it : m_aLight)
	{
		D3DLIGHT9 infoLight = it->GetLightInfo();

		D3DXCOLOR col = infoLight.Diffuse;

		col += (m_colLight - col) * SPEED_CHANGE_LIGHTCOL;

		infoLight.Diffuse = col;

		it->SetLightInfo(infoLight);
	}
}

//=====================================================
// ���C�g�̐F���Z�b�g
//=====================================================
void CPlayerSelect::ResetDestColLight(void)
{
	m_colLight = COL_LIGHT_DEFAULT;
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_I))
	{// �A�C�X������
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
			pIceManager->CreateIce(2, -1);
	}

	pDebugProc->Print("\n���S���W[%f,%f,%f]", m_posMid.x, m_posMid.y, m_posMid.z);
	pDebugProc->Print("\n�G���g���[�l��[%d�l]", m_nCountPlayer);
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayerSelect::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}
}