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

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	// モーション列挙
	enum EMotion
	{
		MOTION_NEUTRAL = 0,
		MOTION_JUMPSTART,
		MOTION_JUMPFLY,
		MOTION_LANDING,
		MOTION_PECK,
		MOTION_MAX
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return s_pPlayer; }

	// 取得処理
	D3DXVECTOR3 GetMove(void) { return m_move; }	
	D3DXVECTOR3 GetPositionDest(void) { return m_posDest; }

	// 設定処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetPositionDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	virtual void SetPosition(D3DXVECTOR3 pos) override { CGameObject::SetPosition(pos); m_posDest = pos; }

private:
	// メンバ関数
	void Input(void);	// 入力
	void MoveAnalog(void);	// アナログ移動
	void InputMoveAnalog(void);	// アナログ移動入力
	void CollideIce(void);	// 氷との判定
	void MoveGrid(void);	// グリッド移動
	void MoveToGrid(void);	// グリッドまでの移動
	void Debug(void);	// デバッグ処理
	void MotionFinishCheck(void);	// モーションが終了したか確認
	void MovePositionDest(void);

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posDest;	// 目標位置

	int m_nGridV;	// 今いるグリッドの縦番号
	int m_nGridH;	// 今いるグリッドの横番号
	int m_motionType;	//モーションの種類
	bool m_bMove;	// 移動中フラグ
	float m_jumpTime;	// ジャンプ時間

	// 静的メンバ変数
	static CPlayer *s_pPlayer;	// 自身のポインタ
};

#endif