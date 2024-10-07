//*****************************************************
//
// �V�[������[scene.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"
#include "game.h"
#include "logo.h"
#include "title.h"
#include "tutorial.h"
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "sound.h"

#include "texture.h"
#include "objectX.h"
#include "meshcylinder.h"
#include "polygon3D.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CScene::CScene()
{
	m_nTimerTrans = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CScene::~CScene()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CScene::Init(void)
{
	// �}�b�v���f���̐���
	CObjectX* pMapX = CObjectX::Create();

	if (pMapX)
	{
		int nIdx = CModel::Load("data\\MODEL\\block\\map01.x");
		pMapX->BindModel(nIdx);
	}

	//	�y�U���f���̐���
	CObjectX* pDohyou = CObjectX::Create();

	if (pDohyou)
	{
		int nIdx = CModel::Load("data\\MODEL\\dohyou\\dohyou01.x");
		pDohyou->BindModel(nIdx);
		pDohyou->SetRotation({0.0f, 1.57f, 0.0f});
	}

	// �R�V�����_�[�̐���
	CMeshCylinder* pCylinder = CMeshCylinder::Create();

	if (pCylinder)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\mountain000.png");
		pCylinder->SetIdxTexture(nIdx);
		pCylinder->SetRadius(6000.0f);
		pCylinder->SetHeight(5000.0f);
		pCylinder->SetVtx();
	}

	// �n�ʃ|���S���̐���
	CPolygon3D* pPoly = CPolygon3D::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.57f });

	if (pPoly)
	{
		int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\BG\\field00.jpg");
		pPoly->SetIdxTexture(nIdx);
		pPoly->SetSize(20000.0f, 20000.0f);
		pPoly->SetVtx();
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CScene::Uninit(void)
{
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
		pSound->Stop();

	CLight::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CScene::Update(void)
{
	// �����_���[�̎擾
	CRenderer *pRenderer = CRenderer::GetInstance();
	if (pRenderer != nullptr)
	{
		// �����_���[�̍X�V
		pRenderer->Update();
	}

	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();
	if (pCamera != nullptr)
	{
		// �J�����̍X�V
		pCamera->Update();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CScene::Draw(void)
{

}

//=====================================================
// �V�[���̐�������
//=====================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	switch (mode)
	{
	case CScene::MODE_LOGO:

		pScene = new CLogo;

		break;
	case CScene::MODE_TITLE:

		pScene = new CTitle;

		break;
	case CScene::MODE_TUTORIAL:

		pScene = new CTutorial;

		break;
	case CScene::MODE_GAME:

		pScene = new CGame;

		break;
	case CScene::MODE_RANKING:

		pScene = new CRanking;

		break;
	default:

		pScene = new CGame;

		break;
	}

	if (pScene != nullptr)
	{// �V�[������
		pScene->Init();
	}

	return pScene;
}