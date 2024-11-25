//*****************************************************
//
// �`���[�g���A���}�l�[�W���[[tutorialManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorialManager.h"
#include "tutorial.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CTutorialManager::FuncUpdateState CTutorialManager::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,	// ���ł��Ȃ����
	nullptr,	// �ʏ���
	nullptr,	// �I�����
};

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorialManager::CTutorialManager()
{

}

//=====================================================
// ��������
//=====================================================
CTutorialManager *CTutorialManager::Create(void)
{
	CTutorialManager* pTutorialManager = nullptr;

	pTutorialManager = new CTutorialManager;

	if (pTutorialManager != nullptr)
		pTutorialManager->Init();

	return pTutorialManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorialManager::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorialManager::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorialManager::Update(void)
{
	CTutorial *pTutorial = CTutorial::GetInstance();

	if (pTutorial == nullptr)
		return;

	CTutorial::E_State state = pTutorial->GetState();

	assert(state > -1 && state < CTutorial::E_State::STATE_MAX);
	if (s_aFuncUpdateState[state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ
		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[state]))();
	}
}

//=====================================================
// �ړ���Ԃ̍X�V
//=====================================================
void CTutorialManager::UpdateMove(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CTutorialManager::Draw(void)
{

}