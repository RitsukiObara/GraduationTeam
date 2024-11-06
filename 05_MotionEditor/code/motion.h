//*****************************************************
//
// モーションの処理[motion.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************
// 前方宣言
//*****************************************************
class CObject3D;
class CManipulater;

//*****************************************************
// インクルード
//*****************************************************
#include "parts.h"
#include "object.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace motion
{
const int MAX_PARTS = 24;	// パーツの最大数
const int MAX_KEY = 24;	// キーの最大数
const int MAX_MOTION = 24;	// モーションの最大数
}

//*****************************************************
// クラスの定義
//*****************************************************
class CMotion : public CObject
{
public:
	//キーの構造体
	typedef struct
	{
		float fPosX;				//位置X
		float fPosY;				//位置Y
		float fPosZ;				//位置Z
		float fRotX;				//向きX
		float fRotY;				//向きY
		float fRotZ;				//向きZ
	}KEY;

	//キー情報構造体
	typedef struct
	{
		int nFrame;					//再生フレーム
		KEY aKey[motion::MAX_PARTS];			//各モデルのキー要素
	}KEY_INFO;

	typedef struct
	{// パーティクル情報構造体
		int nKey;	//生成するキー
		int nFrame;	//生成するフレーム
        float fNumFrame;    // 持続フレーム
        float fTimer;    // 持続タイマー
        float fRadius;    // 半径
		D3DXVECTOR3 offset;	// オフセット位置
		int nIdxParent;	// 親となるパーツの番号
	}EVENT_INFO;
	
	typedef struct
	{// 当たり判定情報構造体
		int nKey;	//生成するキー
		int nFrame;	//生成するフレーム
		D3DXVECTOR3 offset;	// オフセット位置
		int nIdxParent;	// 親となるパーツの番号
	}COLLISION_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;						//ループするかどうか
		int nNumKey;					//キーの総数
		KEY_INFO aKeyInfo[motion::MAX_PARTS];	//キー情報
		int nNumEvent;	// パーティクルの数
		int nNumCollision;	// 当たり判定の数
		EVENT_INFO *pEvent;	// パーティクルのポインタ
	}MOTION_INFO;

	// パーツの構造体
	typedef struct
	{
		int nIdx;
		int nIdxParent;
		CParts *pParts;
	}Parts;

	// メンバ関数
	CMotion(int nPriority = 5);	// コンストラクタ
	~CMotion();	// デストラクタ
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(char *pPath);
	void MultiplyMtx(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 設定処理
	void SetPosShadow(D3DXVECTOR3 pos) { m_posShadow = pos; }	// 設定処理
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	D3DXVECTOR3 *GetPosAddress(void) { return &m_pos; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMtxPos(int nIdx);
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMotion(int nMotionType);
	int GetMotion(void) { return m_motionType; }
	void SetKeyOld(void);
	static CMotion *Create(char *pPath);
	Parts *GetParts(int nIdx) { return m_apParts[nIdx]; }
	D3DXMATRIX *GetMatrix(void) { return &m_mtxWorld; }
	bool IsFinish(void) { return m_bFinish; }
	void SetMatrix(void);
	float GetRadiusMax(void);
	void SetAfterImage(D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f }, int m_nLife = 10);
	int GetKey(void) { return m_nKey; }
	int GetFrame(void) { return m_nCounterMotion; }
	void SetAllCol(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void ResetAllCol(void);
	void InitPose(int nMotion);
	void EnableShadow(bool bShadow) { m_bShadow = bShadow; }
	void EnableIndependent(bool bInde) { m_bInde = bInde; }
	EVENT_INFO *GetInfoEvent(int nMotion) { return m_aMotionInfo[nMotion].pEvent; }
	int GetNumEventInfo(int nMotion) { return m_aMotionInfo[nMotion].nNumEvent; }
	virtual void Event(EVENT_INFO *pEventInfo) {};
    CManipulater *CreateManipulater(int nIdxPart);
    void DeleteManipulater(void);

private:
	void Motion(void);
	void Input(void);
	void DrawMotionState(void);
	void SetPose(void);
	void SetTransform(int nIdx = -1);
	void SaveMotion(void);
	void Reset(void);
    void CreateIcon(int nIdx);
    void SelectParts(void);

	Parts *m_apParts[motion::MAX_PARTS];	// パーツの構造体
	MOTION_INFO m_aMotionInfo[motion::MAX_MOTION];	//モーション情報の構造体
	KEY m_aKeyOld[motion::MAX_PARTS];	// 前回のキー情報の構造体
	KEY_INFO m_keyInfoTemp;	// コピー用のキー情報
	int m_nNumMotion;	//モーションの総数
	int m_motionType;	//モーションの種類
	int m_motionTypeOld;	//前回のモーションの種類
	int m_nFrame;	// フレーム数
	bool m_bLoopMotion;	//ループするかどうか
	int m_nNumKey;	//キーの総数
	int m_nKey;	//現在のキー
	int m_nCounterMotion;	//モーションカウンター
	int m_nNumParts;	// パーツの数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posShadow;	// 影の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	//向き
	D3DXMATRIX m_mtxWorld;	// マトリックス
	D3DXCOLOR m_col;	// 色
	bool m_bFinish;	// モーションが終わったかどうか
	bool m_bShadow;	// 影を描画するかどうか
	bool m_bInde;	// 分離しているかどうか
	int m_nIdxParts;
	bool m_bMotion;
	bool m_bSetUp;
    char m_aPathSave[256];    // 保存パス
    std::map<int, CObject3D*> m_mapIcon;    // アイコンのマップコンテナ
    CManipulater *m_pManipulater;   // マニピュレーター
};

#endif
