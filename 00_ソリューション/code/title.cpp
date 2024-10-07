//*****************************************************
//
// �^�C�g������[title.cpp]
// Author:���R����
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
#include "CameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "smoke.h"
#include "meshcylinder.h"
#include "fan3D.h"
#include "meshfield.h"
#include "particle.h"
#include "orbit.h"
#include "debugproc.h"
#include "UI.h"
#include "light.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int NUM_LIGHT = 3;	// ���C�g�̐�

	namespace title
	{
		const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// �p�X
		const float WIDTH		= 1.09f * 0.5f * 0.88f;	// ��
		const float HEIGHT		= 0.18f * 0.88f;		// ����
		const float	MOVE_TIME	= 1.5f;	// �ړ�����
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;						// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f, 0.32f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// �����ʒu
	}

	namespace select
	{
		const char* PATH[] =
		{
			"data\\TEXTURE\\UI\\title_select00.png",	// �Q�[���I�����̃p�X
			"data\\TEXTURE\\UI\\title_select01.png",	// �`���[�g���A���I�����̃p�X
		};

		const float	MOVE_TIME	= 0.84f;	// �ړ�����
		const float	PLUS_TIME	= 0.0f;	// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �ҋ@����
		const float WIDTH		= 0.8f  * 0.32f;	// ��
		const float HEIGHT		= 0.25f * 0.32f;	// ����
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// ��I�����̐F
		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// �I�����̐F
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// �����F
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(0.36f, 0.0f, 0.0f);			// ��
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f - SPACE.x * 0.5f, 0.85f, 0.0f);	// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS;				// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;	// �����ʒu
	}
}

//*****************************************************
//	�ÓI�����o�ϐ��錾
//*****************************************************
CTitle::AFuncUpdateState CTitle::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// �������Ȃ��X�V
	&CTitle::UpdateLogoMove,	// ���S�̈ړ���Ԃ̍X�V
	&CTitle::UpdateSelectMove,	// �I�����ڂ̈ړ���Ԃ̍X�V
	&CTitle::UpdateSelect,		// �I����Ԃ̍X�V
	nullptr,					// �I����Ԃ̍X�V
};

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));
	m_state = STATE_LOGO_MOVE;
	m_pLogo = nullptr;
	m_fCurTime = 0.0f;
	m_nCurSelect = 0;
	m_nOldSelect = 0;
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
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ���S�̐���
	m_pLogo = CUI::Create();
	if (m_pLogo != nullptr)
	{
		// ���S�̐ݒ�
		m_pLogo->SetIdxTexture(CTexture::GetInstance()->Regist(title::PATH));	// �e�N�X�`������
		m_pLogo->SetPosition(title::INIT_POS);			// �ʒu
		m_pLogo->SetSize(title::WIDTH, title::HEIGHT);	// �傫��
		m_pLogo->SetCol(title::INIT_COL);				// �F
		m_pLogo->SetVtx();								// ���_���f
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̐���
		m_apSelect[i] = CUI::Create();
		if (m_apSelect[i] != nullptr)
		{
			// �����ʒu���v�Z
			D3DXVECTOR3 posSelect = select::INIT_POS + (select::SPACE * (float)i);	// �����ʒu

			// �I�����̐ݒ�
			m_apSelect[i]->SetIdxTexture(CTexture::GetInstance()->Regist(select::PATH[i]));	// �e�N�X�`������
			m_apSelect[i]->SetPosition(posSelect);					// �ʒu
			m_apSelect[i]->SetSize(select::WIDTH, select::HEIGHT);	// �傫��
			m_apSelect[i]->SetCol(select::INIT_COL);				// �F
			m_apSelect[i]->SetVtx();								// ���_���f
		}
	}

	// �J�����C���X�^���X�擾
	CCamera *pCamera = CManager::GetCamera();
	assert(pCamera != nullptr);

	// �J�������̐ݒ�
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	pInfoCamera->fLength = { 500.0f };			// ����
	pInfoCamera->rot  = { 1.35f, 0.0f, 0.0f };	// ����
	pInfoCamera->posR = { 0.0f, 600.0f, 0.0f };	// �����_�ʒu
	pCamera->SetPosV();	// ���_���f

	// ���C�g�̐���
	CreateLight();

	// BGM�̍Đ�
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	return S_OK;
}

//=====================================================
// ���C�g�̐���
//=====================================================
void CTitle::CreateLight(void)
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
		infoLight.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
		infoLight.Direction = vecDir;

		pLight->SetLightInfo(infoLight);
	}
}

//=====================================================
// �I������
//=====================================================
void CTitle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pLogo);
	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		Object::DeleteObject((CObject**)&m_apSelect[i]);
	}

	// �V�[���̏I��
	CScene::Uninit();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTitle::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTitle::Draw(void)
{
	// �V�[���̕`��
	CScene::Draw();
}

//=====================================================
//	���S�̈ړ���Ԃ̍X�V����
//=====================================================
void CTitle::UpdateLogoMove(void)
{
	// �^�C�}�[�����Z
	m_fCurTime += CManager::GetDeltaTime();

	// �o�ߎ����̊������v�Z
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, title::MOVE_TIME);

	// �^�C�g���̐F�𔽉f
	m_pLogo->SetCol(title::INIT_COL + (title::DIFF_COL * fRate));

	// �^�C�g���̈ʒu�𔽉f
	m_pLogo->SetPosition(title::INIT_POS + (title::DIFF_POS * fRate));
	m_pLogo->SetVtx();	// ���_���f

	if (m_fCurTime >= title::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �^�C�g���̐F��␳
		m_pLogo->SetCol(title::DEST_COL);

		// �^�C�g���̈ʒu��␳
		m_pLogo->SetPosition(title::DEST_POS);
		m_pLogo->SetVtx();	// ���_���f

		// �I�����ڂ̈ړ���Ԃɂ���
		m_state = STATE_SELECT_MOVE;
	}
}

//=====================================================
//	�I�����ڂ̈ړ���Ԃ̍X�V����
//=====================================================
void CTitle::UpdateSelectMove(void)
{
	// �^�C�}�[�����Z
	m_fCurTime += CManager::GetDeltaTime();

	// �I�����̈ړ�
	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �A�C�R���w�i���ꂼ��̌o�ߎ��Ԃ��v�Z
		float fRateTime = m_fCurTime - (select::PLUS_TIME * (float)i);
		universal::LimitNum(fRateTime, 0.0f, select::MOVE_TIME);	// �o�ߎ��Ԃ�␳

		// ���ꂼ��̌o�ߎ������犄�����v�Z
		float fRate = easing::InOutQuad(fRateTime, 0.0f, select::MOVE_TIME);

		// �I�����̈ʒu���v�Z
		D3DXVECTOR3 posInit = select::INIT_POS + (select::SPACE * (float)i);

		// �I�����̐F���v�Z
		D3DXCOLOR colCur = select::INIT_COL;
		colCur.a = select::INIT_ALPHA + (select::DIFF_ALPHA * fRate);	// ���݂̓����x��ݒ�

		// �I�����̈ʒu�𔽉f
		m_apSelect[i]->SetPosition(posInit + (select::DIFF_POS * fRate));
		m_apSelect[i]->SetVtx();	// ���_���f

		// �I�����̐F�𔽉f
		m_apSelect[i]->SetCol(colCur);
	}

	// �I�����̈ړ��␳
	if (m_fCurTime >= select::MOVE_TIME + select::PLUS_TIME * (SELECT_MAX - 1))
	{ // �S�I�����̑ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // �I�����̑������J��Ԃ�

			// �I�����̖ڕW�����ʒu���v�Z
			D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

			// �I�����̈ʒu��␳
			m_apSelect[i]->SetPosition(posDest);
			m_apSelect[i]->SetVtx();	// ���_���f

			// �I�����̐F��␳
			m_apSelect[i]->SetCol(select::DEST_COL);
		}

		// �I����Ԃɂ���
		m_state = STATE_SELECT;

		// �^�C�g���J�����ɂ���
		Camera::ChangeState(new CCameraStateTitle);
	}
}

//=====================================================
//	�I����Ԃ̍X�V����
//=====================================================
void CTitle::UpdateSelect(void)
{
	CInputManager* pInput = CInputManager::GetInstance();	// ���̓}�l�[�W���[���
	assert(pInput != nullptr);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	//--------------------------------------------------------
	//	�I��������
	//--------------------------------------------------------
	// �O��̑I������ۑ�
	m_nOldSelect = m_nCurSelect;

	// �I��������
	if (pInput->GetTrigger(CInputManager::BUTTON_TRIGGER_LEFT))
	{
		// ���ɑI�������炷
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}
	if (pInput->GetTrigger(CInputManager::BUTTON_TRIGGER_RIGHT))
	{
		// �E�ɑI�������炷
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}

	// �O��̑I��v�f�̐F�𔒐F�ɐݒ�
	m_apSelect[m_nOldSelect]->SetCol(select::DEFAULT_COL);

	// ���݂̑I��v�f�̐F�����F�ɐݒ�
	m_apSelect[m_nCurSelect]->SetCol(select::CHOICE_COL);

	//--------------------------------------------------------
	//	�I��������
	//--------------------------------------------------------
	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{
		// �I����Ԃɂ���
		m_state = STATE_END;

		// �t�F�[�h���̏ꍇ������
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr) { assert(false); return; }
		if (pFade->GetState() != CFade::FADE_NONE) { assert(false); return; }
		switch (m_nCurSelect)
		{ // �I�������Ƃ̏���
		case SELECT_GAME:

			// �Q�[���ɑJ�ڂ���
			pFade->SetFade(CScene::MODE_GAME);
			break;

		case SELECT_TUTORIAL:

			// �`���[�g���A���ɑJ�ڂ���
			pFade->SetFade(CScene::MODE_TUTORIAL);
			break;

		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
}
