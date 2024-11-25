//*****************************************************
//
// �v�����[UI�̏���[UIplayer.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UIplayer.h"
#include "gameManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace icon
{
const float WIDTH = 0.05f;							// ��
const float HEIGHT = 0.086f;						// ����
const D3DXVECTOR3 POS_INIT = { 0.5f,0.3f,0.0f };	// �����ʒu
}
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_apIconPlayer()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIPlayer::~CUIPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CUIPlayer *CUIPlayer::Create()
{
	CUIPlayer *pUIPlayer = new CUIPlayer;

	if (pUIPlayer != nullptr)
		pUIPlayer->Init();

	return pUIPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIPlayer::Init(void)
{
	// �v���C���[�G���g���[�t���O�擾
	CGame::E_GameMode mode;
	vector<bool> abFrag;

	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])	// �t���O�������Ă��Ȃ�������ʂ�Ȃ�
			continue;

		m_apIconPlayer[i] = CUI::Create();

		if (m_apIconPlayer[i] == nullptr)
			continue;

		// UI�̏����ݒ�
		m_apIconPlayer[i]->SetSize(icon::WIDTH, icon::HEIGHT);
		m_apIconPlayer[i]->SetPosition(icon::POS_INIT);
		m_apIconPlayer[i]->SetVtx();
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIPlayer::Uninit(void)
{
	Object::DeleteObject((CObject**)m_apIconPlayer, NUM_PLAYER);

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIPlayer::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CUIPlayer::Draw(void)
{

}