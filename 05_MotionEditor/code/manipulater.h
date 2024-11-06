//*****************************************************
//
// マニピュレーター[manipulater.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MANIPULATER_H_
#define _MANIPULATER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方定義
//*****************************************************
class CStateManipulater;
class CMotion;

//*****************************************************
// クラスの定義
//*****************************************************
class CManipulater : public CObject
{// マニピュレータークラス
public:
    CManipulater(int nPriority = 4);	// コンストラクタ
	~CManipulater();    // デストラクタ

    // メンバ関数
	HRESULT Init(void); // 初期化処理
	void Uninit(void);  // 終了処理
	void Update(void);  // 更新処理
	void Draw(void);    // 描画処理
    void ChangeState(CStateManipulater *pState);  // ステイトの変更

    // 変数取得・設定関数
    int GetIdxPart(void) { return m_nIdxPart; } // パーツ番号
    void SetIdxPart(int nIdx) { m_nIdxPart = nIdx; }
    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
    D3DXVECTOR3 GetPosition(void) { return m_pos; }
    void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }	// 向き
    D3DXVECTOR3 GetRotation(void) { return m_rot; }
    CMotion *GetMotion(void) { return m_pMotion; }  // モーション取得

    // 静的メンバ関数
    static CManipulater* Create(int nIdxPart, CMotion *pMotion);    // 生成処理

private:
	// メンバ関数
    D3DXVECTOR3 m_pos;  // 位置
    D3DXVECTOR3 m_rot;  // 向き

	// メンバ変数
    int m_nIdxPart; // パーツの番号
    CStateManipulater *m_pState;    // ステイトのポインタ
    CMotion *m_pMotion; // モーションのポインタ
};

class CStateManipulater
{// ステイトの基底クラス
public:
    CStateManipulater() {};	// コンストラクタ
    virtual ~CStateManipulater() {};    // デストラクタ
    
    // メンバ関数
    virtual void Init(CManipulater *pManipulater) = 0; // 初期化処理
    virtual void Uninit(CManipulater *pManipulater) = 0;  // 終了処理
    virtual void Update(CManipulater *pManipulater) = 0;  // 更新処理

private:
};

class CStateManipulaterTranslate : public CStateManipulater
{// 移動のステイト
public:
    CStateManipulaterTranslate();    // コンストラクタ
    virtual ~CStateManipulaterTranslate();   // デストラクタ

    // メンバ関数
    void Init(CManipulater *pManipulater) override; // 初期化処理
    void Uninit(CManipulater *pManipulater) override;  // 終了処理
    void Update(CManipulater *pManipulater) override;  // 更新処理

private:
    enum E_MODE
    {// 軸モード
        MODE_NONE = 0,  // 何でもない状態
        MODE_X,  // X軸を動かしている
        MODE_Y,  // Y軸を動かしている
        MODE_Z,  // Z軸を動かしている
        MODE_MAX
    };
    // メンバ関数
    void Input(CManipulater *pManipulater);   // 入力処理
    void FollowModel(CManipulater *pManipulater); // モデルの追従
    void ModeMove(CManipulater *pManipulater);    // モードごとの動き

    // メンバ変数
    CObjectX *m_pManipulater;   // マニピュレーター
    E_MODE m_mode;  // 軸のモード
};

#endif
