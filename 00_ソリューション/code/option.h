//*****************************************************
//
// オプション処理[option.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _OPTION_H_
#define _OPTION_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// クラスの定義
//*****************************************************
class COption : public CScene
{
public:
	// 設定中項目列挙
	enum OPTIONPARAM
	{
		PARAM_BGM = 0,
		PARAM_SE,
		PARAM_BIBE,
		PARAM_MAX
	};
	enum BIBESWITCH
	{
		BIBE_ON = 0,
		BIBE_OFF,
		BIBESWITCH_MAX
	};

	// サウンド系オブジェクト構造体
	struct SoundUIObject
	{
		CUI *text;		// テキスト
		CUI *bar;		// バー
		CUI *point;		// 現在値
		CUI *penguin;	// ペンギン
	};

	COption();	// コンストラクタ
	~COption();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	typedef void (COption::*SettingFunc)(void);

	// メンバ関数
	void CreateUIAll(void);			// UI生成全体
	void CreateSoundUIObj(void);	// サウンド系UIオブジェクト生成
	void CreateBibeUIObj(void);		// 振動UIオブジェクト生成
	void Input(void);				// 入力系処理
	void ModeSelectBack(void);		// モード選択画面に戻る
	void Select(void);				// 項目選択
	void MoveSelect(void);			// 項目動かす
	void SettingBGM(void);			// BGM設定
	void SettingSE(void);			// SE設定
	void SettingSound(CUI* pUI, float* volumeBase, float volumeAfter);
	void SettingBibe(void);			// 振動有無設定
	void MoveBGM(void);				// BGMのUI動かす
	void MoveSE(void);				// SEのUI動かす
	void MoveBibe(void);			// 振動有無のUI動かす
	void ColChangeBibe(void);		// 振動有無の色変更
	void BackSelect(void);			// 選択状態に戻る
	void CreateSingleUI(CUI** ppUI, string path, D3DXVECTOR3 pos, float width, float height);
	float ScaleChange(float* angle,float speed, float range);	// サイズ変更

	// メンバ変数
	// オブジェクト系
	CUI *m_pOptionText;							// 「オプション」のテキスト
	SoundUIObject m_aSoundUIObj[PARAM_MAX - 1];	// サウンド系UIオブジェクト
	CUI *m_pBibeText;							// 振動のテキスト
	CUI *m_apBibePenguin[BIBESWITCH_MAX];		// 振動有無UIのペンギン
	CUI *m_pButtonUI;							// 戻るボタンのUI
	
	// 値系
	static SettingFunc s_settingFunc[PARAM_MAX];	// 設定項目の関数に飛ぶ関数ポインタ
	OPTIONPARAM m_optionParam;	// 設定中項目
	float m_fOptionTextScale;	// 設定中項目の拡縮（0～2PI）
	float m_fBGMVolume;			// BGMボリューム
	float m_fSEVolume;			// SEボリューム
	BIBESWITCH m_Bibe;			// バイブ有無
	float m_fParamScale;		// 設定項目の拡縮（0～2PI）
	bool m_bSetting;			// 設定中フラグ
};

#endif