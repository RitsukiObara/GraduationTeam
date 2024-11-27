//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPolygon3D;
class CIce;
class CInputManager;
class CFlowIce;
class CPeckLine;
class CShadow;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	// モーション列挙
	enum MOTION
	{
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_STARTJUMP,
		MOTION_STAYJUMP,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_FLOW,
		MOTION_VICTORY,
		MOTION_FAIL,
		MOTION_GUTS,
		MOTION_NECKSHAKE,
		MOTION_UNYO,	// 首うね
		MOTION_CANNOTPECK,
		MOTION_MULTIAPPEAR,
		MOTION_WINGPTPT,	// 羽根パタパタ
		MOTION_STOMACH,	// 腹ベタ
		MOTION_FALL,
		MOTION_MAX
	};
	// プレイヤー状態
	enum E_State
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常
		STATE_FLOW,		// 流されている
		STATE_DEATH,		// 死
		STATE_INVINCIBLE,	// デバッグ向け無敵
		STATE_MAX
	};

	CPlayer(int nPriority = 5);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage) override;	// ヒット処理
	virtual bool Peck(void);		// 突っつく処理

	// 取得・設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// 移動量
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void EnableInput(bool bEnable) { m_bEnableInput = bEnable; }	// 入力可能フラグ
	bool IsEnableInput(void) { return m_bEnableInput; }
	void SetGridV(int nValue) { m_nGridV = nValue; }				// グリッドの縦番号
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }				// グリッドの横番号
	int GetGridH(void) { return m_nGridH; }
	void SetState(E_State state) { m_state = state; }				// 状態
	E_State GetState(void) { return m_state; }
	int GetTimePeck(void) { return m_nTimePeck; }					// 氷を突いた回数
	void BindInputMgr(CInputManager *pInputMgr) { m_pInputMgr = pInputMgr; }	// 入力マネージャー
	CInputManager *GetInpuManager(void) { return m_pInputMgr; }
	void SetID(int nID) { m_nID = nID; }	// 番号
	int GetID(void) { return m_nID; }
	void SetShadowPos(D3DXVECTOR3 pos);

	// 静的メンバ関数
	static CPlayer* Create(void);										// 生成処理
	static vector<CPlayer*> GetInstance(void) { return s_apPlayer; }	// インスタンスの取得
	static void EnableInputAll(bool bValue);							// 全プレイヤーの入力フラグ
	static int GetNumPlayer(void) { return (int)s_apPlayer.size(); }	// プレイヤー数の取得

protected:
	virtual void StartJump(void);	// ジャンプの開始

private:
	// 構造体定義
	struct S_FragMotion
	{// モーションフラグの構造体
		bool bWalk;		// 歩行
		bool bPeck;		// 突っつき
		bool bJump;		// ジャンプ
	};

	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	void CreateDirUI(void);	// 方向UIの生成

	void FollowIce(void);	// 氷に追従
	void Input(void);		// 入力
	
	void MoveAnalog(void);		// アナログ移動
	void InputMoveAnalog(void);	// アナログ移動入力
	void DisableTurn(void);		// 振り返りの無効化
	void Forward(void);			// 前進処理
	void DecreaseMove(void);	// 移動量の減衰
	void FactingRot(void);		// 向きの補正
	void JudgeTurn(void);		// 振り向きの検出
	
	void CollideIce(void);	// 氷との判定
	bool CheckAnyIceForward(void);	// 前方に氷があるかの判定

	void InputPeck(void);			// 突っつきの入力
	void RotationDirUI(int nDir);	// 方向UIの回転
	
	CIce *SelectIceByRot(float fRot);	// 氷を向きで取得
	bool CheckGridChange(void);			// グリッドが変わったかどうかの判定
	
	void StartFlows(void);	// 漂流開始
	bool FindFlowIce(void);	// 漂流する氷の検出
	void StayFlow(void);	// 漂流中の処理
	void FlowDeath(void);	// 漂流中の死
	void EndFlows(void);	// 漂流終了

	void InputJump(void);			// ジャンプの入力
	void SarchJumpIce(void);		// ジャンプ先の氷を探す
	void StayJump(void);			// ジャンプ中の処理
	void LimitInSideFlowIce(void);	// 流氷の内側に制限
	void EndJump(void);				// ジャンプの終了
	
	void FollowDirUI(void);	// 方向UIの追従

	void Event(EVENT_INFO* pEventInfo) override;	// モーションイベント
	void ManageMotion(void);						// モーションの管理

	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号

	bool m_bEnableInput;	// 入力可能フラグ
	bool m_bTurn;			// 振り返っているフラグ
	float m_fRotTurn;		// 振り返る角度

	D3DXVECTOR3 m_move;			// 移動量
	float m_fTimerStartMove;	// 移動の立ち上がりのタイマー
	E_State m_state;			// プレイヤー状態

	vector<CIce*> m_apIceJump;	// ジャンプ先の候補の氷
	CIce *m_pIceDestJump;		// ジャンプ目標の氷
	bool m_bEnableJump;			// ジャンプ可能フラグ
	D3DXVECTOR3 m_posInitJump;	// ジャンプ初期位置

	CIce *m_pIceMoveDest;			// 移動目標の氷
	CIce *m_pLandFlow;				// 漂流時に乗ってる氷のポインタ
	CFlowIce *m_pLandSystemFlow;	// 乗ってる流氷システム
	int m_nTimePeck;				// 氷を突いた回数
	CPolygon3D *m_pDir;				// 方向を示すポリゴン
	CPeckLine *m_pPeckLine;			// つっつき線
	CShadow *m_pShadow;				// 影のポインタ

	S_FragMotion m_fragMotion;	// モーションフラグ

	int m_nID;					// 番号
	CInputManager *m_pInputMgr;	// 入力マネージャー

	// 静的メンバ変数
	static vector<CPlayer*> s_apPlayer;	// 格納用の配列
};

#endif