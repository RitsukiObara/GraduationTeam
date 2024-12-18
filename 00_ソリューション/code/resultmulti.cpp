//*****************************************************
//
// �}���`���[�h�̃��U���g����[resultmulti.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultmulti.h"
#include "inputManager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "CameraState.h"
#include "sound.h"
#include "objectX.h"
#include "meshfield.h"
#include "UI.h"
#include "gameManager.h"
#include "npcpenguin.h"
#include "npcpenguinstate_resultmulti.h"
#include "resultseal.h"
#include "snow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace Penguin
{
const D3DXVECTOR3 WINNER_POS = D3DXVECTOR3(800.0f, 0.0f, -1800.0f);	// ���҂̈ʒu
const vector<D3DXVECTOR3> LOSER_POS =	// �s�҂̈ʒu
{
	D3DXVECTOR3(800.0f, 0.0f, -500.0f),
	D3DXVECTOR3(800.0f, 0.0f, -700.0f),
	D3DXVECTOR3(800.0f, 0.0f, -900.0f)
};
const vector<D3DXVECTOR3> LOSER_ROT =	// �s�҂̌���
{
	D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f),
	D3DXVECTOR3(0.0f, 0.5471f * D3DX_PI, 0.0f),
	D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f)
};
const int MAX_PLAYER = 4;	// �ő�l��
}

namespace Map
{
const string FIELD_TEX_PATH = "data\\TEXTURE\\MATERIAL\\field.jpg";	// �n�ʃe�N�X�`���p�X
const string SNOWDOME_PATH = "data\\MODEL\\object\\Snowdome.x";		// ���܂��烂�f���p�X
}

namespace UI
{
const string NUMBER_PATH = "data\\TEXTURE\\UI\\player_Count.png";	// ��P�Ƃ��̃e�N�X�`��
const string TEXT_PATH = "data\\TEXTURE\\UI\\win.png";				// �u�̏����v�̃e�N�X�`��
const float NUMBER_WIDTH = 0.1f;	// ���i��P�j
const float NUMBER_HEIGHT = 0.15f;	// �����i��P�j
const float TEXT_WIDTH = 0.15f;		// ���i�u�̏����v�j
const float TEXT_HEIGHT = 0.15f;	// �����i�u�̏����v�j
const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(0.3125f, 0.85f, 0.0f);	// ��P�Ƃ��̈ʒu
const D3DXVECTOR3 TEXT_POS = D3DXVECTOR3(0.55f, 0.85f, 0.0f);		// �u�̏����v�̈ʒu
const int MAX_PLAYER = 4;
}

//--------------------------------
// ���������̒萔
//--------------------------------
namespace draw
{
const string PATH = "data\\TEXTURE\\UI\\draw.png";			// ��������
const float WIDTH = 0.15f;									// ��
const float HEIGHT = 0.10f;									// ����
const D3DXVECTOR3 POS = D3DXVECTOR3(0.5f, 0.85f, 0.0f);		// �ʒu
}
//----------------------------
// �{�^��UI�萔
//----------------------------
namespace buttonUI
{
	const string PATH = "data\\TEXTURE\\UI\\A_Move.png";
	const float WIDTH = 0.09f;
	const float HEIGHT = 0.049f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
}

namespace Snow
{
	const int CNT_SNOW = 5;						// �Ⴊ�~��^�C�~���O
	const float MAX_HEIGHT = 1800.0f;				// �Ⴊ�~���Ă��鍂��
	const int MAX_SNOW_RADIUS = 100;				// �ő��̔��a
	const int MIN_SNOW_RADIUS = 20;					// �Œ��̔��a
	const int SNOW_LIFE = 200;						// ��̃��C�t
	const float SNOW_MOVE_SPED = -10.0f;			// ��̈ړ����x
	const int MAX_SNOW_POS = 2200;					// ��̍ő吶���ꏊ
	const int MIN_SNOW_POS = -2200;					// ��̍Œᐶ���ꏊ
	const int BLIZZARD_CNT = 800;					// ���Ⴊ����܂ł̃J�E���g
}

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultMulti::CResultMulti()
{
	m_pWinnerNum = nullptr;
	m_pWinnerText = nullptr;
	m_pButtonUI = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResultMulti::~CResultMulti()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CResultMulti::Init(void)
{
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
		return E_FAIL;

	// �J����
	Camera::ChangeState(new CCameraStateResultMulti);

	// ���b�V���t�B�[���h
	CMeshField* pMeshField = CMeshField::Create();
	if (pMeshField != nullptr)
	{
		pMeshField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist(&Map::FIELD_TEX_PATH[0]));
		pMeshField->SetDivTex(128);
	}

	// ���܂���
	CObjectX *pIgloo = CObjectX::Create();
	if (pIgloo != nullptr)
	{
		pIgloo->BindModel(CModel::Load(&Map::SNOWDOME_PATH[0]));
		pIgloo->SetPosition(D3DXVECTOR3(800.0f, -10.0f, 600.0f));
	}

	// �{�^��UI
	m_pButtonUI = CUI::Create();
	if (m_pButtonUI != nullptr)
	{
		// �ݒ�
		m_pButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&buttonUI::PATH[0]));	// �e�N�X�`������
		m_pButtonUI->SetPosition(buttonUI::POS);					// �ʒu
		m_pButtonUI->SetSize(buttonUI::WIDTH, buttonUI::HEIGHT);	// �傫��
		m_pButtonUI->SetVtx();	// ���_���f
	}

	// BGM�̍Đ�
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	// �ΐ팋�ʎ擾
	int playerNum = Penguin::MAX_PLAYER;
	int winner = 0;
	vector<bool> enablePlayer;
	gameManager::LoadWinner(&playerNum, &winner);

	// ���������t���O�̐ݒ�
	m_bDraw = winner == -1;

	if (!m_bDraw)	// �ʏ�̏�����
		InitNormal(winner, playerNum);
	else			// ���������̏�����
		InitDraw();

	// ���̓}�l�[�W���[����
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// �ʏ�̏�����
//=====================================================
void CResultMulti::InitNormal(int nIdxWinner, int nPlayerNum)
{
	//-----------------------------------
	// ���Ҕԍ�UI����
	//-----------------------------------
	SetUI(&m_pWinnerNum, UI::NUMBER_PATH, UI::NUMBER_POS, UI::NUMBER_WIDTH, UI::NUMBER_HEIGHT);
	SetUI(&m_pWinnerText, UI::TEXT_PATH, UI::TEXT_POS, UI::TEXT_WIDTH, UI::TEXT_HEIGHT);

	//-----------------------------------
	// ���ݒ�
	//-----------------------------------
	vector<bool> enablePlayer;

	for (int cnt = 0; cnt < Penguin::MAX_PLAYER; cnt++)
	{
		if (cnt < nPlayerNum)
		{
			enablePlayer.push_back(true);
		}
		else
		{
			enablePlayer.push_back(false);
		}
	}

	// ���Ҕԍ��ݒ�
	if (m_pWinnerNum != nullptr)
	{
		m_pWinnerNum->SetTex(D3DXVECTOR2((float)(nIdxWinner) / UI::MAX_PLAYER, 0.0f), D3DXVECTOR2((float)(nIdxWinner + 1) / UI::MAX_PLAYER, 1.0f));
		m_pWinnerNum->SetVtx();
	}

	//-----------------------------------
	// �L�����N�^�[����
	//-----------------------------------
	// �e�y���M������
	CNPCPenguin* pPenguin = nullptr;
	// ���҃y���M��
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_BANZAI, (CNPCPenguin::SKIN)nIdxWinner);
	pPenguin->SetPosition(Penguin::WINNER_POS);
	enablePlayer[nIdxWinner] = false;	// ���҂�\���ς݂Ƃ���false�ɂ��Ă���
	// �s�҃y���M��
	int loser = 0;
	for (int cnt = 0; cnt < Penguin::MAX_PLAYER; cnt++)
	{
		if (enablePlayer[cnt] == true)
		{// �܂��\�����Ă��Ȃ�
			pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Flee(D3DXVECTOR3(800.0f, -10.0f, 600.0f)), (CNPCPenguin::SKIN)cnt);
			pPenguin->SetPosition(Penguin::LOSER_POS[loser]);
			pPenguin->SetRotation(Penguin::LOSER_ROT[loser]);
			loser++;
		}
	}

	// �A�U���V����
	CResultSeal* pSeal = CResultSeal::Create(D3DXVECTOR3(800.0f, -10.0f, 600.0f));
	pSeal->SetPosition(D3DXVECTOR3(1200.0f, 0.0f, -900.0f));
	pSeal->SetRotation(D3DXVECTOR3(0.0f, 0.4f * D3DX_PI, 0.0f));
}

//=====================================================
// ���������̏�����
//=====================================================
void CResultMulti::InitDraw(void)
{
	SetUI(&m_pWinnerText, draw::PATH, draw::POS, draw::WIDTH, draw::HEIGHT);
}

//=====================================================
// �I������
//=====================================================
void CResultMulti::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pWinnerNum);
	Object::DeleteObject((CObject**)&m_pWinnerText);
	Object::DeleteObject((CObject**)&m_pButtonUI);
	
	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();

	// �V�[���̏I��
	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CResultMulti::Update(void)
{
	CInputManager *pInputMgr = CInputManager::GetInstance();
	assert(pInputMgr != nullptr);

	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	// ����
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		// �t�F�[�h���̏ꍇ������
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		// �X�e�[�W�v���C���[�ɑJ��
		pFade->SetFade(CScene::MODE_SELECTPLAYER);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	// ��~�炷����
	if (m_bDraw)
	{
		CSnow::SetSnow(Snow::MAX_SNOW_POS, Snow::MIN_SNOW_POS, Snow::MAX_HEIGHT, Snow::MAX_SNOW_RADIUS, Snow::MIN_SNOW_RADIUS,
			Snow::SNOW_LIFE, D3DXVECTOR3(0.0f, Snow::SNOW_MOVE_SPED, 0.0f), Snow::CNT_SNOW);
	}

	// �V�[���̍X�V
	CScene::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CResultMulti::Draw(void)
{
	// �V�[���̕`��
	CScene::Draw();
}

//=====================================================
// UI�z�u����
//=====================================================
void CResultMulti::SetUI(CUI** pUI, string path, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	*pUI = CUI::Create();
	if (*pUI != nullptr)
	{// �����̐ݒ�
		(*pUI)->SetIdxTexture(CTexture::GetInstance()->Regist(&path[0]));	// �e�N�X�`������
		(*pUI)->SetPosition(pos);		// �ʒu
		(*pUI)->SetSize(fWidth, fHeight);	// �傫��
		(*pUI)->SetVtx();	// ���_���f
	}
	else
	{
		assert(false);
	}
}

