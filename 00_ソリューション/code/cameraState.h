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
	CCameraStateSelectStage() {};
	void Init(CCamera *pCamera);
	void Update(CCamera* pCamera) override;

private:

};

#endif