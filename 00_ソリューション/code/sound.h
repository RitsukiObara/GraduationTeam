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
		LABEL_BGM_TUTORIAL,				// チュートリアルBGM
		LABEL_BGM_GAME01,				// ゲームBGM01
		LABEL_BGM_RESULT,				// リザルトBGM
		LABEL_BGM_OPTION,				// オプションBGM

		LABEL_SE_COUNTDOWN,				// カウントダウン
		LABEL_SE_COUNTSTART,			// 開始時スタート
		LABEL_SE_PENGUIN_JUMP,			// ペンギンジャンプ
		LABEL_SE_SEA,					// 静かな海（未）
		LABEL_SE_BREAK_ICE,				// 氷破壊
		LABEL_SE_SEAL_CRY,				// アザラシ鳴き声
		LABEL_SE_SEA_SPLASH_01,			// 敵が海から上がってきた（重ねその1）
		LABEL_SE_SEA_SPLASH_02,			// 敵が海から上がってきた（重ねその2）
		LABEL_SE_ICE_CHAIN,				// 氷同士ぶつかった
		LABEL_SE_DECISION,				// 決定音
		LABEL_SE_SELECT,				// 選択音
		LABEL_SE_PENGUIN_VOICE00,		// ペンギンの鳴き声1
		LABEL_SE_PENGUIN_VOICE01,		// ペンギンの鳴き声2
		LABEL_SE_FLAPPING_WING,			// ペンギンの羽ばたき音
		LABEL_SE_PENGUIN_WALKING,		// ペンギンの歩く音
		LABEL_SE_POLARBEAR_VOICE,		// シロクマ鳴き声
		LABEL_SE_POLARBEAR_ASSALT,		// シロクマ突撃
		LABEL_SE_ALBATROSS,				// アホウドリ鳴き声

		LABEL_SE_PAUSE_MENU,			// ポーズ
		LABEL_SE_PAUSE_ARROW,			// 移動
		LABEL_SE_PAUSE_ENTER00,			// 決定
		LABEL_SE_PAUSE_ENTER01,			// 決定

		LABEL_SE_GAME_START,			// ゲームスタート

		LABEL_SE_TITLE_FADE,			// タイトルフェード
		LABEL_SE_STAGE_ENTRY,			// ステージに入っていく音

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
		{ "data/SOUND/BGM/BGM_Result.wav",-1 },			// チュートリアル画面BGM（未）
		{ "data/SOUND/BGM/BGM_Game00.wav",-1 },			// ゲーム画面BGM
		{ "data/SOUND/BGM/BGM_Result.wav",-1 },			// リザルト画面BGM
		{ "data/SOUND/BGM/BGM_Option.wav",-1 },			// オプション画面BGM

		{ "data/SOUND/SE/SE_Count.wav",0 },				// カウントダウン
		{ "data/SOUND/SE/SE_Start.wav",0 },				// 開始時スタート
		{ "data/SOUND/SE/SE_Penguin_Jump.wav",0 },		// ペンギンジャンプ
		{ "data/SOUND/SE/SE_Sea.wav",0 },				// 静かな海
		{ "data/SOUND/SE/SE_Break_Ice.wav",0 },			// 氷破壊
		{ "data/SOUND/SE/SE_Seal_Cry.wav",0 },			// アザラシ鳴き声
		{ "data/SOUND/SE/SE_SeaSplash_01.wav",0 },		// 敵が海から上がってきた（重ねその1）
		{ "data/SOUND/SE/SE_SeaSplash_02.wav",0 },		// 敵が海から上がってきた（重ねその2）
		{ "data/SOUND/SE/SE_Ice_Union.wav",0 },			// 氷同士ぶつかった
		{ "data/SOUND/SE/SE_Decision.wav",0 },			// 決定音
		{ "data/SOUND/SE/SE_Select.wav",0 },			// 選択音
		{ "data/SOUND/SE/SE_Penguin_Voice00.wav",0 },	// ペンギン鳴き声1
		{ "data/SOUND/SE/SE_Penguin_Voice01.wav",0 },	// ペンギン鳴き声2
		{ "data/SOUND/SE/SE_flapping_wing.wav",0 },		// ペンギンの羽ばたき音
		{ "data/SOUND/SE/SE_Walking_Sound.wav",0 },		// ペンギンの歩く音
		{ "data/SOUND/SE/SE_Polarbear_Voice.wav",0 },	// シロクマ鳴き声
		{ "data/SOUND/SE/SE_Polarbear_Assault.wav",0 },	// シロクマ突撃
		{ "data/SOUND/SE/SE_Albatross.wav",0 },			// アホウドリ鳴き声

		{ "data/SOUND/SE/pause.wav",0 },				// ポーズ
		{ "data/SOUND/SE/SE_ENTER00.wav",0 },			// 選択
		{ "data/SOUND/SE/SE_Enter01.wav",0 },			// 決定
		{ "data/SOUND/SE/SE_Enter02.wav",0 },			// 決定

		{ "data/SOUND/SE/SE_GameStart.wav",0 },			// ゲームスタート

		{ "data/SOUND/SE/SE_Fade.wav",0 },				// タイトルフェード音
		{ "data/SOUND/SE/SE_StageEntry.wav",-1 },				// タイトルフェード音
	};

	static CSound *m_pSound;	// 自身のポインタ
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif