#pragma once
#ifndef __IBGATEWAY_H__
#define __IBGATEWAY_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"

#include <atomic>

namespace cktrader {

#define CKTRADER_IB_FILE "ib_setting.json"

CAPI_CKTRADER
{
	CK_EXPORTS  IGateway* CreateGateway(EventEngine* pEvent,const char* x);
	CK_EXPORTS int ReleaseGateway(IGateway*p);
}

class EventEngine;
class IBApi;

class IBGateway : public IGateway
{
private:
	std::string host;	// 连接地址
	int port;  // 连接端口
	int clientId;      // 用户编号
	std::string accountCode; // 账户编号

	std::atomic<long> tickerId = 0;     // 订阅行情时的代码编号
	

	EventEngine* m_event_service = nullptr;
	std::string gatewayName;

	IBApi* api = nullptr;

protected:
	std::string readFile(std::string fileName);

public:
	IBGateway(EventEngine* pEvent, std::string gateWayName = "ib");
	~IBGateway();

	std::string getName();
	EventEngine* getEventEngine();

	void connect(std::string& userID, std::string& password);
	void subscribe(SubscribeReq& subReq);
	std::string sendOrder(OrderReq& orderReq);
	void cancelOrder(CancelOrderReq& cancelOrderReq);
	void qryAccount();
	void qryPosition();
	void close();

	void writeLog(std::string logInfo);

	std::atomic<long> orderId;
	std::atomic<bool> connected = false;

	std::map<std::string, SubscribeReq> subscribeReqDict;     // 用来保存订阅请求的字典

	std::map<long, TickData> tickDict;      // tick快照字典，key为tickerId，value为VtTickData对象
	std::map<long, std::string> tickProductDict;  //tick对应的产品类型字典，key为tickerId，value为产品类型

	std::map<long, OrderData> orderDict; // 报单字典，key为orderId，value为VtOrderData对象

	std::map<std::string, AccountData > accountDict;  // 账户字典

	std::map<std::string, ContractData> contractDict;  // 合约字典
};

}

#endif