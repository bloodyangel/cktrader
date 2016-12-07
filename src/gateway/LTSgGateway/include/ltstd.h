#pragma once
#ifndef __LTSTD_H__
#define __LTSTD_H__

#include "ltsdefs.h"
#include "LTSGateway.h"

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "SecurityFtdcTraderApi.h"

namespace cktrader {

class LtsTd : public CSecurityFtdcTraderSpi
{
private:
	CSecurityFtdcTraderApi* api;			//API对象
	
	EventEngine *m_event_service;
	LTSGateway *gateWay;
	std::string gateWayName;

	std::atomic<int> reqID = 0;//操作请求编号
	std::atomic<int> orderRef = 0;// 订单编号

	std::atomic<bool> connectionStatus = false;//      连接状态
	std::atomic<bool> loginStatus = false;// 登录状态

	std::string userID;// 账号
	std::string password;//密码
	std::string brokerID;//经纪商代码
	std::string address;// 服务器地址
	std::string productInfo;
	std::string authCode;
	std::string randCode;

	int frontID;//前置机编号
	int sessionID;// 会话编号	

public:
	LtsTd(EventEngine* pEvent, LTSGateway* gateway);
	~LtsTd();

	//-------------------------------------------------------------------------------------
	//API回调函数
	//-------------------------------------------------------------------------------------

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///错误应答
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登录请求响应
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///获取认证随机码请求响应
	virtual void OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CSecurityFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CSecurityFtdcTradeField *pTrade);

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo);

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo);

	///Liber发起出金应答
	virtual void OnRspFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Liber发起出金通知
	virtual void OnRtnFundOutByLiber(CSecurityFtdcFundTransferField *pFundTransfer);

	///Liber发起出金错误回报
	virtual void OnErrRtnFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo);

	///银行发起入金通知
	virtual void OnRtnFundInByBank(CSecurityFtdcFundTransferField *pFundTransfer);

	///资金内转应答
	virtual void OnRspFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///资金内转流水通知
	virtual void OnRtnFundInterTransferSerial(CSecurityFtdcFundInterTransferSerialField *pFundInterTransferSerial);

	///资金内转错误回报
	virtual void OnErrRtnFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo);

	//-------------------------------------------------------------------------------------
	//task：任务
	//-------------------------------------------------------------------------------------

	void processFrontConnected(Datablk& data);

	void processFrontDisconnected(Datablk& data);

	void processHeartBeatWarning(Datablk& data);

	void processRspError(Datablk& data);

	void processRspUserLogin(Datablk& data);

	void processRspUserLogout(Datablk& data);

	void processRspFetchAuthRandCode(Datablk& data);

	void processRspOrderInsert(Datablk& data);

	void processRspOrderAction(Datablk& data);

	void processRspUserPasswordUpdate(Datablk& data);

	void processRspTradingAccountPasswordUpdate(Datablk& data);

	void processRtnOrder(Datablk& data);

	void processRtnTrade(Datablk& data);

	void processErrRtnOrderInsert(Datablk& data);

	void processErrRtnOrderAction(Datablk& data);

	void processRspFundOutByLiber(Datablk& data);

	void processRtnFundOutByLiber(Datablk& data);

	void processErrRtnFundOutByLiber(Datablk& data);

	void processRtnFundInByBank(Datablk& data);

	void processRspFundInterTransfer(Datablk& data);

	void processRtnFundInterTransferSerial(Datablk& data);

	void processErrRtnFundInterTransfer(Datablk& data);

	//-------------------------------------------------------------------------------------
	//req:主动函数的请求字典
	//-------------------------------------------------------------------------------------
	void connect(std::string userID,
					std::string password, 
					std::string brokerID, 
					std::string address,
					std::string productInfo,
					std::string authCode);

	void login();

	std::string sendOrder(OrderReq& orderReq);
	void cancelOrder(CancelOrderReq& req);

	void close();
};

}//cktrader

#endif