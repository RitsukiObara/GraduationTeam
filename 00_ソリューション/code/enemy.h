//*****************************************************
//
// 敵の処理[enemy.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
class CEnemy : public CMotion
{
public:
	// 列挙型定義
	enum TYPE
	{// 種類
		TYPE_SEALS = 0,
		TYPE_MAX
	};
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_APPER,	// 出現状態
		STATE_STOP,		// 停止状態
		STATE_MOVE,		// 移動状態
		STATE_ATTACK,	// 攻撃状態
		STATE_DRIFT,	// 流れる状態
		STATE_MAX
	};

	// 関数ポインタ型の定義
	typedef void (CEnemy::*UpdateState)(void);

	CEnemy(int nPriority = 4);	// コンストラクタ
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void TranslateByGrid(int nIdxV,int nIdxH);	// グリッドによる移動
	void FollowIce(void);	// 氷に追従
	virtual void Death(void);	// 死亡時の処理
	void DisableTurn(void);	// 振り返りの無効化

	virtual void UpdateApper(void) = 0;		// 出現状態の更新

	virtual void UpdateStop(void);		// 停止状態の更新

	virtual void UpdateMove(void);		// 移動状態の更新

	virtual void UpdateAttack(void) = 0;	// 攻撃状態の更新

	virtual void UpdateDrift(void);		// 漂流状態の更新

	// 変数取得・設定関数
	void SetGridV(int nValue) { m_nGridV = nValue; }	// グリッドの縦番号
	int GetGridV(void) { return m_nGridV; }
	void SetGridH(int nValue) { m_nGridH = nValue; }	// グリッドの横番号
	int GetGridH(void) { return m_nGridH; }

	void SetGridVNext(int nValue) { m_nGridVNext = nValue; }	// 次のグリッドの縦番号
	int GetGridVNext(void) { return m_nGridVNext; }
	void SetGridHNext(int nValue) { m_nGridHNext = nValue; }	// 次のグリッドの横番号
	int GetGridHNext(void) { return m_nGridHNext; }

	void SetGridVDest(int nValue) { m_nGridVDest = nValue; }	// 目標のグリッドの縦番号
	int GetGridVDest(void) { return m_nGridVDest; }
	void SetGridHDest(int nValue) { m_nGridHDest = nValue; }	// 目標のグリッドの横番号
	int GetGridHDest(void) { return m_nGridHDest; }

	void SetSpeedMove(float fValue) { m_fSpeedMove = fValue; }	// 移動速度
	float GetSpeedMove(void) { return m_fSpeedMove; }

	void EnableFollowIce(bool bValue) { m_bFollowIce = bValue; }	// 氷追従フラグ
	bool IsFollowIce(void) { return m_bFollowIce; }

	void EnableTurn(bool bValue) { m_bTurn = bValue; }	// 振り向きモーション
	bool IsTurn(void) { return m_bTurn; }

	void EnableMove(bool bValue) { m_bEnableMove = bValue; }	// 移動可能フラグ
	bool IsEnableMove(void) { return m_bEnableMove; }

	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 移動量
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// 静的メンバ関数
	static CEnemy* Create(int nType,int nGridV = 0, int nGridH = 0);	// 生成処理
	static std::vector<CEnemy*> GetInstance(void) { return s_vector; }	// 配列の取得

private:
	// メンバ関数
	void InitGridIdx(void);	// グリッド番号の初期化
	virtual void SetApperTransform(void) = 0;	// 出現時のトランスフォーム設定
	void SarchNearIceToDest(void);	// 目標に近い氷を探す
	bool PathFind(int nIdxV, int nIdxH, vector<CIce*>& apIce);	// 探索の再帰関数
	void MoveToNextGrid(void);	// 次のグリッドに向かって移動する
	void JudgeTurn(void);	// 反転するかの判定
	void JudgeCanMove(void);	// 移動できるかの判断
	void CheckChangeGrid(void);	// グリッドが変わったかの確認
	virtual void AliveDestGrid(void) {};	// 目的地に着いた時の仮想関数
	void DriftDeath(void);	// 漂流中の死亡関数
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号
	int m_nGridVNext;	// 次行くグリッドの縦番号
	int m_nGridHNext;	// 次行くグリッドの横番号
	int m_nGridVDest;	// 目標のグリッドの縦番号
	int m_nGridHDest;	// 目標のグリッドの横番号

	bool m_bEnableMove;	// 移動可能フラグ
	bool m_bTurn;	// 振り向きのフラグ
	float m_fRotTurn;	// 振り向き角度
	float m_fTimerDeath;	// 死亡までのタイマー
	float m_fSpeedMove;	// 移動速度
	bool m_bFollowIce;	// 氷追従フラグ
	E_State m_state;	// 状態
	CIce *m_pIceLand;	// 乗っている氷
	D3DXVECTOR3 m_move;	// 移動量
	
	// 静的メンバ変数
	static std::vector<CEnemy*> s_vector;	// 自身のポインタ
};

namespace enemy
{
inline bool IsEnemyOnIce(int nIdxV, int nIdxH)
{
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (auto it : aEnemy)
	{
		int nIdxVEnemy = it->GetGridV();
		int nIdxHEnemy = it->GetGridH();

		if (nIdxVEnemy == nIdxV && nIdxHEnemy == nIdxH)
		{
			return true;
		}
	}

	return false;
}
}

#endif