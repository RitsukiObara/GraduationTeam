//*****************************************************
//
// �X�e�[�W���U���gUI�̏���[stageResultUI.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "stageResultUI.h"
#include "manager.h"
#include "polygon2D.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "UI.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// ���ڂ̕�
#define RESULT_HEIGHT	(0.05f)	// ���ڂ̍���
#define MOVE_FACT	(0.15f)	// �ړ����x
#define LINE_ARRIVAL	(0.05f)	// ���������Ƃ���邵�����l
#define LINE_UNINIT	(3.0f)	// �I������܂ł̂������l

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\caption03.png";	// �N���A���S�̃p�X
	const char* FAIL_LOGO_PATH = "data\\TEXTURE\\UI\\caption02.png";	// ���s���S�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CStageResultUI* CStageResultUI::m_pStageResultUI = nullptr;	// ���g�̃|�C���^

//====================================================
// �R���X�g���N�^
//====================================================
CStageResultUI::CStageResultUI():m_apResult(),m_aPosDest()
{
	m_RESULT = RESULT_CREAR;
	m_state = STATE_NONE;
	m_bSound = false;
}

//====================================================
// �f�X�g���N�^
//====================================================
CStageResultUI::~CStageResultUI()
{

}

//====================================================
// ��������
//====================================================
CStageResultUI* CStageResultUI::Create(void)
{
	if (m_pStageResultUI == nullptr)
	{
		m_pStageResultUI = new CStageResultUI;

		if (m_pStageResultUI != nullptr)
		{
			m_pStageResultUI->Init();
		}
	}

	return m_pStageResultUI;
}

//====================================================
// ����������
//====================================================
HRESULT CStageResultUI::Init(void)
{
	//�N���A�̎�
	if (m_RESULT == RESULT_CREAR)
	{
		// �N���A���S�̐���
		m_apResult[RESULT_CREAR] = CPolygon2D::Create(7);

		if (m_apResult[RESULT_CREAR] != nullptr)
		{
			m_apResult[RESULT_CREAR]->SetSize(300.0f, 100.0f);
			m_apResult[RESULT_CREAR]->SetPosition(D3DXVECTOR3(640.0f, 150.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CREAR_LOGO_PATH);
			m_apResult[RESULT_CREAR]->SetIdxTexture(nIdx);
			m_apResult[RESULT_CREAR]->SetVtx();
		}
	}

	//���s�̎�
	else if (m_RESULT == RESULT_FAIL)
	{
		// ���s���S�̐���
		m_apResult[RESULT_FAIL] = CPolygon2D::Create(7);

		if (m_apResult[RESULT_FAIL] != nullptr)
		{
			m_apResult[RESULT_FAIL]->SetSize(330.0f, 100.0f);
			m_apResult[RESULT_FAIL]->SetPosition(D3DXVECTOR3(640.0f, 150.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(FAIL_LOGO_PATH);
			m_apResult[RESULT_FAIL]->SetIdxTexture(nIdx);
			m_apResult[RESULT_FAIL]->SetVtx();
		}
	}

	m_state = STATE_IN;

	m_aPosDest[0].x = RESULT_WIDTH;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CStageResultUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apResult[nCnt] != nullptr)
		{
			m_apResult[nCnt]->Uninit();

			m_apResult[nCnt] = nullptr;
		}
	}

	m_pStageResultUI = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void CStageResultUI::Update(void)
{
	// ��ԊǗ�
	ManageState();
}

//====================================================
// ��ԊǗ�
//====================================================
void CStageResultUI::ManageState(void)
{
	// �I���t���O�p
	int nEnd = 0;

	// �|���S����ڕW�ʒu�Ɍ����킹��
	//�N���A�̎�
	if (m_RESULT == RESULT_CREAR)
	{
		if (m_apResult[RESULT_CREAR] != nullptr)
		{
			D3DXVECTOR3 pos = m_apResult[RESULT_CREAR]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			////D3DXVECTOR3 vecDiff = m_apResult[RESULT_CREAR] - pos;
			//float fDiffOld = vecDiff.x;

			//vecDiff *= MOVE_FACT;

			//vecDiff += pos;

			//m_apResult[RESULT_CREAR]->SetPosition(vecDiff);

			//m_apResult[RESULT_CREAR]->SetVtx();

			//float fDiff = m_aPosDest[RESULT_CREAR].x - vecDiff.x;

			//if (fDiffOld * fDiffOld >= LINE_ARRIVAL * LINE_ARRIVAL &&
			//	fDiff * fDiff < LINE_ARRIVAL * LINE_ARRIVAL &&
			//	RESULT_CREAR < RESULT_MAX - 1)
			//{// �������������l��艺�ɂȂ����牺�̂��̂𓮂���
			//	if (m_state == STATE_IN)
			//	{
			//		m_aPosDest[RESULT_CREAR + 1].x = RESULT_WIDTH;
			//	}
			//	else if (m_state == STATE_OUT)
			//	{
			//		m_aPosDest[RESULT_CREAR + 1].x = -RESULT_WIDTH;
			//	}
			//}

			//if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
			//	m_state == STATE_OUT)
			//{// �I���̃��C��
			//	nEnd++;
			//}
		}
	}

	if (nEnd == RESULT_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// �t�F�[�h���鏈��
//====================================================
void CStageResultUI::Fade(RESULT RESULT)
{
	CFade* pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}

	switch (RESULT)
	{
	case CStageResultUI::RESULT_CREAR:

		CGame::SetState(CGame::STATE_END);
		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CStageResultUI::RESULT_FAIL:

		m_state = STATE_OUT;
		m_aPosDest[0].x = -RESULT_WIDTH;

		break;
	default:
		break;
	}
}

//====================================================
// �`�揈��
//====================================================
void CStageResultUI::Draw(void)
{

}