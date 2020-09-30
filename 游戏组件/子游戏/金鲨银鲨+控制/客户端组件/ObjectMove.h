#pragma once


//��������
#define GEM_SHOW_INDEX				12


//������
struct DPOINT
{
	DOUBLE				x;
	DOUBLE				y;

	DPOINT() {  x = 0.0; y = 0.0; }
	DPOINT( POINT& Par ) {  x = Par.x; y = Par.y; }
	DPOINT( DPOINT& Par ) {  x = Par.x; y = Par.y; }
	DPOINT( DOUBLE ParX, DOUBLE ParY ) { x = ParX; y = ParY; }
	void operator+= (DPOINT& point) { x += point.x; y += point.y; }
	inline void SetPoint( DOUBLE ParX, DOUBLE ParY ) { x = ParX; y = ParY; }
};

//�����ƶ���
class CObjectMove
{
	//����
public:
	BYTE							cbFrameIndex;			//��������
	BYTE							bMove;					//�Ƿ��ƶ�
	UINT							nCreateTime;			//����ʱ��
	UINT							nMoveTime;				//�ƶ�ʱ��
	UINT							nFrameTime;				//��֡ʱ��
	DPOINT							ptPosition;				//λ��
	DPOINT							ptEnd;					//����λ��
	DPOINT							ptBegin;				//��ʼλ��
	CLapseCount						LapseCount;				//���ż���

public:
	CObjectMove()
	{
		bMove			=	FALSE;
		cbFrameIndex	=	0;
		nCreateTime		=	0;
		nMoveTime		=	1;
		nFrameTime		=	1;
		ptPosition.SetPoint(0, 0);
		ptBegin.SetPoint(0, 0);
		ptEnd.SetPoint(0, 0);
	}

	CObjectMove( DPOINT ParBegin, DPOINT ParEnd, UINT nParMoveTime, UINT nParFrameTime )
	{
		if( nParMoveTime == 0 )
			nParMoveTime = 1;

		if( nParFrameTime == 0 )
			nParFrameTime = 1;

		bMove			=	TRUE;
		cbFrameIndex	=	0;
		nCreateTime		=	GetTickCount();
		nMoveTime		=	nParMoveTime;
		nFrameTime		=	nParFrameTime;
		ptPosition		=   ParBegin;
		ptBegin			=	ParBegin;
		ptEnd			=	ParEnd;
		LapseCount.Initialization();
	}

	~CObjectMove(){}

	VOID BeginMove( DPOINT ParBegin, DPOINT ParEnd, UINT nParMoveTime, UINT nParFrameTime )
	{
		if( nParMoveTime == 0 )
			nParMoveTime = 1;

		if( nParFrameTime == 0 )
			nParFrameTime = 1;

		bMove			=	TRUE;
		cbFrameIndex	=	0;
		nCreateTime		=	GetTickCount();
		nMoveTime		=	nParMoveTime;
		nFrameTime		=	nParFrameTime;
		ptPosition		=   ParBegin;
		ptBegin			=	ParBegin;
		ptEnd			=	ParEnd;
		LapseCount.Initialization();
	}

	BOOL Move()
	{
		//֡��ת��
		if ( LapseCount.GetLapseCount(nFrameTime) > 0 )
			cbFrameIndex++;

		if ( nMoveTime == 0 || !bMove )
			return FALSE;

		//ƫ��ʱ��
		UINT nOffSetTime = GetTickCount() - nCreateTime;

		//λ���ƶ�
		DOUBLE dOffSet = (DOUBLE)nOffSetTime / (DOUBLE)nMoveTime;
		if ( dOffSet > 1.00 )
			dOffSet = 1.00;

		ptPosition.x = (ptEnd.x - ptBegin.x) * dOffSet + ptBegin.x;
		ptPosition.y = (ptEnd.y - ptBegin.y) * dOffSet + ptBegin.y;

		if ( nOffSetTime >= nMoveTime )
		{
			ptPosition = ptEnd;
			bMove = FALSE;
			return TRUE;
		}
		return FALSE;
	}
};


//����滭�ṹ
#define FISH_STEP				5
struct tgaFishDraw
{

public:
	INT					nFishIndex;				//������
	BYTE				nFishType;				//������
	BYTE				bLinear;				//ֱ����
	BOOL				bWhetherToCreate;		//�Ƿ񴴽�

	BYTE				cbFrameIndex;			//��������

	UINT				nOffsetTime;			//ƫ��ʱ��
	BYTE				cbCreateTime;			//����ʱ��
	UINT				nClientTime;			//�ͻ���ʱ��

	BYTE				cbMoveCount;			//�ƶ�����
	BYTE				cbMoveTime[FISH_STEP];	//�ܲ���
	DPOINT				ptMovePos[FISH_STEP];	//�ƶ�λ��

	DPOINT				ptLast;					//�ϴ�λ��
	DPOINT				ptPosition;				//��ǰλ��

	CLapseCount			LapseCount;				//���ż���

public:
	tgaFishDraw( INT nParFishIndex, BYTE nParFishType, BYTE bParLinear, 
		UINT nParOffsetTime, BYTE cbParCreateTime, UINT nParClientTime,
		BYTE cbParMoveCount , POINT* pParMovePos , BYTE* pParMoveTime )
	{
		nFishIndex = nParFishIndex;
		nFishType = nParFishType;
		bLinear = bParLinear;

		cbFrameIndex = rand()%255;

		nOffsetTime = nParOffsetTime;
		cbCreateTime = cbParCreateTime;
		nClientTime = nParClientTime;

		cbMoveCount = cbParMoveCount;
		for(INT i = 0; i < FISH_STEP; ++i )
		{
			ptMovePos[i].SetPoint( pParMovePos[i].x, pParMovePos[i].y );
			cbMoveTime[i] = pParMoveTime[i];
		}

		ptLast.SetPoint( pParMovePos[0].x, pParMovePos[0].y );
		ptPosition.SetPoint( pParMovePos[0].x, pParMovePos[0].y );
		bWhetherToCreate = FALSE;
		LapseCount.Initialization();
	}

	//��֪Բ��,�뾶,���� ��Բ������һ
	DPOINT RotatePoint( DPOINT ptCircle, DOUBLE dRadius, DOUBLE dRadian )
	{
		DPOINT point;
		point.x = dRadius*cos(dRadian) + ptCircle.x;
		point.y = dRadius*sin(dRadian) + ptCircle.y;
		return point;
	}

	BOOL WhetherToCreate()
	{
		return bWhetherToCreate;
	}


	BOOL Move()
	{
		UINT unTime = (GetTickCount() + nOffsetTime - nClientTime);

		if (unTime < (UINT)cbCreateTime * 1000 )
		{
			bWhetherToCreate = FALSE;
			return TRUE;
		}

		unTime -= cbCreateTime * 1000 ;
		UINT unAllTime = 0;
		INT  nMoveIndex = 0;
		BOOL bSuccess = FALSE;
		for ( int i = 0; i < cbMoveCount; ++i )
		{
			unAllTime += (cbMoveTime[i] * 1000);
			if ( unTime <= unAllTime )
			{
				nMoveIndex = i;
				bSuccess = TRUE;
				break;
			}
		}

		if ( !bSuccess || nMoveIndex >= (cbMoveCount - 1) )
		{
			bWhetherToCreate = FALSE;
			return FALSE;
		}

		for( int i = 0; i < nMoveIndex; ++i )
		{
			unTime -= cbMoveTime[i] * 1000;
		}

		DPOINT ptLastTemp( ptPosition.x, ptPosition.y );
		DOUBLE dProportion = (DOUBLE)(unTime)/(DOUBLE)(cbMoveTime[nMoveIndex] * 1000);

		if( bLinear )
		{
			ptPosition.x = (ptMovePos[nMoveIndex + 1].x - ptMovePos[nMoveIndex].x) * ( dProportion ) + ptMovePos[nMoveIndex].x;
			ptPosition.y = (ptMovePos[nMoveIndex + 1].y - ptMovePos[nMoveIndex].y) * ( dProportion ) + ptMovePos[nMoveIndex].y;
		}
		else
		{
			DPOINT ptStart(ptMovePos[nMoveIndex].x, ptMovePos[nMoveIndex].y);
			DPOINT ptObjective(ptMovePos[nMoveIndex + 1].x, ptMovePos[nMoveIndex + 1].y);
			DOUBLE dDistance = sqrt(pow(abs(ptObjective.x - ptStart.x),2) + pow(abs(ptObjective.y - ptStart.y),2));
			DPOINT ptCircle (	(max(ptObjective.x,ptStart.x) - min(ptObjective.x,ptStart.x))/2 + min(ptObjective.x,ptStart.x), 
				(max(ptObjective.y,ptStart.y) - min(ptObjective.y,ptStart.y))/2 + min(ptObjective.y,ptStart.y)	);
			DOUBLE dAngleDeviation = atan2( ptObjective.y - ptStart.y, ptObjective.x - ptStart.x);
			DOUBLE dAngleRotating = 0.0;
			DOUBLE dAngle = D3DX_PI * (((nMoveIndex%2) == 0) ? 1 : -1);
			

			//�ƶ�����
			if ( dAngleDeviation <= D3DX_PI/2 && dAngleDeviation > -D3DX_PI/2)
				dAngleRotating = dAngle * ( 1.0 - dProportion);
			else
				dAngleRotating = dAngle * ( dProportion );

			//����ƫ�ƻ���
			if( dAngleDeviation <= D3DX_PI/2 && dAngleDeviation >= 0.0 )
				dAngleRotating += dAngleDeviation;
			else if ( dAngleDeviation > D3DX_PI/2 && dAngleDeviation <= D3DX_PI )
				dAngleRotating -= (D3DX_PI - dAngleDeviation);
			else if ( dAngleDeviation < 0.0 && dAngleDeviation >= -D3DX_PI/2 )
				dAngleRotating += dAngleDeviation;
			else if ( dAngleDeviation < -D3DX_PI/2 && dAngleDeviation > -D3DX_PI )
				dAngleRotating += D3DX_PI - abs(dAngleDeviation);

			ptPosition = RotatePoint( ptCircle, dDistance/2 ,dAngleRotating );
		}


		if( ptLastTemp.x - ptPosition.x != 0.0 || ptLastTemp.y - ptPosition.y != 0.0 )
		{
			ptLast = ptLastTemp;
		}

		if ( nMoveIndex >= (cbMoveCount - 1) )
		{
			bWhetherToCreate = FALSE;
			return FALSE;
		}

		bWhetherToCreate = TRUE;
		return TRUE;
	}
	~tgaFishDraw(void){}

};

struct tgaAnimalsDraw
{
	bool					bBright;					//��
	bool					bflicker;					//����
	CRect					rectDraw;					//�滭����

	tgaAnimalsDraw()
	{
		bBright = false;
		bflicker = false;
		rectDraw.SetRect(0, 0, 0, 0);
	};
	~tgaAnimalsDraw(){};
};