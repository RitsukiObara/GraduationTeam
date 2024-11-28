//*****************************************************
//
// パーティクル処理[particle.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラス定義
//*****************************************************
class CParticle : public CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,			// 何でもない
		TYPE_RUN,				// 疾走
		TYPE_RESULTSMOKE,		// リザルトの煙
		TYPE_ICEBREAK,			// 氷破壊時
		TYPE_HITENEMY,			// 敵と当たったときのパーティクル
		TYPE_PECKHARDICE,		// 硬い氷を叩いた時のパーティクル
		TYPE_REPAIRICE,			// 氷の修復するときのパーティクル
		TYPE_DROP,				// 水しぶき
		TYPE_BUBBLE_SINK,		// 沈むときの泡
		TYPE_ENTER_MAP,			// マップに入ったときのエフェクト
		TYPE_STAR_HIT,			// ヒット時の星パーティクル
		TYPE_STICK_ICE,			// 氷がくっつく時のパーティクル
		TYPE_SNOW_STORM,		// 雪が舞ったときのパーティクル
		TYPE_STAGESELECT_SNOW,	// ステージセレクトの雪の軌道
		TYPE_LAND,				// 敵が氷に着地するときのパーティクル
		TYPE_ENTERPLAYER,		// プレイヤーエンター時のパーティクル
		TYPE_MAX
	}TYPE;

	CParticle(int nPriority = 3);	// コンストラクタ
	~CParticle();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};
	static CParticle *Create(D3DXVECTOR3 pos, TYPE type, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f },D3DXVECTOR3 *pPosOwner = nullptr, int nPriorityEffect = 5);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// 取得処理
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; };	// 向き設定
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// 向き取得
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	static void Load(void);	// 読込処理
	static void Unload(void);	// 破棄処理

private:
	typedef struct
	{// パーティクル情報
		char acTexPath[256];
		int nLife;	// 寿命
		int nLifeEffect;	// エフェクトの寿命
		float fRadiusEffect;	// エフェクトの半径
		int nNumEffect;	// エフェクトの数
		float fSpeed;	// エフェクトの基準スピード
		D3DXCOLOR col;	// 色
		int nAdd;	// 加算処理をするかどうか
		float fGravity;	// 重力
		float fDecrease;	// 半径減少量
		float fRangeRot;	// 向きのランダム範囲
		int nRot;	// 向きを反映するかどうか
		bool bTurn;	// 反転するかどうか
		int modeRender;	// 描画モード
	}PARTICLE_INFO;

	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 *m_pPosOwner;	// 持ち主の位置
	D3DXVECTOR3 m_rot;	// 向き
	int m_nLife;	// 寿命
	static PARTICLE_INFO *m_apParticleInfo[TYPE_MAX + 1];
	TYPE m_type; // 種類
	int m_nPriorityEffect;
};

#endif