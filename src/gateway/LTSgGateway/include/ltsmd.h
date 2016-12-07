#pragma once
#ifndef __LTSMD_H__
#define __LTSMD_H__

#include <string>
#include <atomic>
#include <vector>

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "SecurityFtdcMdApi.h"
#include "ltsdefs.h"
#include "LTSGateway.h"

namespace cktrader {

class LtsMd : public CSecurityFtdcMdSpi
{
private:
	CSecurityFtdcMdApi* api;			//API对象

	EventEngine *m_event_service;
	LTSGateway *m_gateWay;

	std::string gateWayName;

	std::string userID;// 账号
	std::string password;//密码
	std::string brokerID;//        # 经纪商代码
	std::string address;//        # 服务器地址

	std::vector<SubscribeReq> *subscribedSymbols = nullptr;//    已订阅合约代码

	std::atomic<int> reqID = 0;//请求ID

	std::atomic<bool> connectionStatus = false;      // 连接状态
	std::atomic<bool> loginStatus = false;//            登录状态

public:
	LtsMd(EventEngine* pEvent,LTSGateway* gateway);
	~LtsMd();

	//-------------------------------------------------------------------------------------
	//API回调函数
	//-------------------------------------------------------------------------------------

	//当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	//当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	//@param nReason 错误原因
	//        0x1001 网络读失败
	//        0x1002 网络写失败
	//        0x2001 接收心跳超时
	//        0x2002 发送心跳失败
	//        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	//心跳超时警告。当长时间未收到报文时，该方法被调用。
	//@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	//错误应答
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//登录请求响应
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//登出请求响应
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//订阅行情应答
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//取消订阅行情应答
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//深度行情通知
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);


	//-------------------------------------------------------------------------------------
	//task：任务
	//-------------------------------------------------------------------------------------

	void processFrontConnected(Datablk& data);

	void processFrontDisconnected(Datablk& data);

	void processHeartBeatWarning(Datablk& data);

	void processRspError(Datablk& data);

	void processRspUserLogin(Datablk& data);

	void processRspUserLogout(Datablk& data);

	void processRspSubMarketData(Datablk& data);

	void processRspUnSubMarketData(Datablk& data);

	void processRtnDepthMarketData(Datablk& data);

	//-------------------------------------------------------------------------------------
	//req:主动函数的请求字典
	//-------------------------------------------------------------------------------------
	void connect(std::string userID, std::string password, std::string brokerID, std::string mdAddress);

	void subscribe(SubscribeReq& subscribeReq);

	int subscribeMarketData(std::string instrumentID,std::string exchageID);
	int unSubscribeMarketData(std::string instrumentID, std::string exchageID);
	int close();
};

}

#endif