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
#include "manager.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace icon
{
const float WIDTH = 0.05f;							// ��
const float HEIGHT = 0.086f;						// ����
const D3DXVECTOR3 POS_INIT = { 0.35f,0.3f,0.0f };	// �����ʒu
const D3DXVECTOR3 POS_DEST[] =						// �ڕW�ʒu
{
	{ WIDTH,HEIGHT,0.0f },
	{ 1.0f - WIDTH,HEIGHT,0.0f },
	{ WIDTH,1.0f - HEIGHT,0.0f },
	{ 1.0f - WIDTH,1.0f - HEIGHT,0.0f },
};
const float TIME_MOVE = 3.0f;	// �ړ��ɂ����鎞��
const string PATH_TEX[] =		// �e�N�X�`���p�X
{
	"data\\TEXTURE\\UI\\playericon_00.png",
	"data\\TEXTURE\\UI\\playericon_01.png",
	"data\\TEXTURE\\UI\\playericon_02.png",
	"data\\TEXTURE\\UI\\playericon_03.png",
};
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIPlayer::FuncUpdateState CUIPlayer::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// ���ł��Ȃ����
	&CUIPlayer::UpdateScatter,	// �U��΂���
};

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_apIconPlayer(), m_state(E_State::STATE_NONE), m_fTimerScatter(0.0f)
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
		D3DXVECTOR3 pos = icon::POS_INIT;
		pos.x += icon::WIDTH * i * 2;
		m_apIconPlayer[i]->SetPosition(pos);
		m_apIconPlayer[i]->SetVtx();

		int nIdxTexture = Texture::GetIdx(&icon::PATH_TEX[i][0]);
		m_apIconPlayer[i]->SetIdxTexture(nIdxTexture);
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
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ
		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// �U��΂�n�߂鏈��
//=====================================================
void CUIPlayer::StartScatter(void)
{
	m_state = E_State::STATE_SCATTER;
}

//=====================================================
// �U��΂�X�V
//=====================================================
void CUIPlayer::UpdateScatter(void)
{
	if (m_fTimerScatter < icon::TIME_MOVE)
		m_fTimerScatter += CManager::GetDeltaTime();

	float fRate = easing::EaseOutExpo(m_fTimerScatter);

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (m_apIconPlayer[i] == nullptr)
			continue;

		D3DXVECTOR3 vecDiff = icon::POS_DEST[i] - icon::POS_INIT;

		D3DXVECTOR3 pos = icon::POS_INIT + vecDiff * fRate;

		m_apIconPlayer[i]->SetPosition(pos);
		m_apIconPlayer[i]->SetVtx();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CUIPlayer::Draw(void)
{

}