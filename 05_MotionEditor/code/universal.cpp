//*****************************************
//
// ���낢��ėp�����l�ߍ���
// Author:���R����
//
//*****************************************

//*****************************************
// �C���N���[�h
//*****************************************
#include "manager.h"
#include "renderer.h"
#include "universal.h"
#include "debugproc.h"

namespace universal
{
//=========================================
// �l�̐���
//=========================================
void LimitValue(float *pValue, float fMax, float fMin)
{
	if (fMax < *pValue)
	{
		*pValue = fMax;
	}

	if (fMin > *pValue)
	{
		*pValue = fMin;
	}
}

//=========================================
// �����l�̐���
//=========================================
void LimitValueInt(int *pValue, int nMax, int nMin)
{
    if (pValue == nullptr)
        return;

    if (nMax <= *pValue)
    {
        *pValue = nMax;
    }

    if (nMin >= *pValue)
    {
        *pValue = nMin;
    }
}

//=========================================
// ��]���䏈��
//=========================================
void LimitRot(float *fRot)
{
	if (*fRot > D3DX_PI)
	{
		*fRot -= 6.28f;
	}
	if (*fRot < -D3DX_PI)
	{
		*fRot += 6.28f;
	}
}

//=========================================
// ���x�̐���
//=========================================
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax)
{
	if (pVec == nullptr)
	{
		return;
	}

	// ���x�̎擾
	float fSpeed = D3DXVec3Length(pVec);

	if (fSpeed > fSpeedMax)
	{// ���x�̐���
		D3DXVECTOR3 vecNew = *pVec;

		D3DXVec3Normalize(&vecNew, &vecNew);

		vecNew *= fSpeedMax;

		*pVec = vecNew;
	}
}

//========================================
// ��`���̈ʒu����
//========================================
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos)
{
	if (pPos == nullptr)
		return;
	
	LimitValue(&pPos->x, fWidth, -fWidth);
	LimitValue(&pPos->z, fHeight, -fHeight);
}

//========================================
// �~���̋�������
//========================================
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	posTarget.y = pPos->y;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength >= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// ���̋��������i�����j
//========================================
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget)
{
	if (pPos == nullptr)
		return 0.0f;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	float fDistDiff = D3DXVec3Length(&vecDiff);

	if (fLength <= fDistDiff)
	{
		fDistDiff = fLength;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		vecDiff *= fLength;

		*pPos = posTarget - vecDiff;
	}

	return fDistDiff;
}

//========================================
// �z�[�~���O
//========================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove)
{
	if (pMove == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - pos;

	VecConvertLength(&vecDiff, fSpeedChase);

	*pMove += vecDiff;
}

//========================================
// �x�N�g���𒷂��ŕ␳���鏈��
//========================================
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength)
{
	D3DXVec3Normalize(pVec, pVec);

	*pVec *= fLength;
}

//========================================
// ���΂̑��΍��W���o������
//========================================
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate)
{
	D3DXVECTOR3 posInvers = { 0.0f,0.0f,0.0f };

	D3DXVECTOR3 vecDiff = posCenter - pos;

	vecDiff *= fRate;

	posInvers = posCenter + vecDiff;

	return posInvers;
}

//========================================
// �ڕW�ʒu�Ɍ���������
//========================================
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact)
{
	if (pPos == nullptr)
		return;

	D3DXVECTOR3 vecDiff = posTarget - *pPos;

	*pPos += vecDiff * fFact;
}

//========================================
// �I�t�Z�b�g�ւ̃x�N�g��
//========================================
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset)
{
	D3DXVECTOR3 pos =
	{
		mtx._41,
		mtx._42,
		mtx._43,
	};

	D3DXMATRIX mtxOffset;

	SetOffSet(&mtxOffset, mtx, posOffset);

	D3DXVECTOR3 offset =
	{
		mtxOffset._41,
		mtxOffset._42,
		mtxOffset._43,
	};

	D3DXVECTOR3 vecDiff = offset - pos;

	return vecDiff;
}

//========================================
// �ɍ��W�̌v�Z
//========================================
D3DXVECTOR3 PolarCoordinates(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 vec;

	vec =
	{
		sinf(rot.x) * sinf(rot.y),
		cosf(rot.x),
		sinf(rot.x) * cosf(rot.y),
	};

	return vec;
}

//========================================
// 3�����x�N�g���̓���
//========================================
float Vec3Dot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	float fDot = 0.0f;

	fDot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;

	return fDot;
}

//========================================
// ���C�Ƌ��̔���
//========================================
bool CalcRaySphere(D3DXVECTOR3 posStartRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posSphere, float fRadius, D3DXVECTOR3 *pPosStartHit, D3DXVECTOR3 *pPosEndHit)
{
	// ���̍��W�����C�̊J�n�n�_���߂�
	posSphere -= posStartRay;

	float A = vecRay.x * vecRay.x + vecRay.y * vecRay.y + vecRay.z * vecRay.z;
	float B = vecRay.x * posSphere.x + vecRay.y * posSphere.y + vecRay.z * posSphere.z;
	float C = posSphere.x * posSphere.x + posSphere.y * posSphere.y + posSphere.z * posSphere.z - fRadius * fRadius;

	if (A == 0.0f)
		return false; // ���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // ���C�̔��΂ŏՓ�

	if (pPosStartHit != nullptr)
	{// �Փ˂��͂��߂��Ƃ���
		*pPosStartHit = posStartRay + a1 * vecRay;
	}

	if (pPosEndHit != nullptr)
	{// �Փ˂��I������Ƃ���
		*pPosStartHit = posStartRay + a2 * vecRay;
	}

	return true;
}

//========================================
// ���C�ƕ��ʂ̔���
//========================================
bool CalcRayFlat(D3DXVECTOR3 posPlane, D3DXVECTOR3 nor, D3DXVECTOR3 srcRay, D3DXVECTOR3 endRay, D3DXVECTOR3 *pPosHit)
{
	D3DXVECTOR3 vecToSrc = srcRay - posPlane;
	D3DXVECTOR3 vecToEnd = endRay - posPlane;

	float fDotSrc = Vec3Dot(vecToSrc, nor);
	float fDotEnd = Vec3Dot(vecToEnd, nor);

	if (pPosHit != nullptr)
	{// �Փ˂������W�̐ݒ�
		D3DXVECTOR3 posHit;

		if (fDotEnd < 0)
			fDotEnd *= -1;
		if (fDotSrc < 0)
			fDotSrc *= -1;

		float fDotMax = fDotSrc + fDotEnd;
		float fRate = fDotSrc / fDotMax;

		D3DXVECTOR3 vecDiff = endRay - srcRay;
		posHit = srcRay + vecDiff * fRate;

		*pPosHit = posHit;
	}

	return fDotSrc * fDotEnd <= 0;
}

//========================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//========================================
D3DXVECTOR3 CalcScreenToWorld(D3DXVECTOR3 posScreen)
{
	HWND hWnd = FindWindow(CLASS_NAME, WINDOW_NAME);

	RECT rect;
	GetClientRect(hWnd, &rect);
	float fWidthScreen = static_cast<float>(rect.right - rect.left);
	float fHeightScreen = static_cast<float>(rect.bottom - rect.top);

	// �e�s��̋t�s����Z�o
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX InvView, InvPrj, VP, InvViewport, View, Prj;

	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Prj);

	D3DXMatrixInverse(&InvView, NULL, &View);
	D3DXMatrixInverse(&InvPrj, NULL, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = fWidthScreen / 2.0f; VP._22 = -fHeightScreen / 2.0f;
	VP._41 = fWidthScreen / 2.0f; VP._42 = fHeightScreen / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 pOut;
	D3DXVec3TransformCoord(&pOut, &D3DXVECTOR3(posScreen.x, posScreen.y, posScreen.z), &tmp);

	return pOut;
}

//========================================
// �X�N���[�����W��3D�ɕϊ�
//========================================
void ConvertScreenPosTo3D(D3DXVECTOR3 *pPosNear, D3DXVECTOR3 *pPosFar, D3DXVECTOR3 *pVecDiff)
{
	HWND hWnd = FindWindow(CLASS_NAME, WINDOW_NAME);

	if (!hWnd)
	{
		assert(("�E�B���h�E������Ȃ���I", false));
	}

	POINT posCursor;

	GetCursorPos(&posCursor);

	CDebugProc::GetInstance()->Print("\n�ϊ��O[%d,%d]", posCursor.x, posCursor.y);

	ScreenToClient(hWnd, &posCursor);

	CDebugProc::GetInstance()->Print("\n�ϊ���[%d,%d]", posCursor.x, posCursor.y);

	*pPosNear = universal::CalcScreenToWorld(D3DXVECTOR3((float)posCursor.x, (float)posCursor.y, 0.0f));
	*pPosFar = universal::CalcScreenToWorld(D3DXVECTOR3((float)posCursor.x, (float)posCursor.y, 1.0f));

	if (pVecDiff != nullptr)
	{
		*pVecDiff = *pPosFar - *pPosNear;
		D3DXVec3Normalize(pVecDiff, pVecDiff);
	}
}

//========================================
// �O�ς̍쐬
//========================================
D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
{
	D3DXVECTOR3 vecCross;

	vecCross =
	{
		vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x
	};

	return vecCross;
}

//========================================
// �I�t�Z�b�g�ݒ菈��
//========================================
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtxWorldOffset);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posOffset.x, posOffset.y, posOffset.z);
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxTrans);

	//�}�g���b�N�X���������킹��
	D3DXMatrixMultiply(pMtxWorldOffset, pMtxWorldOffset, &mtxWorldOwner);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorldOffset);
}

//========================================
// �����␳����
//========================================
void FactingRot(float *pfRot, float fRotDest, float rotateFact)
{
	// �����̊p�x�̕␳
	LimitRot(pfRot);
	LimitRot(&fRotDest);

	//�����p�x���擾
	float fRotDiff = fRotDest - *pfRot;

	//�p�x�̏C��
	LimitRot(&fRotDiff);

	//�p�x�␳
	*pfRot += fRotDiff * rotateFact;

	LimitRot(pfRot);
}

//========================================
// �ڕW��������������
//========================================
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact)
{
	if (pRot == nullptr)
	{
		return;
	}

	// �����p�x�̎擾
	D3DXVECTOR3 vecDiff = posTarget - posOwn;

	D3DXVECTOR3 rotDest = VecToRot(vecDiff);

	FactingRot(&pRot->x, rotDest.x, fFact);
	FactingRot(&pRot->y, rotDest.y, fFact);
}

//========================================
// �����̔�r
//========================================
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	rot.y = atan2f(vec.x, vec.z);

	float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

	rot.x = atan2f(fLength, -vec.y);

	return rot;
}

//========================================
// �}�g���b�N�X����ʒu�̎擾
//========================================
D3DXVECTOR3 GetMtxPos(D3DXMATRIX mtx)
{
	D3DXVECTOR3 pos =
	{
		mtx._41,
		mtx._42,
		mtx._43
	};

	return pos;
}

//========================================
// �����̔�r
//========================================
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = D3DXVec3Length(&vecDiff);

	if (fLength < fLengthMax)
	{
		if (fDiff != nullptr)
		{
			*fDiff = fLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// �����̔�r����
//========================================
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff)
{
	D3DXVECTOR3 vecDiff = posTarget - posOwn;
	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	if (fLength < fLengthMax)
	{
		if (fDiff != nullptr)
		{
			*fDiff = fLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}

//========================================
// �O�ς̌v�Z
//========================================
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return pos1.z * pos2.x - pos1.x * pos2.z;
}

//========================================
// �O�ς̓����蔻��v�Z
//========================================
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move)
{
	bool bHit = false;

	D3DXVECTOR3 vecLine = vecDest - vecSorce;
	D3DXVECTOR3 vecToPos = posTarget - vecSorce;
	float fArea = CrossProduct(vecLine, vecToPos);

	if (fArea > 0)
	{
		if (pRate != nullptr)
		{
			// �������Z�o

			float fAreaMax = CrossProduct(vecLine, move);
			fArea = CrossProduct(vecToPos, move);

			*pRate = fArea / fAreaMax;
		}

		bHit = true;
	}

	return bHit;
}

//========================================
// �O�p�`�̒��ɂ��邩�̔���
//========================================
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget)
{
	int nHit = 0;

	if(IsCross(posTarget, vtx1, vtx2, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx2, vtx3, nullptr) == false)
		nHit++;
	if (IsCross(posTarget, vtx3, vtx1, nullptr) == false)
		nHit++;

	if (nHit == 3)
	{
		return true;
	}

	return false;
}

//========================================
// �O�p�`�̉��ɂ��邩�̔���
//========================================
bool IsOnTrianglePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, float& rHeight)
{
	D3DXVECTOR3 vecP, vecTemp;
	float fHeight, fDot;

	// �|���S���Ɠ��O����
	if (D3DXVec3Cross(&vecTemp, &(posTarget - vtx1), &(vtx2 - vtx1))->y < 0 &&
		D3DXVec3Cross(&vecTemp, &(posTarget - vtx2), &(vtx3 - vtx2))->y < 0 &&
		D3DXVec3Cross(&vecTemp, &(posTarget - vtx3), &(vtx1 - vtx3))->y < 0)
	{
		// y���@����0�ł͂Ȃ�������
		if (vtxNor.y == 0.0f)
			return false;

		// �p����ڕW�ʒu�ւ̃x�N�g��
		vecP = posTarget - vtx1;

		// ���ς��v�Z
		fDot = (vtxNor.x * vecP.x) + (vtxNor.z * vecP.z);

		// ���ς�p���č������v�Z
		fHeight = -(fDot / vtxNor.y) + vtx1.y;

		// �������ڕW�ʒu��荂��������
		if (fHeight >= posTarget.y)
		{
			// �������
			rHeight = fHeight;
			return true;
		}
	}

	return false;
}

//========================================
// �|���S���̉��ɂ��邩�̔���
//========================================
bool IsOnSquarePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor1, D3DXVECTOR3 vtxNor2, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight)
{
	bool bColllision = false;

	// 1�ڂ̃|���S���Ɠ��O����
	if (IsOnTrianglePolygon(vtx1, vtx2, vtx3, vtxNor1, posTarget, rHeight))
		return true;

	// 2�ڂ̃|���S���Ɠ��O����
	if (IsOnTrianglePolygon(vtx4, vtx3, vtx2, vtxNor2, posTarget, rHeight))
		return true;
	
	return bColllision;
}

//========================================
// �l�p�̉��ɂ��邩�̔���
// 1���玞�v���ɑ�����Ă�������
//========================================
bool IsOnSquare(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight)
{
	D3DXVECTOR3 vecP, vecTemp;
	float fHeight, fDot;

	// �|���S���Ɠ��O����
	if (D3DXVec3Cross(&vecTemp, &(posTarget - vtx1), &(vtx2 - vtx1))->y <= 0 &&
		D3DXVec3Cross(&vecTemp, &(posTarget - vtx2), &(vtx3 - vtx2))->y <= 0 &&
		D3DXVec3Cross(&vecTemp, &(posTarget - vtx3), &(vtx4 - vtx3))->y <= 0 &&
		D3DXVec3Cross(&vecTemp, &(posTarget - vtx4), &(vtx1 - vtx4))->y <= 0)
	{
		// y���@����0�ł͂Ȃ�������
		if (vtxNor.y == 0.0f)
			return false;

		// �p����ڕW�ʒu�ւ̃x�N�g��
		vecP = posTarget - vtx1;

		// ���ς��v�Z
		fDot = (vtxNor.x * vecP.x) + (vtxNor.z * vecP.z);

		// ���ς�p���č������v�Z
		fHeight = -(fDot / vtxNor.y) + vtx1.y;

		// �������ڕW�ʒu��荂��������
		if (fHeight >= posTarget.y)
		{
			// �������
			rHeight = fHeight;
			return true;
		}
	}

	return false;
}

//========================================
// ���ɑ΂���O�ς̉����߂�����
//========================================
bool LineCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3* pos, D3DXVECTOR3 posOld)
{
	D3DXVECTOR3 vecMove, vecLine, vecToPos, vecToPosOld;
	float fRate, fOutUnit, fOutRate;

	// �p�x�N�g���v�Z
	vecMove = *pos - posOld;
	vecLine = vtx1 - vtx2;
	vecToPos = *pos - vtx2;
	vecToPosOld = posOld - vtx2;

	// y������0�ɂ���
	vecMove.y = 0.0f;
	vecLine.y = 0.0f;
	vecToPos.y = 0.0f;
	vecToPosOld.y = 0.0f;

	// ��_�{���v�Z
	fOutUnit = CrossProduct(vecLine, vecMove);
	fOutRate = CrossProduct(vecToPos, vecMove);
	fRate = fOutRate / fOutUnit;

	// ��������܂����ł��邩����
	if (CrossProduct(vecLine, vecToPos) > 0.0f && 
		CrossProduct(vecLine, vecToPosOld) <= 0.0f && 
		fRate >= 0.0f && fRate < 1.0f)
	{
		D3DXVECTOR3 vecNor, posCross;
		float fDot;

		//�@���x�N�g���v�Z
		vecNor.x = -vecLine.z;
		vecNor.y = 0.0f;
		vecNor.z = vecLine.x;

		//�@���x�N�g���P�ʉ�
		D3DXVec3Normalize(&vecNor, &vecNor);

		//��_�v�Z
		posCross = vtx2 + (vecLine * fRate);
		vecMove = *pos - posCross;
		vecMove.y = 0.0f;
		posCross.y = 0.0f;

		//���������傫��
		fDot = D3DXVec3Dot(&-vecMove, &vecNor);

		*pos = posCross + (vecMove + (vecNor * fDot));

		return true;
	}

	return false;
}

//========================================
// OBB�̕��ʂɑ΂��鉟���߂����菈��
//========================================
D3DXVECTOR3 CollideOBBToPlane(D3DXVECTOR3* posOBB, D3DXVECTOR3 vecAxial, D3DXVECTOR3 posPlane, D3DXVECTOR3 vecNorPlane)
{
	// �e�������x�N�g���v�Z
	D3DXVECTOR3 axis1 = D3DXVECTOR3(vecAxial.x, 0.0f, 0.0f);
	D3DXVECTOR3 axis2 = D3DXVECTOR3(0.0f, vecAxial.y, 0.0f);
	D3DXVECTOR3 axis3 = D3DXVECTOR3(0.0f, 0.0f, vecAxial.z);

	// �ˉe���̒������v�Z
	float lenProjection = lengthAxis(vecNorPlane, axis1, axis2, axis3);

	// �����ƃ^�[�Q�b�g�̈ʒu�֌W���v�Z
	float lenPos = D3DXVec3Dot(&(*posOBB - posPlane), &vecNorPlane);

	// �߂荞��ł���
	if (lenProjection < fabs(lenPos))
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vecNorPlane.y = 0.0f;

	// �߂荞�݋�Ŗ߂�������ς���
	if (lenPos > 0.0f)
		return vecNorPlane * (lenProjection - fabs(lenPos));
	else
		return vecNorPlane * (lenProjection + fabs(lenPos));
}

//========================================
// �~���ƃ��C�̔���
//========================================
bool RayCollideCylinder(D3DXVECTOR3 startRay, D3DXVECTOR3 EndRay, D3DXVECTOR3 posCylinder1, D3DXVECTOR3 posCylinder2,
    float fRadiusCylinder, D3DXVECTOR3 *pPosEnter, D3DXVECTOR3 *pPosExit, bool bInfinite)
{
    D3DXVECTOR3 px = posCylinder1 - startRay;
    posCylinder2 -= startRay;
    D3DXVECTOR3 sx = posCylinder2 - px;

    // �e����ϒl
    float Dvv = EndRay.x * EndRay.x + EndRay.y * EndRay.y + EndRay.z * EndRay.z;
    float Dsv = sx.x * EndRay.x + sx.y * EndRay.y + sx.z * EndRay.z;
    float Dpv = px.x * EndRay.x + px.y * EndRay.y + px.z * EndRay.z;
    float Dss = sx.x * sx.x + sx.y * sx.y + sx.z * sx.z;
    float Dps = px.x * sx.x + px.y * sx.y + px.z * sx.z;
    float Dpp = px.x * px.x + px.y * px.y + px.z * px.z;
    float rr = fRadiusCylinder * fRadiusCylinder;

    if (Dss == 0.0f)
        return false; // �~������`����Ȃ�

    float A = Dvv - Dsv * Dsv / Dss;
    float B = Dpv - Dps * Dsv / Dss;
    float C = Dpp - Dps * Dps / Dss - rr;

    if (A == 0.0f)
        return false;

    float s = B * B - A * C;
    if (s < 0.0f)
        return false; // ���C���~���ƏՓ˂��Ă��Ȃ�
    s = sqrtf(s);

    float a1 = (B - s) / A;
    float a2 = (B + s) / A;

    *pPosEnter = startRay + a1 * EndRay;
    *pPosExit = startRay + a2 * EndRay;

    if (!bInfinite)
    {// �L���~���Ƃ���
        D3DXVECTOR3 posEnterLocal = *pPosEnter - startRay;

        float f1 = CheckDot(px, posCylinder2, posEnterLocal);
        float f2 = CheckDot(posCylinder2, px, posEnterLocal);

        return f1 > 0.0f && f2 > 0.0f;
    }

    return true;
}

//========================================
// 3�x�N�g���̓��Ϗo��
//========================================
float CheckDot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 vec3)
{
    return (vec1.x - vec2.x) * (vec3.x - vec2.x) + (vec1.y - vec2.y) * (vec3.y - vec2.y) + (vec1.z - vec2.z) * (vec3.z - vec2.z);
}

//========================================
// ��`�̒��ɂ��邩�ǂ����̌v�Z
//========================================
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos)
{
	bool bHit = false;
	int nHit = 0;

	D3DXVECTOR3 aVec[4] =
	{// �z��Ɋi�[
		vtx1,
		vtx2,
		vtx3,
		vtx4,
	};

	// �S�ӂ̂Ȃ��ɂ��邩�̃`�F�b�N
	for (int i = 0; i < 4; i++)
	{
		int nNext = (i + 1) % 4;

		// �����̌������ɂ��邩�̔���
		bool bCross = IsCross(pos, aVec[i], aVec[nNext], nullptr);

		if (bCross)
		{
			nHit++;
		}
	}

	if (nHit >= 4)
	{
		bHit = true;
	}

	return bHit;
}

//========================================
// ��ʓ�����
//========================================
bool IsInScreen(const D3DXVECTOR3 pos, D3DXMATRIX mtx, D3DXVECTOR3 *pPos)
{
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer == nullptr)
	{
		return false;
	}

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);

	D3DXMATRIX projectionMatrix;
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);

	D3DXMATRIX viewMatrix;
	pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXVECTOR3 screenPosition;
	D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	if (pPos != nullptr)
	{
		*pPos = screenPosition;
	}

	if (screenPosition.x >= viewport.X && 
		screenPosition.x <= viewport.X + viewport.Width &&
		screenPosition.y >= viewport.Y && 
		screenPosition.y <= viewport.Y + viewport.Height &&
		screenPosition.z >= 0.0f && screenPosition.z <= 1.0f)
	{
		return true;
	}

	return false;
}

//=====================================================
// �΍������̏���
//=====================================================
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget)
{
	// �ϐ��錾
	D3DXVECTOR3 vecDiff;
	float fSpeedTarget;
	float fDataA, fDataB, fDataC, fDataD;
	int nFrame1, nFrame2;

	// �ڕW�̑��x���v�Z
	fSpeedTarget = D3DXVec3Length(&moveTarget);

	// �ڕW�Ƃ̈ʒu�̍������擾
	vecDiff = posTarget - pos;

	fDataA = (moveTarget.x * moveTarget.x + moveTarget.y * moveTarget.y + moveTarget.z * moveTarget.z) - fSpeedBullet * fSpeedBullet;

	fDataB = 2 * (posTarget.x * moveTarget.x + posTarget.y * moveTarget.y + posTarget.z * moveTarget.z);

	fDataC = (posTarget.x * posTarget.x + posTarget.y * posTarget.y + posTarget.z * posTarget.z);

	if (fDataA == 0)
	{// 0����h�~
		if (fDataB == 0)
		{
			return posTarget;
		}
		else
		{
			return posTarget + moveTarget * (-fDataC / fDataB);
		}
	}

	//�񎟕������̉��̌����̔��ʎ��ŕ���
	fDataD = fDataB * fDataB - 4 * fDataA * fDataC;

	if (fDataD > 0)
	{
		float fDataE = (float)sqrt(fDataD);
		nFrame1 = (int)((-fDataB - fDataE) / (2 * fDataA));
		nFrame2 = (int)((-fDataB + fDataE) / (2 * fDataA));
		//����2�Ȃ̂Ő��̐��̍ŏ��l���g��
		nFrame1 = (int)PlusMin((float)nFrame1, (float)nFrame2);
	}
	else
	{
		//������
		//������Ȃ��̂ō��̈ʒu��_��
		nFrame1 = 0;
	}

	//�\�z�ʒu��Ԃ�
	return posTarget + moveTarget * (float)nFrame1;
}

//=====================================================
// �v���X�̍ŏ��l��Ԃ�����
//=====================================================
float PlusMin(float fData1, float fData2)
{
	if (fData1 < 0 && fData2 < 0)
	{// �ǂ�������̒l�Ȃ�O��Ԃ�
		return 0;
	}
	else if (fData1 < 0)
	{
		return fData2;
	}
	else if (fData2 < 0)
	{
		return fData1;
	}

	return fData1 < fData2 ? fData1 : fData2;
}

//========================================
// �͈͓��̃����_�����l��Ԃ�����
//========================================
int RandRange(int nMax, int nMin)
{
	int nRange = nMax - nMin;
	int nRand = rand() % nRange + nMin;

	return nRand;
}

//========================================
// RGB��D3DXCOLOR�ɕϊ����鏈��
//========================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a)
{
	D3DXCOLOR col;

	col =
	{
		(float)r / 255.0f,
		(float)g / 255.0f,
		(float)b / 255.0f,
		(float)a / 255.0f,
	};

	return col;
}

//========================================
// 3D���W�̐��`�⊮
//========================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime)
{
	D3DXVECTOR3 pos = start;

	D3DXVECTOR3 vecDiff = end - start;

	pos += vecDiff * fTime;

	return pos;
}

//========================================
// �����ɑ΂���̎ˉe�ϊ�
//========================================
float lengthAxis(D3DXVECTOR3 sep, D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3)
{
	float length1, length2, length3;

	length1 = fabs(D3DXVec3Dot(&sep, &e1));
	length2 = fabs(D3DXVec3Dot(&sep, &e2));
	length3 = fabs(D3DXVec3Dot(&sep, &e3));

	return length1 + length2 + length3;
}

//========================================
//3������Ԃł̍s��ɂ���]���W�ϊ��֐�
//(�C�ӂ̓_����̃I�t�Z�b�g�ʒu���p�x�Ƌ����ŕϊ�)
//========================================
D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset)
{
	D3DXVECTOR3 posAnswer;
	D3DXMATRIX mtxO, mtxAnswer;
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxO);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	mtxPalent = mtxO;

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxAnswer);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	posAnswer = GetMtxPos(mtxAnswer);

	return posAnswer;
}

//========================================
//3������Ԃł̍s��ɂ���]���W�ϊ�����̖@���v�Z�֐�
//========================================
D3DXVECTOR3 NorRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin)
{
	D3DXVECTOR3 posMax, posMin;
	D3DXVECTOR3 vecNor;

	posMax = PosRelativeMtx(posO, rot, D3DXVECTOR3(offsetMax.x, offsetMax.y, offsetMax.z));
	posMin = PosRelativeMtx(posO, rot, D3DXVECTOR3(offsetMax.x, offsetMin.y, offsetMax.z));

	vecNor = posMax - posMin;

	D3DXVec3Normalize(&vecNor, &vecNor);

	return vecNor;
}

//========================================
// �s�񂩂�I�C���[�p�𒊏o
//========================================
D3DXVECTOR3 ExtractEulerAngles(D3DXMATRIX mtx)
{
    D3DXVECTOR3 angles;

    // �s�b�`�ix������̉�]�j
    angles.x = asinf(-mtx._32);

    // �R�T�C�����g�p���ăW���o�����b�N���`�F�b�N
    float cosPitch = cosf(angles.x);

    // ���[�iy������̉�]�j
    angles.y = atan2f(mtx._31, mtx._33);

    // ���[���iz������̉�]�j
    angles.z = atan2f(mtx._12, mtx._22);

    return angles;
}

//========================================
// Y���x�N�g����X���Ɍ����ē|������
//========================================
D3DXVECTOR3 RotateYToX(D3DXVECTOR3 vec)
{
    // Y���Ɍ������x�N�g����X���Ɍ������߂̉�]�s����v�Z
    D3DXVECTOR3 from(0, 1, 0);  // Y������
    D3DXVECTOR3 to(1, 0, 0);    // X������

    D3DXVECTOR3 axis;
    D3DXVec3Cross(&axis, &from, &to);
    float cosTheta = D3DXVec3Dot(&from, &to);
    float angle = acosf(cosTheta);

    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationAxis(&rotationMatrix, &axis, angle);

    // ��]�s����x�N�g���ɓK�p
    D3DXVECTOR3 result;
    D3DXVec3TransformCoord(&result, &vec, &rotationMatrix);

    return result;
}
}	// namespace universal