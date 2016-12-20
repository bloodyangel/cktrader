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
	std::string host;	// ���ӵ�ַ
	int port;  // ���Ӷ˿�
	int clientId;      // �û����
	std::string accountCode; // �˻����

	std::atomic<long> tickerId = 0;     // ��������ʱ�Ĵ�����
	

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

	std::map<std::string, SubscribeReq> subscribeReqDict;     // �������涩��������ֵ�

	std::map<long, TickData> tickDict;      // tick�����ֵ䣬keyΪtickerId��valueΪVtTickData����
	std::map<long, std::string> tickProductDict;  //tick��Ӧ�Ĳ�Ʒ�����ֵ䣬keyΪtickerId��valueΪ��Ʒ����

	std::map<long, OrderData> orderDict; // �����ֵ䣬keyΪorderId��valueΪVtOrderData����

	std::map<std::string, AccountData > accountDict;  // �˻��ֵ�

	std::map<std::string, ContractData> contractDict;  // ��Լ�ֵ�
};

}

#endif