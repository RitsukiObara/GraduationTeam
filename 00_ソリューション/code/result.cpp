//*****************************************************
//
// ���U���g�̏���[result.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "result.h"
#include "manager.h"
#include "ui.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "timer.h"
#include "ranking.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	namespace bg
	{
		const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// �p�X
		const float WIDTH		= 0.5f;	// ��
		const float HEIGHT		= 0.5f;	// ����
		const float	MOVE_TIME	= 1.5f;	// �ړ�����
		const float DEST_ALPHA	= 0.5f;	// �ڕW�F
		const float INIT_ALPHA	= 0.0f;	// �����F
		const float DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;			// �����F
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �ʒu
	}

	namespace caption
	{
		const float	MOVE_TIME	= 1.5f;	// �ړ�����
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;						// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f, 0.2f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// �����ʒu
		const float WIDTH	= 0.2f;		// ��
		const float HEIGHT	= 0.17f;	// ����
	}

	namespace curtime
	{
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.48f, 0.41f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// �����ʒu
	}

	namespace cont
	{
		const char* PATH = "data\\TEXTURE\\UI\\continue.png";	// �p�X
		const float	MOVE_TIME = 1.5f;	// �ړ�����

		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �ڕW�F
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// �����F
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// �����F
		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(0.5f, 0.6f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;						// �����ʒu

		const float WIDTH	= 0.3f;		// ��
		const float HEIGHT	= 0.12f;	// ����
	}

	namespace select
	{
		const char* PATH[] =
		{
			"data\\TEXTURE\\UI\\yes.png",	// �Q�[���I�����̃p�X
			"data\\TEXTURE\\UI\\no.png",	// �`���[�g���A���I�����̃p�X
		};

		const float	MOVE_TIME	= 0.84f;	// �ړ�����
		const float	PLUS_TIME	= 0.0f;		// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �ҋ@����
		const float WIDTH		= 0.4f  * 0.32f;	// ��
		const float HEIGHT		= 0.29f * 0.32f;	// ����
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
// �ÓI�����o�ϐ�
//*****************************************************
CResult *CResult::s_pResult = nullptr;	// ���g�̃|�C���^
CResult::AFuncUpdateState CResult::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// �������Ȃ��X�V
	&CResult::UpdateFade,		// �t�F�[�h��Ԃ̍X�V
	&CResult::UpdateClearMove,	// �N���A�󋵈ړ���Ԃ̍X�V
	&CResult::UpdateContMove,	// �R���e�j���[�ړ���Ԃ̍X�V
	&CResult::UpdateSelectMove,	// �I�����ڂ̈ړ���Ԃ̍X�V
	&CResult::UpdateSelect,		// �I����Ԃ̍X�V
	nullptr,					// �I����Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CResult::CResult()
{
	m_pBg = nullptr;
	m_pCaption = nullptr;
	m_pCurTime = nullptr;

	m_pContinue = nullptr;
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));

	m_state = STATE_NONE;
	m_fCurTime = 0.0f;	// ���݂̑ҋ@����
	m_nCurSelect = 0;				// ���݂̑I����
	m_nOldSelect = 0;				// �O��̑I����
}

//====================================================
// �f�X�g���N�^
//====================================================
CResult::~CResult()
{

}

//====================================================
// ��������
//====================================================
CResult *CResult::Create(bool bWin)
{
	if (s_pResult == nullptr)
	{
		s_pResult = new CResult;
		if (s_pResult != nullptr)
		{
			// �o�ߎ��Ԃ������o��
			CTimer::SaveSecond(CGame::GetInstance()->GetTimeSecond());

			s_pResult->Init();

			s_pResult->Create2D(bWin);

			float fTime = CGame::MAX_TIME - CGame::GetInstance()->GetTimeSecond();
			if (bWin == false)
			{
				fTime = 60;
			}

			CRanking::SetTime(fTime);
		}
	}

	return s_pResult;
}

//====================================================
// 2D�I�u�W�F�N�g�̐���
//====================================================
void CResult::Create2D(bool bWin)
{
	char *pPathCaption[2] =
	{
		"data\\TEXTURE\\UI\\lose.png",
		"data\\TEXTURE\\UI\\win.png",
	};

	// �w�i�̐���
	m_pBg = CUI::Create();
	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(bg::POS);
		m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_pBg->SetVtx();
	}

	// ���o���̐���
	m_pCaption = CUI::Create();
	if (m_pCaption != nullptr)
	{
		m_pCaption->SetIdxTexture(CTexture::GetInstance()->Regist(pPathCaption[bWin]));
		m_pCaption->SetPosition(caption::INIT_POS);
		m_pCaption->SetSize(caption::WIDTH, caption::HEIGHT);
		m_pCaption->SetCol(caption::INIT_COL);
		m_pCaption->SetVtx();
	}

	// �N���A���Ԃ̐���
	m_pCurTime = CTimer::Create();
	if (m_pCurTime != nullptr)
	{
		m_pCurTime->SetSecond(CGame::MAX_TIME - CTimer::LoadSecond());
		m_pCurTime->SetPosition(curtime::INIT_POS);
		m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::INIT_COL);
	}

	// ���S�̐���
	m_pContinue = CUI::Create();
	if (m_pContinue != nullptr)
	{
		// ���S�̐ݒ�
		m_pContinue->SetIdxTexture(CTexture::GetInstance()->Regist(cont::PATH));	// �e�N�X�`������
		m_pContinue->SetPosition(cont::INIT_POS);			// �ʒu
		m_pContinue->SetSize(cont::WIDTH, cont::HEIGHT);	// �傫��
		m_pContinue->SetCol(cont::INIT_COL);
		m_pContinue->SetVtx();								// ���_���f
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
}

//=====================================================
//	�t�F�[�h��Ԃ̍X�V����
//=====================================================
void CResult::UpdateFade(void)
{
	// �^�C�}�[�����Z
	m_fCurTime += CManager::GetDeltaTime();

	// �o�ߎ����̊������v�Z
	float fRate = easing::InQuad(m_fCurTime, 0.0f, bg::MOVE_TIME);

	// �t�F�[�h�̓����x�𔽉f
	m_pBg->SetAlpha(bg::INIT_ALPHA + (bg::DIFF_ALPHA * fRate));

	if (m_fCurTime >= bg::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �t�F�[�h�̓����x��␳
		m_pBg->SetAlpha(bg::DEST_ALPHA);

		// �����o����Ԃɂ���
		m_state = STATE_CLEAR_MOVE;
	}
}

//=====================================================
//	�N���A�󋵈ړ���Ԃ̍X�V����
//=====================================================
void CResult::UpdateClearMove(void)
{
	// �^�C�}�[�����Z
	m_fCurTime += CManager::GetDeltaTime();

	// �o�ߎ����̊������v�Z
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, caption::MOVE_TIME);

	m_pCaption->SetCol(caption::INIT_COL + (caption::DIFF_COL * fRate));
	m_pCaption->SetPosition(caption::INIT_POS + (caption::DIFF_POS * fRate));
	m_pCaption->SetVtx();	// ���_���f

	m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::INIT_COL + (caption::DIFF_COL * fRate));
	m_pCurTime->SetPosition(curtime::INIT_POS + (curtime::DIFF_POS * fRate));

	if (m_fCurTime >= caption::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		m_pCaption->SetCol(caption::DEST_COL);
		m_pCaption->SetPosition(caption::DEST_POS);
		m_pCaption->SetVtx();	// ���_���f

		m_pCurTime->SetColor(CTimer::NUMBER_MAX, caption::DEST_COL);
		m_pCurTime->SetPosition(curtime::DEST_POS);

		// �����o����Ԃɂ���
		m_state = STATE_CONT_MOVE;
	}
}

//=====================================================
//	�R���e�j���[�ړ���Ԃ̍X�V����
//=====================================================
void CResult::UpdateContMove(void)
{
	// �^�C�}�[�����Z
	m_fCurTime += CManager::GetDeltaTime();

	// �o�ߎ����̊������v�Z
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, cont::MOVE_TIME);

	m_pContinue->SetCol(cont::INIT_COL + (cont::DIFF_COL * fRate));
	m_pContinue->SetPosition(cont::INIT_POS + (cont::DIFF_POS * fRate));
	m_pContinue->SetVtx();	// ���_���f

	if (m_fCurTime >= cont::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		m_pContinue->SetCol(cont::DEST_COL);
		m_pContinue->SetPosition(cont::DEST_POS);
		m_pContinue->SetVtx();	// ���_���f

		// �����o����Ԃɂ���
		m_state = STATE_SELECT_MOVE;
	}
}

//=====================================================
//	�I�����ڂ̈ړ���Ԃ̍X�V����
//=====================================================
void CResult::UpdateSelectMove(void)
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
	}
}

//=====================================================
//	�I����Ԃ̍X�V����
//=====================================================
void CResult::UpdateSelect(void)
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
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{
		// ���ɑI�������炷
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}
	if (pInput->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
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
		// �t�F�[�h���̏ꍇ������
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr) { assert(false); return; }
		if (pFade->GetState() != CFade::FADE_NONE) { assert(false); return; }
		switch (m_nCurSelect)
		{ // �I�������Ƃ̏���
		case SELECT_YES:

			// �Q�[���ɑJ�ڂ���
			pFade->SetFade(CScene::MODE_GAME);
			break;

		case SELECT_NO:

			// �����L���O�ɑJ�ڂ���
			pFade->SetFade(CScene::MODE_RANKING);
			break;

		default:
			assert(false);
			break;
		}

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
}

//====================================================
// ����������
//====================================================
HRESULT CResult::Init(void)
{
	// ������Ԃɂ���
	m_state = STATE_FADE;

	// �Q�[����ʂ����U���g��Ԃɂ���
	CGame::SetState(CGame::STATE::STATE_RESULT);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	// �T�E���h�̍Đ�
	pSound->Play(CSound::LABEL_BGM_RESULT);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CResult::Uninit(void)
{
	// ���g�̃A�h���X��j��
	s_pResult = nullptr;

	Object::DeleteObject((CObject**)&m_pBg);
	Object::DeleteObject((CObject**)&m_pCaption);

	Release();
}

//====================================================
// �X�V����
//====================================================
void CResult::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))();
	}
}

//====================================================
// �`�揈��
//====================================================
void CResult::Draw(void)
{

}