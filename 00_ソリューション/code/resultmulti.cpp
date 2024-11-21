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
#include "npcpenguinstate_resultmulti.h"

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
	}
	namespace manual
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial00.jpg";	// �p�X
		const float WIDTH		= 0.5f;	// ��
		const float HEIGHT		= 0.5f;	// ����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// �ʒu
	}
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultMulti::CResultMulti()
{

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

	// ����UI����
	//m_pManual = CUI::Create();
	//if (m_pManual != nullptr)
	//{
	//	// �����̐ݒ�
	//	m_pManual->SetIdxTexture(CTexture::GetInstance()->Regist(manual::PATH));	// �e�N�X�`������
	//	m_pManual->SetPosition(manual::POS);				// �ʒu
	//	m_pManual->SetSize(manual::WIDTH, manual::HEIGHT);	// �傫��
	//	m_pManual->SetVtx();	// ���_���f
	//}

	// ���b�V���t�B�[���h
	CMeshField* pMeshField = CMeshField::Create();
	pMeshField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\MATERIAL\\ice000.jpg"));
	pMeshField->SetDivTex(128);

	// ���܂���
	CObjectX *pIgloo = CObjectX::Create();
	pIgloo->BindModel(CModel::Load("data\\MODEL\\object\\Snowdome.x"));
	pIgloo->SetPosition(D3DXVECTOR3(800.0f, -10.0f, 600.0f));

	// BGM�̍Đ�
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	// �e�y���M������
	CNPCPenguin* pPenguin = nullptr;
	// ���҃y���M��
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_BANZAI);
	pPenguin->SetPosition(Penguin::WINNER_POS);
	// �s�҃y���M��
	for (int cnt = 0; cnt < (int)Penguin::LOSER_POS.size(); cnt++)
	{
		pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Flee(D3DXVECTOR3(800.0f, -10.0f, 600.0f)));
		pPenguin->SetPosition(Penguin::LOSER_POS[cnt]);
		pPenguin->SetRotation(Penguin::LOSER_ROT[cnt]);
	}

	// �A�U���V����


	// ���̓}�l�[�W���[����
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CResultMulti::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pWinner);

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

		// �^�C�g���J��
		pFade->SetFade(CScene::MODE_TITLE);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_DECISION);
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

