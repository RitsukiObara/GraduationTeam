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
		MOTION_JUMPSTART,
		MOTION_JUMPFLY,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_MAX
	};

	// プレイヤー状態
	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_DEATH,		// 死
		STATE_INVINCIBLE,	// デバッグ向け無敵
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPlayer* Create(void);
	static CPlayer* GetInstance(void) { return s_pPlayer; }

	// 取得・設定
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	D3DXVECTOR3 GetPositionDest(void) { return m_posDest; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理

private:
	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	void Input(void);	// 入力
	void MoveAnalog(void);	// アナログ移動
	void InputMoveAnalog(void);	// アナログ移動入力
	void CollideIce(void);	// 氷との判定
	void MoveGrid(void);	// グリッド移動
	bool JudgeSarchOrMove(void);	// 選択状態か移動状態かの判定
	void UpdateInputSelectIce(void);	// 氷選択状態の更新
	void UpdateInputMoveToIce(void);	// 氷に向かって移動している状態の更新
	CIce *SelectIceByRot(float fRot);	// 氷を向きで取得
	void WalkToDestIce(CIce *pIceDest);	// 目標の氷に向かって移動する処理
	bool CheckGridChange(void);	// グリッドが変わったかどうかの判定
	void InputPeck(void);	// 突っつきの入力
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号
	bool m_bAnalog;	// アナログ操作
	D3DXVECTOR3 m_posDest;	// 目標位置
	D3DXVECTOR3 m_move;	// 移動量
	STATE m_state;		// プレイヤー状態
	CIce *m_pIceMoveDest;	// 移動目標の氷

	// 静的メンバ変数
	static CPlayer* s_pPlayer;	// 自身のポインタ
};

#endif