//*****************************************************
//
// �V���O�����U���g�̏���[resultSingle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultSingle.h"
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
		const float WIDTH = 0.5f;	// ��
		const float HEIGHT = 0.5f;	// ����
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �����̐F
		const float DEST_ALPHA = 0.5f;	// �ڕW�F
		const float DIFF_ALPHA = DEST_ALPHA - INIT_COL.a;			// �����A���t�@�l
		const D3DXVECTOR3 POS = D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �ʒu
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

}

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
CResultSingle::FuncUpdateState CResultSingle::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// �������Ȃ��X�V
	&CResultSingle::UpdateFade,		// �t�F�[�h��Ԃ̍X�V
	&CResultSingle::UpdateSelect,		// �I����Ԃ̍X�V
	nullptr,					// �I����Ԃ̍X�V
};

//====================================================
// �R���X�g���N�^
//====================================================
CResultSingle::CResultSingle()
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CResultSingle::~CResultSingle()
{

}

//====================================================
// ��������
//====================================================
CResultSingle *CResultSingle::Create(bool bWin)
{
	CResultSingle *pResult = nullptr;

	pResult = new CResultSingle;

	if (pResult != nullptr)
	{
		pResult->Init();

		pResult->Create2D(bWin);
	}

	return pResult;
}

//====================================================
// ����������
//====================================================
HRESULT CResultSingle::Init(void)
{
	// ������Ԃɂ���
	m_state = STATE_FADE;

	// �Q�[����ʂ����U���g��Ԃɂ���
	CGame::SetState(CGame::E_State::STATE_RESULT);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	// �T�E���h�̍Đ�
	pSound->Play(CSound::LABEL_BGM_RESULT);

	return S_OK;
}

//====================================================
// 2D�I�u�W�F�N�g�̐���
//====================================================
void CResultSingle::Create2D(bool bWin)
{
	//-----------------------------------------
	// �w�i�̐���
	//-----------------------------------------
	m_pBg = CUI::Create();

	if (m_pBg == nullptr)
		return;

	m_pBg->SetSize(bg::WIDTH, bg::HEIGHT);
	m_pBg->SetPosition(bg::POS);
	m_pBg->SetCol(bg::INIT_COL);
	m_pBg->SetVtx();
}

//====================================================
// �I������
//====================================================
void CResultSingle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pBg);
	Object::DeleteObject((CObject**)&m_pCaption);

	Release();
}

//====================================================
// �X�V����
//====================================================
void CResultSingle::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
//	�t�F�[�h��Ԃ̍X�V����
//=====================================================
void CResultSingle::UpdateFade(void)
{
	
}

//=====================================================
//	�I����Ԃ̍X�V����
//=====================================================
void CResultSingle::UpdateSelect(void)
{

}

//====================================================
// �`�揈��
//====================================================
void CResultSingle::Draw(void)
{

}