#pragma once

#include "Stdafx.h"
#include "ChessBorad.h"
#include "ChessManual.h"
#include "GameScoreWnd.h"
#include "DlgUserRequest.h"

//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_REGRET					(WM_USER+101)						//悔棋消息
#define IDM_PEACE					(WM_USER+102)						//求和消息
#define IDM_GIVEUP					(WM_USER+103)						//认输消息
#define IDM_TRADE					(WM_USER+104)						//对换消息
#define IDM_SHOW_ID					(WM_USER+105)						//手数消息
#define IDM_PRESERVE				(WM_USER+106)						//保存消息
#define IDM_STUDY					(WM_USER+107)						//研究消息
#define IDM_REGRET_ANSWER			(WM_USER+108)						//悔棋应答
#define IDM_PEACE_ANSWER			(WM_USER+109)						//求和应答

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//友元定义
	friend class CGameClientEngine;

	//变量定义
protected:
	WORD							m_wBlackUser;						//黑棋用户
	WORD							m_wGameClock;						//局时时间
	WORD							m_wLeftClock[2];					//剩余时间

	//位置信息
protected:
	CSize							m_UserInfoSize;						//位置信息
	CSize							m_ButtonBackSize;					//位置信息
	CPoint							m_ChessFlagPoint[2];				//标志位置
	int								m_nYBorder ;						//
	int								m_nXBorder ;						//
	//界面变量
protected:
	CBitImageEx						m_ImageBack;						//背景图案
	CBitImageEx						m_ImageFlag;						//标志视图
	CBitImageEx						m_ImageClock;						//时钟标志
	CBitImageEx						m_ImageUserInfo;					//用户信息
	CBitImageEx						m_ImageDecorate;					//装饰位图
	CBitImageEx						m_ImageButtonBack;					//按钮底图

	//控件变量
public:
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btRegret;							//悔棋按钮
	CSkinButton						m_btPeace;							//求和按钮
	CSkinButton						m_btGiveUp;							//认输按钮
	CSkinButton						m_btShowID;							//手数按钮
	//CSkinButton						m_btTrade;							//对换按钮
	CSkinButton						m_btPreserve;						//保存按钮
	CSkinButton						m_btStudy;							//研究按钮
	CChessBorad						m_ChessBorad;						//游戏棋盘
	CChessManual					m_ChessManual;						//棋谱窗口
	CGameScoreWnd					m_GameScoreWnd;						//成绩窗口
	CDlgUserRequest					m_PeaceRequest;						//和棋请求
	CDlgUserRequest					m_RegretRequest;					//悔棋请求

#ifdef VIDEO_GAME

	//视频组件
private:
	CVideoServiceManager			m_VedioServiceManager;				//视频对象
	CVideoServiceControl			m_DlgVedioService[GAME_PLAYER];		//视频组件

#endif


	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();


	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }


	//功能函数
public:
	//和棋请求
	void ShowPeaceRequest();
	//悔棋请求
	void ShowRegretRequest();
	//设置黑棋
	void SetBlackUser(WORD wBlackUser);
	//设置时间
	void SetGameClock(WORD wGameClock);
	//设置时间
	void SetUserClock(WORD wViewChairID, WORD wLeftClock);
	//更新视图
	void RefreshGameView();
	//更新视图
	void RefreshGameView(CRect rect);
	//内部函数 
private:
	//更新状态
	void UpdateUserInfoArea();
	//用户时钟
	void DrawUserClock(CDC * pDC, int nXPos, int nYPos, WORD wClockCount);

	//控件消息
protected:
	//点击棋盘
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);

	//消息映射
protected:
	//开始按钮
	afx_msg void OnStart();
	//悔棋按钮
	afx_msg void OnRegret();
	//求和按钮
	afx_msg void OnPeace();
	//认输按钮
	afx_msg void OnGiveUp();
	//手数按钮
	afx_msg void OnShowID();
	//对换按钮
	afx_msg void OnTrade();
	//保存按钮
	afx_msg void OnPreserve();
	//研究按钮
	afx_msg void OnStudy();
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
