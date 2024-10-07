//*****************************************
//
// �ėp�����Z�b�g
// Author:���R����
//
//*****************************************
#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

//*****************************************
// �}�N���̒�`
//*****************************************
// �������J���}�N��
#define SAFE_UNINIT(p)		if ((p) != nullptr) { (p)->Uninit();		(p) = nullptr; }	// Uninit�֐��̔j���}�N��
#define SAFE_FREE(p)		if ((p) != nullptr) { free((p));			(p) = nullptr; }	// free�֐��̔j���}�N��
#define SAFE_RELEASE(p)		if ((p) != nullptr) { (p)->Release();		(p) = nullptr; }	// Release�֐��̔j���}�N��
#define SAFE_DEL_OBJECT(p)	if ((p) != nullptr) { DeleteObject((p));	(p) = nullptr; }	// DeleteObject�֐��̔j���}�N��
#define SAFE_REF_RELEASE(p)	if ((p) != nullptr) { (p)->Release((p)); }						// �Q�ƃ|�C���^�t��Release�֐��̔j���}�N��
#define SAFE_DELETE(p)		if ((p) != nullptr) { delete	(p);		(p) = nullptr; }	// �z����g�p���Ȃ�delete���g�p����j���}�N��
#define SAFE_DEL_ARRAY(p)	if ((p) != nullptr) { delete[]	(p);		(p) = nullptr; }	// �z����g�p����delete[]���g�p����j���}�N��

//*****************************************
// �N���X�̒�`
//*****************************************
namespace universal
{
//======================================
// �����n
//======================================
void LimitValuefloat(float *pValue, float fMax, float fMin);
void LimitValueInt(int *pValue, int nMax, int nMin);
void LimitRot(float *fRot);	//��]���䏈��
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax);
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos);
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);

//======================================
// �v�Z�n
//======================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove = nullptr);
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate = 1.0f);
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
void FactingRot(float *pfRot, float fRotDest, float rotateFact);
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact);
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact);
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset);
D3DXVECTOR3 PolarCoordinates(D3DXVECTOR3 rot);
float Vec3Dot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
bool CalcRaySphere(D3DXVECTOR3 posStartRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posSphere, float fRadius, D3DXVECTOR3 *pPosStartHit = nullptr, D3DXVECTOR3 *pPosEndHit = nullptr);
bool CalcRayFlat(D3DXVECTOR3 posPlane, D3DXVECTOR3 nor, D3DXVECTOR3 srcRay, D3DXVECTOR3 endRay, D3DXVECTOR3 *pPosHit = nullptr);
D3DXVECTOR3 CalcScreenToWorld(D3DXVECTOR3 posScreen);
void ConvertScreenPosTo3D(D3DXVECTOR3 *pPosNear, D3DXVECTOR3 *pPosFar, D3DXVECTOR3 *pVecDiff);
D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
void ParentMtx(D3DXMATRIX *pMtx, D3DXMATRIX mtxParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

//======================================
// �ϊ��n
//======================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a);
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec);
D3DXVECTOR3 GetMtxPos(D3DXMATRIX mtx);
int RandRange(int nMax, int nMin);
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength);
float lengthAxis(D3DXVECTOR3 separationAxis, D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3);
D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset);
D3DXVECTOR3 NorRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin);

//======================================
// ����n
//======================================
float PlusMin(float fData1, float fData2);
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget);
bool IsOnTrianglePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, float& rHeight);
bool IsOnSquarePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor1, D3DXVECTOR3 vtxNor2, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsOnSquare(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
bool LineCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3* pos, D3DXVECTOR3 posOld);
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos);
bool IsInScreen(const D3DXVECTOR3 pos, D3DXMATRIX mtx, D3DXVECTOR3 *pPos = nullptr);
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
D3DXVECTOR3 CollideOBBToPlane(D3DXVECTOR3* posOBB, D3DXVECTOR3 vecAxial, D3DXVECTOR3 posPlane, D3DXVECTOR3 vecNorPlane);

//======================================
// �⊮�n
//======================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime);
inline float EaseOutCubic(float fTime) { return 1 - powf(1.0f - fTime, 3.0f); };
inline float EaseInCubic(float fTime) { return powf(fTime, 3.0f); };
float EaseOutBack(float fTime);
inline float EaseOutExpo(float fTime) { return fTime == 1 ? 1 : 1 - powf(2, -10 * fTime); }

template<class T> bool LimitNum		// �l�͈͓̔�����
( // ����
	T& rNum,		// �������l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
);
template<class T> bool LimitMinNum	// �l�̍ŏ��l����
( // ����
	T& rNum,		// �������l
	const T min		// �ŏ��͈�
);
template<class T> bool LimitMaxNum	// �l�̍ő�l����
( // ����
	T& rNum,		// �������l
	const T max		// �ő�͈�
);
template<class T> float ValueToRate	// �l�̊����ϊ�
( // ����
	const T num,	// ���������鐔�l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
);
}

// �C�[�W���O�֐����
namespace easing
{
	// �ʏ�֐�
	inline float Liner(const float x)		{ return x; }

	inline float InSine(const float x)		{ return 1.0f - cosf((x * D3DX_PI) * 0.5f); }
	inline float OutSine(const float x)		{ return sinf((x * D3DX_PI) * 0.5f); }
	inline float InOutSine(const float x)	{ return -(cosf(x * D3DX_PI) - 1.0f) * 0.5f; }

	inline float InQuad(const float x)		{ return x * x; }
	inline float OutQuad(const float x)		{ return 1.0f - (1.0f - x) * (1.0f - x); }
	inline float InOutQuad(const float x)	{ return (x < 0.5f) ? (2.0f * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 2.0f) * 0.5f); }

	inline float InCubic(const float x)		{ return x * x * x; }
	inline float OutCubic(const float x)	{ return 1.0f - powf(1.0f - x, 3.0f); }
	inline float InOutCubic(const float x)	{ return (x < 0.5f) ? (4.0f * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 3.0f) * 0.5f); }

	inline float InQuart(const float x)		{ return x * x * x * x; }
	inline float OutQuart(const float x)	{ return 1.0f - powf(1.0f - x, 4.0f); }
	inline float InOutQuart(const float x)	{ return (x < 0.5f) ? (8.0f * x * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 4.0f) * 0.5f); }

	inline float InQuint(const float x)		{ return x * x * x * x * x; }
	inline float OutQuint(const float x)	{ return 1.0f - powf(1.0f - x, 5.0f); }
	inline float InOutQuint(const float x)	{ return (x < 0.5f) ? (16.0f * x * x * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 5.0f) * 0.5f); }

	// �e���v���[�g�֐�
	template<class T> inline float Liner(T num, const T min, const T max)		{ return Liner(universal::ValueToRate(num, min, max)); }

	template<class T> inline float InSine(T num, const T min, const T max)		{ return InSine(universal::ValueToRate(num, min, max)); }
	template<class T> inline float OutSine(T num, const T min, const T max)		{ return OutSine(universal::ValueToRate(num, min, max)); }
	template<class T> inline float InOutSine(T num, const T min, const T max)	{ return InOutSine(universal::ValueToRate(num, min, max)); }

	template<class T> inline float InQuad(T num, const T min, const T max)		{ return InQuad(universal::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuad(T num, const T min, const T max)		{ return OutQuad(universal::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuad(T num, const T min, const T max)	{ return InOutQuad(universal::ValueToRate(num, min, max)); }

	template<class T> inline float InCubic(T num, const T min, const T max)		{ return InCubic(universal::ValueToRate(num, min, max)); }
	template<class T> inline float OutCubic(T num, const T min, const T max)	{ return OutCubic(universal::ValueToRate(num, min, max)); }
	template<class T> inline float InOutCubic(T num, const T min, const T max)	{ return InOutCubic(universal::ValueToRate(num, min, max)); }

	template<class T> inline float InQuart(T num, const T min, const T max)		{ return InQuart(universal::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuart(T num, const T min, const T max)	{ return OutQuart(universal::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuart(T num, const T min, const T max)	{ return InOutQuart(universal::ValueToRate(num, min, max)); }

	template<class T> inline float InQuint(T num, const T min, const T max)		{ return InQuint(universal::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuint(T num, const T min, const T max)	{ return OutQuint(universal::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuint(T num, const T min, const T max)	{ return InOutQuint(universal::ValueToRate(num, min, max)); }
}

//************************************************************
//	�e���v���[�g�֐�
//************************************************************
//============================================================
//	�l�͈͓̔���������
//============================================================
template<class T> bool universal::LimitNum
(
	T& rNum,		// �������l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
)
{
	if (rNum < min)
	{ // �������l���ŏ��͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = min;

		// �^��Ԃ�
		return true;
	}
	else if (rNum > max)
	{ // �������l���ő�͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = max;

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�l�̍ŏ��l��������
//============================================================
template<class T> bool universal::LimitMinNum
(
	T& rNum,	// �������l
	const T min	// �ŏ��͈�
)
{
	if (rNum < min)
	{ // �������l���ŏ��͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = min;

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�l�̍ő�l��������
//============================================================
template<class T> bool universal::LimitMaxNum
(
	T& rNum,	// �������l
	const T max	// �ő�͈�
)
{
	if (rNum > max)
	{ // �������l���ő�͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = max;

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�l�̊����ϊ�
//============================================================
template<class T> float universal::ValueToRate
(
	const T num,	// ���������鐔�l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
)
{
	// ���鐔�����߂�
	float fDiv = static_cast<float>(max) - static_cast<float>(min);
	if (fDiv == 0.0f) { return 0.0f; }	// 0���Z�΍�

	// �����ϊ������l��Ԃ�
	return (static_cast<float>(num) - static_cast<float>(min)) / fDiv;
}

#endif