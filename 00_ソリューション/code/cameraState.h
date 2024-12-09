//*****************************************************
//
// カメラステイト[cameraState.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERASTATE_H_
#define _CAMERASTATE_H_

//****************************************************
// 前方宣言
//****************************************************
class CCamera;
class CSelectStagePenguin;

//****************************************************
// クラスの定義
//****************************************************
// 基底クラス
class CCameraState
{
public:
	CCameraState() {};
	virtual void Init(CCamera *pCamera) = 0;
	virtual void Update(CCamera *pCamera) = 0;

private:
};

// プレイヤー追従
class CFollowPlayer : public CCameraState
{
public:
	CFollowPlayer();
	void Init(CCamera *pCamera) {};
	void Update(CCamera *pCamera) override;

private:
	float m_fTimerPosR;
	float m_fLengthPosR;
	D3DXVECTOR3 m_rotROld;
	bool m_bDebug;
};

// マルチモード
class CMultiGame : public CCameraState
{
public:
	CMultiGame();
	void Init(CCamera* pCamera) {};
	void Update(CCamera* pCamera) override;

private:

};

// 操作
class CMoveControl : public CCameraState
{
public:
	CMoveControl() {};
	void Init(CCamera *pCamera) {};
	void Update(CCamera *pCamera) override;

private:
	bool m_bAbove = false;	// 上空視点かどうか
};

// タイトル
class CCameraStateTitle : public CCameraState
{
public:
	CCameraStateTitle() {};
	void Init(CCamera *pCamera) {};
	void Update(CCamera* pCamera) override;

private:

};

// ステージセレクト時
class CCameraStateSelectStage : public CCameraState
{
public:
	CCameraStateSelectStage(CSelectStagePenguin *pPenguin) { m_pPenguin = pPenguin; }
	void Init(CCamera *pCamera);
	void Update(CCamera* pCamera) override;

private:
	CSelectStagePenguin *m_pPenguin;
};

// シングルリザルト時
class CResultSingle;
class CCameraResultSingle : public CCameraState
{
public:
	CCameraResultSingle(CResultSingle *pResult);
	void Init(CCamera *pCamera);
	void Update(CCamera* pCamera) override;

private:
	// 定数定義
	static constexpr float TIME_MOVE = 5.0f;	// 移動にかかる時間

	// メンバ関数
	void DecidePosDest(CCamera* pCamera);	// 目標位置の決定
	void MoveToPlayerFront(CCamera* pCamera);	// プレイヤーの前まで移動する処理

	// メンバ変数
	D3DXVECTOR3 m_posDest;	// 目標位置
	D3DXVECTOR3 m_posInitiial;	// 初期位置位置
	D3DXVECTOR3 m_vecDiffInitial;	// 初期差分ベクトル
	float m_fTimerMove;	// 移動タイマー
	CResultSingle *m_pResult;	// リザルトのポインタ
};

// マルチモードのリザルト
class CCameraStateResultMulti : public CCameraState
{
public:
	CCameraStateResultMulti() {}
	void Init(CCamera* pCamera);
	void Update(CCamera* pCamera) override;
private:

};

// モード選択画面
class CCameraStateSelectMode : public CCameraState
{
public:
	CCameraStateSelectMode(){}
	void Init(CCamera* pCamera);
	void Update(CCamera* pCamera) override;
private:

};

#endif