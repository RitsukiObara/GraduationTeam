//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高山桃也
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM_TITLE,				// タイトルBGM
		LABEL_BGM_GAME01,				// ゲームBGM01
		LABEL_BGM_RESULT,				// リザルトBGM

		LABEL_SE_PAUSE_MENU,			// ポーズ
		LABEL_SE_PAUSE_ARROW,			// 移動
		LABEL_SE_PAUSE_ENTER00,			// 決定
		LABEL_SE_PAUSE_ENTER01,			// 決定

		LABEL_SE_ATTACK00,				// 小攻撃
		LABEL_SE_ATTACK01,				// 中攻撃
		LABEL_SE_ATTACK02,				// 大攻撃
		LABEL_SE_Guard00,				// ガード00
		LABEL_SE_Guard01,				// ガード01

		LABEL_SE_GAME_START,			// ゲームスタート
		LABEL_MAX
	} LABEL;
	typedef enum
	{
		FADESTATE_NONE = 0,	//何もしていない状態
		FADESTATE_IN,	//フェードイン状態
		FADESTATE_OUT,	//フェードアウト状態
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

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
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];	// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];	// オーディオデータサイズ
	FADESTATE m_fadeSound;	// フェード状態
	LABEL m_SoundLabelNow;	// 現在のサウンドラベル
	LABEL m_SoundLabelNext;	// 次のサウンドラベル
	float m_fSpeedFadeSound;	// サウンドがフェードするスピード
	float m_fVolumeSound;	// サウンドのボリューム

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/BGM_Title.wav",-1 },			// タイトル画面BGM
		{ "data/SOUND/BGM/BGM_Game04.wav",-1 },			// ゲーム画面BGM01
		{ "data/SOUND/BGM/BGM_Result.wav",-1 },			// ゲーム画面BGM01

		{ "data/SOUND/SE/pause.wav",0 },				// ポーズ
		{ "data/SOUND/SE/SE_ENTER00.wav",0 },			// 選択
		{ "data/SOUND/SE/SE_Enter01.wav",0 },			// 決定
		{ "data/SOUND/SE/SE_Enter02.wav",0 },			// 決定

		{ "data/SOUND/SE/SE_Attack00.wav",0 },			// 小攻撃
		{ "data/SOUND/SE/SE_Attack01.wav",0 },			// 中攻撃
		{ "data/SOUND/SE/SE_Attack02.wav",0 },			// 大攻撃
		{ "data/SOUND/SE/SE_Guard00.wav",0 },			// ガード00
		{ "data/SOUND/SE/SE_Guard01.wav",0 },			// ガード01

		{ "data/SOUND/SE/SE_GameStart.wav",0 },			// ゲームスタート
	};

	static CSound *m_pSound;	// 自身のポインタ
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif