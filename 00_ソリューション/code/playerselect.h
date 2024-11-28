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
class CMeshCylinder;
class CFan3D;

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
	virtual void Update(void);
	virtual void Draw(void);

private:
	// メンバ関数
	void CreateMesh(void);			// メッシュの生成
	void CreatePlayer(int nIdx);	// プレイヤーの生成
	void Input(void);				// 操作
	void LimitPlayerPos(void);		// プレイヤーの位置制限
	void GravityPlayer(void);		// プレイヤーの重力処理
	void Ready(int nIdx);			// 準備
	void CheckStart(void);			// 開始するかの確認
	void StartFade(void);			// フェードの開始
	void Debug(void);				// デバッグ処理

	// メンバ変数
	int m_nNumPlayer;	// プレイヤーの数
	E_StandyrState m_StandbyState[MAX_PLAYER];	// プレイヤーの状態
	CUI *m_apPlayerUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ
	CUI *m_apStateUI[MAX_PLAYER];	// プレイヤー用2Dオブジェクトのポインタ
	CPlayer *m_apPlayer[MAX_PLAYER];// プレイヤーのポインタ
	CInputManager *m_apInputMgr[MAX_PLAYER];	// 入力マネージャーの配列
	CMeshCylinder *m_pCylinder;	// 側面のシリンダー
	CFan3D *m_pFan;				// 上側の円ポリゴン

	// 静的メンバ変数
};

#endif