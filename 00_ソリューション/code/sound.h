//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���R����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM_TITLE,				// �^�C�g��BGM
		LABEL_BGM_GAME01,				// �Q�[��BGM01
		LABEL_BGM_RESULT,				// ���U���gBGM

		LABEL_SE_PAUSE_MENU,			// �|�[�Y
		LABEL_SE_PAUSE_ARROW,			// �ړ�
		LABEL_SE_PAUSE_ENTER00,			// ����
		LABEL_SE_PAUSE_ENTER01,			// ����

		LABEL_SE_ATTACK00,				// ���U��
		LABEL_SE_ATTACK01,				// ���U��
		LABEL_SE_ATTACK02,				// ��U��
		LABEL_SE_Guard00,				// �K�[�h00
		LABEL_SE_Guard01,				// �K�[�h01

		LABEL_SE_GAME_START,			// �Q�[���X�^�[�g
		LABEL_MAX
	} LABEL;
	typedef enum
	{
		FADESTATE_NONE = 0,	//�������Ă��Ȃ����
		FADESTATE_IN,	//�t�F�[�h�C�����
		FADESTATE_OUT,	//�t�F�[�h�A�E�g���
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// �R���X�g���N�^
	~CSound();	// �f�X�g���N�^

	static CSound *Create(HWND hWnd);
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

	void SetVolume(LABEL label, float fVolume);
	void SetFade(LABEL LabelNow, LABEL LabelNext, float fSpeed);

	static CSound *GetInstance(void) { return m_pSound; }

private:
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^�T�C�Y
	FADESTATE m_fadeSound;	// �t�F�[�h���
	LABEL m_SoundLabelNow;	// ���݂̃T�E���h���x��
	LABEL m_SoundLabelNext;	// ���̃T�E���h���x��
	float m_fSpeedFadeSound;	// �T�E���h���t�F�[�h����X�s�[�h
	float m_fVolumeSound;	// �T�E���h�̃{�����[��

	// �T�E���h�̏��
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/BGM_Title.wav",-1 },			// �^�C�g�����BGM
		{ "data/SOUND/BGM/BGM_Game04.wav",-1 },			// �Q�[�����BGM01
		{ "data/SOUND/BGM/BGM_Result.wav",-1 },			// �Q�[�����BGM01

		{ "data/SOUND/SE/pause.wav",0 },				// �|�[�Y
		{ "data/SOUND/SE/SE_ENTER00.wav",0 },			// �I��
		{ "data/SOUND/SE/SE_Enter01.wav",0 },			// ����
		{ "data/SOUND/SE/SE_Enter02.wav",0 },			// ����

		{ "data/SOUND/SE/SE_Attack00.wav",0 },			// ���U��
		{ "data/SOUND/SE/SE_Attack01.wav",0 },			// ���U��
		{ "data/SOUND/SE/SE_Attack02.wav",0 },			// ��U��
		{ "data/SOUND/SE/SE_Guard00.wav",0 },			// �K�[�h00
		{ "data/SOUND/SE/SE_Guard01.wav",0 },			// �K�[�h01

		{ "data/SOUND/SE/SE_GameStart.wav",0 },			// �Q�[���X�^�[�g
	};

	static CSound *m_pSound;	// ���g�̃|�C���^
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif