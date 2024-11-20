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
#include "gameManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_UI_PLAYERNUMBER = 0.05f;	// �v���C���[�i���o�[UI�̃T�C�Y
const float HEIGHT_UI_PLAYERNUMBER = 0.8f;	// �v���C���[�i���o�[UI�̈ʒu�̍���

const string PATH_UI_STANDBY = "data\\TEXTURE\\UI\\standby.png";	// �X�^���h�o�C�e�N�X�`���̃p�X
const string PATH_UI_READY = "data\\TEXTURE\\UI\\ready.png";	// ���������e�N�X�`���̃p�X

const D3DXVECTOR3 INIT_MOVE_PLAYER = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// �v���C���[�̏����̈ړ���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nNumPlayer = 0;
	ZeroMemory(&m_StandbyState[0], sizeof(m_StandbyState));
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));
	ZeroMemory(&m_apInputMgr[0], sizeof(m_apInputMgr));
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	Camera::ChangeState(new CFollowPlayer);

	// �e�N�X�`���ԍ��擾
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	float fRateOnePlayer = 1.0f / MAX_PLAYER;

	// UI����
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// �v���C���[�i���o�[UI�̐���
		m_apPlayerUI[nCount] = CUI::Create();
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(SIZE_UI_PLAYERNUMBER, SIZE_UI_PLAYERNUMBER);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, HEIGHT_UI_PLAYERNUMBER, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + fRateOnePlayer * nCount, 0.0f), D3DXVECTOR2(fRateOnePlayer + fRateOnePlayer * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		// �G���g���[UI�̐���
		m_apStateUI[nCount] = CUI::Create();
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.07f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}

		// ���̓}�l�[�W���[�̐���
		CInputManager *pInputMgr = CInputManager::Create();
		m_apInputMgr[nCount] = pInputMgr;
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
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	CSound* pSound = CSound::GetInstance();
	
	// �V�[���̍X�V
	CScene::Update();

	// ����
	Input();

	// �J�n���邩�̊m�F
	CheckStart();
	
#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ����
//=====================================================
void CPlayerSelect::Input(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_apInputMgr[i] == nullptr)
			continue;

		if (m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
			CreatePlayer(i);	// �v���C���[�̃G���g���[

		if (m_apPlayer[i] != nullptr &&
			m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_READY))
			Ready(i);	// ����
	}
}

//=====================================================
// �v���C���[�̐���
//=====================================================
void CPlayerSelect::CreatePlayer(int nIdx)
{
	if (m_apPlayer[nIdx] != nullptr)
		return;	// �g�����܂��Ă��珈����ʂ�Ȃ�

	// �e�N�X�`���ύX
	int nIdxTexture = Texture::GetIdx(&PATH_UI_STANDBY[0]);
	if(m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);

	// �G���g���[��Ԃ�ݒ�
	m_StandbyState[nIdx] = STANDBY_PLAY;
	
	// �v���C���[�̐���
	m_apPlayer[nIdx] = CPlayer::Create();
	
	if (m_apPlayer[nIdx] != nullptr)
	{
		// �v���C���[�����ݒ�
		m_apPlayer[nIdx]->SetMove(INIT_MOVE_PLAYER);
		m_apPlayer[nIdx]->SetState(CPlayer::STATE_NORMAL);

		// ���̓}�l�[�W���[�̊��蓖��
		m_apPlayer[nIdx]->BindInputMgr(m_apInputMgr[nIdx]);	

		// �v���C���[ID�̊��蓖��
		m_apPlayer[nIdx]->SetID(nIdx);
	}

	m_nNumPlayer++;
}

//=====================================================
// ����
//=====================================================
void CPlayerSelect::Ready(int nIdx)
{
	// �G���g���[��Ԃ�ݒ�
	m_StandbyState[nIdx] = STANDBY_OK;

	// �e�N�X�`���ύX
	int nIdxTexture = Texture::GetIdx(&PATH_UI_READY[0]);
	if (m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);
}

//=====================================================
// �J�n���邩�̊m�F
//=====================================================
void CPlayerSelect::CheckStart(void)
{
	if (m_nNumPlayer == 0)
		return;

	bool bStart = true;

	for (int i = 0; i < MAX_PLAYER; i++)
		if (m_StandbyState[i] != E_StandyrState::STANDBY_OK && m_apPlayer[i] != nullptr)
			bStart = false;

	if (bStart)
		StartFade();
}

//=====================================================
// �t�F�[�h�̊J�n
//=====================================================
void CPlayerSelect::StartFade(void)
{
	CFade* pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE::FADE_NONE)
		return;

	// �v���C���[�G���^�[�t���O�̐ݒ�
	vector<bool> abEnter(MAX_PLAYER);

	for (int i = 0; i < MAX_PLAYER; i++)
		abEnter[i] = m_apPlayer[i] != nullptr;

	// ���[�h�̕ۑ�
	gameManager::SaveMode(CGame::E_GameMode::MODE_MULTI, abEnter);

	// �Q�[���ɑJ��
	pFade->SetFade(CScene::MODE_SELECTSTAGE);
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayerSelect::Draw(void)
{

}