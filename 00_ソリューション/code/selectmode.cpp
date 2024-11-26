//*****************************************************
//
// ���[�h�I������[selectmode.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectmode.h"
#include "object.h"
#include "inputManager.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "CameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "particle.h"
#include "orbit.h"
#include "debugproc.h"
#include "UI.h"
#include "gameManager.h"
#include "npcpenguin.h"
#include "npcpenguinstate_selectmode.h"
#include "snow.h"
#include "inputkeyboard.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	namespace selectUI
	{
		const char* PATH[CSelectMode::MODE_MAX] =
		{
			"data\\TEXTURE\\UI\\single_mode.png",
			"data\\TEXTURE\\UI\\party_mode.png",
			//"data\\TEXTURE\\UI\\tutorial00.jpg"
		};
		const float WIDTH[CSelectMode::MODE_MAX] =
		{
			0.14f,
			0.14f,
			//0.5f
		};
		const float HEIGHT[CSelectMode::MODE_MAX] =
		{
			0.25f,
			0.25f,
			//0.5f
		};
		const D3DXVECTOR3 POS[CSelectMode::MODE_MAX] =
		{
			D3DXVECTOR3(0.195f, 0.48f, 0.0f),
			D3DXVECTOR3(0.405f, 0.72f, 0.0f),
			//D3DXVECTOR3(0.5f, 0.5f, 0.0f)
		};
		const D3DXCOLOR NOSELECT_COLOR = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.8f);
		const float NOSELECT_ALPHA = 0.5f;	// �I������Ă��Ȃ��Ƃ��̕s�����x
		const float SELECTUI_COLORCHANGE_COEF = 0.4f;	// ���[�hUI�̕ω��̊���
	}

	namespace manual
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial00.jpg";	// �p�X
		const float WIDTH		= 0.5f;	// ��
		const float HEIGHT		= 0.5f;	// ����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �ʒu
	}

	namespace control
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial_control00.png";	// �p�X
		const float WIDTH		= 0.18f;	// ��
		const float HEIGHT		= 0.08f;	// ����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.82f, 0.91f, 0.0f);	// �ʒu
	}

	const int CNT_SNOW = 10;						// �Ⴊ�~��^�C�~���O
	const float MAX_HEIGHT = 1800.0f;				// �Ⴊ�~���Ă��鍂��
	const int MAX_SNOW_RADIUS = 100;				// �ő��̔��a
	const int MIN_SNOW_RADIUS = 20;				// �Œ��̔��a
	const int SNOW_LIFE = 200;						// ��̃��C�t
	const float SNOW_MOVE_SPED = -10.0f;			// ��̈ړ����x
	const int MAX_SNOW_POS = 2200;					// ��̍ő吶���ꏊ
	const int MIN_SNOW_POS = -2200;					// ��̍Œᐶ���ꏊ
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectMode::CSelectMode()
{
	m_pManual = nullptr;
	m_fCurTime = 0.0f;
	m_selectMode = MODE_SINGLE;
	for (int cnt = 0; cnt < MODE_MAX; cnt++) { m_apModeUI[cnt] = nullptr; }
	m_nSnowStormCnt = 0;
	m_bStorm = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSelectMode::~CSelectMode()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectMode::Init(void)
{
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
		return E_FAIL;

	// �J����
	Camera::ChangeState(new CCameraStateSelectMode);

	m_nSnowStormCnt = 0;
	m_bStorm = false;

	// �����̐���
	//m_pManual = CUI::Create();
	//if (m_pManual != nullptr)
	//{
	//	// �����̐ݒ�
	//	m_pManual->SetIdxTexture(CTexture::GetInstance()->Regist(manual::PATH));	// �e�N�X�`������
	//	m_pManual->SetPosition(manual::POS);				// �ʒu
	//	m_pManual->SetSize(manual::WIDTH, manual::HEIGHT);	// �傫��
	//	m_pManual->SetVtx();	// ���_���f
	//}

	// ���[�h�{�^��UI
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		m_apModeUI[cnt] = CUI::Create();
		if (m_apModeUI[cnt] != nullptr)
		{
			// �ݒ�
			m_apModeUI[cnt]->SetIdxTexture(CTexture::GetInstance()->Regist(selectUI::PATH[cnt]));	// �e�N�X�`������
			m_apModeUI[cnt]->SetPosition(selectUI::POS[cnt]);				// �ʒu
			m_apModeUI[cnt]->SetSize(selectUI::WIDTH[cnt], selectUI::HEIGHT[cnt]);	// �傫��
			m_apModeUI[cnt]->SetVtx();	// ���_���f
		}
	}
	ChangeSelectMode(0);	// �����I�����Ȃ��i�����x�ݒ�݂̂���j

	// ���b�V���t�B�[���h
	CMeshField* pMeshField = CMeshField::Create();
	pMeshField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\MATERIAL\\field.jpg"));
	pMeshField->SetDivTex(128);

	// ���܂���
	CObjectX *pIgloo = CObjectX::Create();
	pIgloo->BindModel(CModel::Load("data\\MODEL\\object\\Snowdome.x"));
	pIgloo->SetPosition(D3DXVECTOR3(800.0f, -10.0f, 600.0f));

	// BGM�̍Đ�
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	// �V�ԃy���M��
	CNPCPenguin* pPenguin = nullptr;
	// �E���S
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Stand);
	pPenguin->SetPosition(D3DXVECTOR3(400.0f, 10.0f, -1000.0f));
	// ���̕�1
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Stand);
	pPenguin->SetPosition(D3DXVECTOR3(-500.0f, 10.0f, 800.0f));
	// ���̕�2
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Stand);
	pPenguin->SetPosition(D3DXVECTOR3(2200.0f, 10.0f, 800.0f));
	// ���܂���
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Stand);
	pPenguin->SetPosition(D3DXVECTOR3(950.0f, 10.0f, 700.0f));

	// ���̓}�l�[�W���[����
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectMode::Uninit(void)
{
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			Object::DeleteObject((CObject**)&m_apModeUI[cnt]);
		}
	}

	Object::DeleteObject((CObject**)&m_pManual);

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();

	// �V�[���̏I��
	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectMode::Update(void)
{
	CInputManager *pInputMgr = CInputManager::GetInstance();
	assert(pInputMgr != nullptr);

	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	assert(pKeyboard != nullptr);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	CSnow::SetSnow(MAX_SNOW_POS, MIN_SNOW_POS, MAX_HEIGHT, MAX_SNOW_RADIUS, MIN_SNOW_RADIUS,
		SNOW_LIFE, D3DXVECTOR3(0.0f, SNOW_MOVE_SPED, 0.0f), CNT_SNOW);

	// ���[�h�̈ړ�
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_AXIS_LEFT))
	{
		ChangeSelectMode(-1);	// ���[�h�O�ɂ��炷

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_SELECT);
	}
	else if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_AXIS_RIGHT))
	{
		ChangeSelectMode(1);	// ���[�h��ɂ��炷

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_SELECT);
	}

	// ���[�h�I������
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		// �t�F�[�h���̏ꍇ������
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		vector<bool> abEnter = { true };

		switch (m_selectMode)
		{
		case CSelectMode::MODE_SINGLE:
			gameManager::SaveMode(CGame::E_GameMode::MODE_SINGLE, abEnter);
			pFade->SetFade(CScene::MODE_SELECTSTAGE);
			break;
		case CSelectMode::MODE_PARTY:
			pFade->SetFade(CScene::MODE_SELECTPLAYER);
			break;
		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	m_nSnowStormCnt++;

	// ����𐶐�
	if (m_nSnowStormCnt == 120)
	{
		CParticle::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), CParticle::TYPE_SNOW_STORM);

		m_nSnowStormCnt = 0;
	}

	// ���[�hUI�����ڍX�V����
	UpdateSelectModeUI();

	// �V�[���̍X�V
	CScene::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CSelectMode::Draw(void)
{
	// �V�[���̕`��
	CScene::Draw();
}

//=====================================================
// �I������̓_�ōX�V����
//=====================================================
void CSelectMode::ChangeSelectMode(int move)
{
	m_selectMode = (MODE)((m_selectMode + move + MODE_MAX) % MODE_MAX);	// �l�ړ�
}

//=====================================================
// ���[�hUI�����ڍX�V����
//=====================================================
void CSelectMode::UpdateSelectModeUI(void)
{
	// �I�����Ă��郂�[�h�s����/����ȊO������
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			// �ݒ�
			if (cnt == m_selectMode)
			{// �I�����Ă��郂�[�h
				D3DXCOLOR nowCol = m_apModeUI[cnt]->GetCol();
				D3DXCOLOR diffCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) - nowCol;
				diffCol *= selectUI::SELECTUI_COLORCHANGE_COEF;
				nowCol += diffCol;
				m_apModeUI[cnt]->SetCol(nowCol);
			}
			else
			{// ����ȊO������
				D3DXCOLOR nowCol = m_apModeUI[cnt]->GetCol();
				D3DXCOLOR diffCol = selectUI::NOSELECT_COLOR - nowCol;
				diffCol *= selectUI::SELECTUI_COLORCHANGE_COEF;
				nowCol += diffCol;
				m_apModeUI[cnt]->SetCol(nowCol);
			}
			m_apModeUI[cnt]->SetVtx();	// ���_���f
		}
	}
}
