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
#include "inputkeyboard.h"
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
			D3DXVECTOR3(0.395f, 0.38f, 0.0f),
			D3DXVECTOR3(0.605f, 0.62f, 0.0f),
			//D3DXVECTOR3(0.5f, 0.5f, 0.0f)
		};
		const float NOSELECT_ALPHA = 0.35f;	// �I������Ă��Ȃ��Ƃ��̕s�����x
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
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

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

	// BGM�̍Đ�
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

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

	// �V�[���̏I��
	CScene::Uninit();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectMode::Update(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	assert(pKeyboard != nullptr);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	// ���[�h�̈ړ�
	if (pKeyboard->GetTrigger(DIK_A))
	{
		ChangeSelectMode(-1);	// ���[�h�O�ɂ��炷

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
	else if (pKeyboard->GetTrigger(DIK_D))
	{
		ChangeSelectMode(1);	// ���[�h��ɂ��炷

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}

	// ���[�h�I������
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// �t�F�[�h���̏ꍇ������
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		switch (m_selectMode)
		{
		case CSelectMode::MODE_SINGLE:
			pFade->SetFade(CScene::MODE_GAME);
			break;
		case CSelectMode::MODE_PARTY:
			pFade->SetFade(CScene::MODE_SELECTPLAYER);
			break;
		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}

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
	// �l�ړ�
	m_selectMode = (MODE)((m_selectMode + move + MODE_MAX) % MODE_MAX);

	// �I�����Ă��郂�[�h�s����/����ȊO������
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			// �ݒ�
			if (cnt == m_selectMode)
			{// �I�����Ă��郂�[�h
				m_apModeUI[cnt]->SetAlpha(1.0f);
			}
			else
			{// ����ȊO������
				m_apModeUI[cnt]->SetAlpha(selectUI::NOSELECT_ALPHA);
			}
			m_apModeUI[cnt]->SetVtx();	// ���_���f
		}
	}
}
