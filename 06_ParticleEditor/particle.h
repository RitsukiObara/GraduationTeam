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
		TYPE_NONE = 0,	// 何でもない
		TYPE_EXPLOSION,	// 爆発
		TYPE_SAND,	// 砂ぼこり
		TYPE_MAX
	}TYPE;

	CParticle(int nPriority = 3);	// コンストラクタ
	~CParticle();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};
	static CParticle *Create(D3DXVECTOR3 pos, TYPE type);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// 取得処理
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	static void Input(void);	// 入力処理
	static void Load(void);

private:
	static void Save(void);

	D3DXVECTOR3 m_pos;	// 位置
	int m_nLifeParticle;	// 寿命

	static D3DXVECTOR3 m_rot;	// 向き
	static int m_nLife;	// 寿命
	static int m_nLifeEffect;	// エフェクトの寿命
	static float m_fRadiusEffect;	// エフェクトの半径
	static int m_nNumEffect;	// エフェクトの数
	static float m_fRangeSpeed;	// スピードの幅
	static float m_fRangeRadius;	// サイズの幅
	static float m_fSpeed;	// エフェクトの基準スピード
	static D3DXCOLOR m_col;	// 色
	static int m_bAdd;	// 加算処理をするかどうか
	static int m_nCurrentCol;	// 色の選択要素
	static bool m_bLoop;	// ループするかどうか
	static int m_nRate;	// 加算倍率
	static float m_fGravity;	// 重力
	static float m_fDecrease;	// 半径減少量
	static int m_bRot;	// 向きを反映するかどうか
	static float m_fRangeRot;	// 向きのランダム範囲
	static int m_nNumAll;	// 総数
	static bool m_bTurn;	// 反転するかどうか
	static int m_modeRender;	// 描画モード
};

#endif