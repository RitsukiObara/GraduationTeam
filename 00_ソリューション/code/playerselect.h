//*****************************************************
//
// ゲーム処理[playerselect.h]
// Author:堀川萩大
//
//*****************************************************
#ifndef _PLAYERSELECT_H_
#define _PLAYERSELECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_PLAYER	(4)

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayerSelect;
class CUI;
class CPlayer;
class CInputManager;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerSelect : public CScene
{
public:
	enum E_StandyrState
	{// プレイヤーの準備状態
		STANDBY_WATE,
		STANDBY_OK,
		STANDBY_PLAY,
		STANDBY_MAX
	};

	CPlayerSelect();	// コンストラクタ
	~CPlayerSelect() {};	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

	// 変数取得・設定関数

private:
	// メンバ関数
	void Input(void);	// 操作
	void CreatePlayer(int nIdx);	// プレイヤーの生成
	void Ready(int nIdx);	// 準備
	void CheckStart(void);	// 開始するかの確認
	void StartFade(void);	// フェードの開始
	void Debug(void);

	// メンバ変数
	int m_nNumPlayer;	// プレイヤーの数
	E_StandyrState m_StandbyState[MAX_PLAYER];	// プレイヤーの状態
	CUI *m_apPlayerUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ
	CUI *m_apStateUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ
	CPlayer *m_apPlayer[MAX_PLAYER];// プレイヤーのポインタ
	CInputManager *m_apInputMgr[MAX_PLAYER];	// 入力マネージャーの配列

	// 静的メンバ変数
};

#endif