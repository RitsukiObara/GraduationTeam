//*****************************************************
//
// �Q�[������[game.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "game.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputManager.h"
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
#include "timer.h"
#include "meshCube.h"
#include "blur.h"
#include "light.h"
#include "result.h"
#include "title.h"
#include "ice.h"
#include "iceManager.h"
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "seals.h"
#include "ocean.h"
#include "stageResultUI.h"
#include "UI_enemy.h"
#include "ocean_flow_UI.h"
#include "flowIce.h"
#include "BG_Ice.h"
#include "flowIceFactory.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(100)	// �I���܂ł̗]�C�̃t���[����
namespace
{
const char* PATH_GAME_ROAD = "data\\MAP\\road00.bin";	// �Q�[�����b�V�����[�h�̃p�X
const int NUM_LIGHT = 3;	// ���C�g�̐�
const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ���C�g�̃f�t�H���g�F
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ���C�g�̐F���ς�鑬�x

const int NUM_GRID_V = 10;	// �c�O���b�h�̐�
const int NUM_GRID_H = 10;	// ���O���b�h�̐�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CGame::STATE CGame::m_state = STATE_NONE;	// ���
CGame *CGame::m_pGame = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGame::CGame()
{
	m_nCntState = 0;
	m_bStop = false;
	m_posMid = { 0.0f,0.0f,0.0f };
	m_pPause = nullptr;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGame::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	m_pGame = this;

	m_colLight = COL_LIGHT_DEFAULT;
	m_state = STATE_NORMAL;
	m_bStop = false;

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	 //�C�̒ǉ�
	COcean::Create();

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

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
	
	// �^�C�}�[�\���̐���
	m_pTimer = CTimer::Create();
	m_pTimer->SetPosition(D3DXVECTOR3(0.48f, 0.07f, 0.0f));
	m_pTimer->SetSecond(MAX_TIME);

	// �X�R�A�\���̐���
	m_pScore = CScore::Create();
	m_pScore->SetPosition(D3DXVECTOR3(0.09f, 0.07f, 0.0f));

	// �X�e�[�W���U���g�\���̐���
	//m_pStageResultUI = CStageResultUI::Create();
	//m_pStageResultUI->SetPosition(D3DXVECTOR3(0.4f, 0.07f, 0.0f));

	// �X�}�l�[�W���[
	CIceManager::Create(NUM_GRID_V, NUM_GRID_H);

	// ��󃂃f���̐���
	m_pOceanFlowUI = COceanFlowUI::Create();
	m_pOceanFlowUI->SetPosition(D3DXVECTOR3(0.09f, 0.07f, 0.0f));

	// �G���\��UI����
	CUIEnemy::Create();

	// �G����
	CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);

	// �v���C���[����
	CPlayer::Create();

	// ���X����
	CFlowIce::Create();

	// �w�i�X����
	CBgIce::Create(D3DXVECTOR3(0.0f,200.0f,0.0f),CBgIce::TYPE_SMALL);

	// ���X�t�@�N�g���[�̐���
	CFlowIceFct::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	CScene::Uninit();

	m_pGame = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	CFade *pFade = CFade::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();

	if (!m_bStop)
	{
		// �V�[���̍X�V
		CScene::Update();
	}

	// �|�[�Y========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
	{
		if(m_pPause == nullptr)
			m_pPause = CPause::Create();
	}

	// �J�����X�V
	UpdateCamera();

	// ��ԊǗ�
	ManageState();

	// ���C�g�̍X�V
	UpdateLight();

	// �|�[�Y�̍X�V
	UpdatePause();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
//=====================================================
void CGame::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CGame::ManageState(void)
{
	CFade *pFade = CFade::GetInstance();

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		// �^�C�}�[�̍X�V
		m_pTimer->AddSecond(-CManager::GetDeltaTime());
		m_pTimer->Update();

		break;
	case CGame::STATE_RESULT:

		break;
	case CGame::STATE_END:

		m_nCntState++;
		if (m_nCntState >= TRANS_TIME && pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// ��~��Ԃ̐؂�ւ�
//=====================================================
void CGame::ToggleStop(void)
{
	m_bStop = m_bStop ? false : true;

	if (m_bStop)
	{
		Camera::ChangeState(new CMoveControl);
	}
	else
	{
		Camera::ChangeState(new CFollowPlayer);
	}
}

//=====================================================
// ���C�g�̐���
//=====================================================
void CGame::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight *pLight = CLight::Create();

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
void CGame::UpdateLight(void)
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
// �|�[�Y�̍X�V
//=====================================================
void CGame::UpdatePause(void)
{
	if (m_pPause == nullptr)
	{
		return;
	}

	m_pPause->Update();
}

//=====================================================
// ���C�g�̐F���Z�b�g
//=====================================================
void CGame::ResetDestColLight(void)
{
	m_colLight = COL_LIGHT_DEFAULT;
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CGame::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_F))
	{// ��~��Ԃ̐؂�ւ�
		ToggleStop();
	}

	if (pKeyboard->GetTrigger(DIK_0))
	{// ����
		CResult::Create(true);
	}
	if (pKeyboard->GetTrigger(DIK_9))
	{// �s�k
		CResult::Create(false);
	}

	if (pKeyboard->GetTrigger(DIK_I))
	{// �A�C�X������
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
			pIceManager->CreateIce(2,-1);
	}

	pDebugProc->Print("\n���S���W[%f,%f,%f]", m_posMid.x, m_posMid.y, m_posMid.z);
}

//=====================================================
// �`�揈��
//=====================================================
void CGame::Draw(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	char *apString[STATE::STATE_MAX] =
	{
		"NONE",
		"NORMAL",
		"END",
	};
}