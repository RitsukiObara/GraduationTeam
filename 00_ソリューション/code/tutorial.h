//*****************************************************
//
// チュートリアル処理[tutorial.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CTutorialManager;
class CUIPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorial : public CScene
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,			// 何でもない状態
		STATE_MOVE,				// 移動状態
		STATE_PECK,				// 突っつき状態
		STATE_EXPLAIN_BREAK,	// 破壊説明
		STATE_EXPLAIN_ENEMY,	// 敵説明
		STATE_END,				// 終了状態
		STATE_MAX
	};
	CTutorial();	// コンストラクタ
	~CTutorial();	// デストラクタ

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	void ProgressState(void);	// チュートリアルを進める

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	int GetCntProgress(void) { return m_nCntProgress; }	// 状態進行カウンター
	void AddCntProgress(CPlayer *pPlayer);

	// 静的メンバ関数
	static CTutorial *GetInstance(void) { return s_pTutorial; }	// インスタンス取得

private:
	// メンバ関数
	void UpdateEnd(void);		// 終了状態
	void CheckProgress(void);	// 進行判断
	void CreateCheck(int nIdx);	// チェックマーク生成
	void Debug(void);			// デバッグ

	// メンバ変数
	E_State m_state;				// 状態
	CTutorialManager *m_pManager;	// チュートリアルマネージャーのポインタ
	CUIPlayer *m_pUIPlayer;			// プレイヤーUIのポインタ
	float m_fTimeEnd;				// 終了タイマー
	int m_nCntProgress;				// 進行カウンター
	CUI *m_pCaption;				// キャプション
	float m_fTimerCaption;			// キャプションのタイマー
	vector<CUI*> m_apCheck;			// チェックマークの配列
	bool m_abComplete[NUM_PLAYER];	// 完了フラグ

	// 関数ポインタ型を定義
	typedef void (CTutorial::*FuncUpdateState)(void);

	// 静的メンバ変数
	static FuncUpdateState s_aFuncUpdateState[];	// 状態更新関数
	static CTutorial *s_pTutorial;					// 自身のポインタ
};

#endif