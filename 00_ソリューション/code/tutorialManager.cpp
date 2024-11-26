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
	&CTutorialManager::UpdateMove,		// �ړ����
	&CTutorialManager::UpdatePeck,		// �˂������
	&CTutorialManager::UpdateBreak,		// �j����
	&CTutorialManager::UpdateExplain,	// �������
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

}

//=====================================================
// �ړ���Ԃ̍X�V
//=====================================================
void CTutorialManager::UpdateMove(void)
{

}

//=====================================================
// �˂�����Ԃ̍X�V
//=====================================================
void CTutorialManager::UpdatePeck(void)
{

}

//=====================================================
// �j���Ԃ̍X�V
//=====================================================
void CTutorialManager::UpdateBreak(void)
{

}

//=====================================================
// ������Ԃ̍X�V
//=====================================================
void CTutorialManager::UpdateExplain(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CTutorialManager::Draw(void)
{

}

//=====================================================
// ��Ԃ̕ύX
//=====================================================
void CTutorialManager::ChangeState(CTutorial::E_State stateNext)
{
	CTutorial *pTutorial = CTutorial::GetInstance();
	if (pTutorial == nullptr)
		return;


}