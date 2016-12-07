#pragma once
#ifndef __CTPGATEWAY_H__
#define __CTPGATEWAY_H__

#include <string>
#include <mutex>

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"
#include "ctpdefs.h"

#include <atomic>

namespace cktrader {

#define CKTRADER_CTP_FILE "ctp_setting.json"

CAPI_CKTRADER
{
	CK_EXPORTS  IGateway* CreateGateway(EventEngine* pEvent,const char* x);
	CK_EXPORTS int ReleaseGateway(IGateway*p);
}

class EventEngine;
class CtpMd;
class CtpTd;

class CTPGateWay:public IGateway
{
public:
    CTPGateWay(EventEngine* pEvent,std::string gateWayName = "ctp");
    ~CTPGateWay();

	void onTick(TickData& tick);
	void onTrade(TradeData& trade);
	void onOrder(OrderData& order);
	void onPosition(PositionData& position);
	void onAccount(AccountData& account);
	void onError(ErrorData& error);
	void onLog(LogData& log);
	void onContract(ContractData& contract);

	std::string getName();
	EventEngine* getEventEngine();

    void connect(std::string& userID,std::string& password);
    void subscribe(SubscribeReq& subReq);
    std::string sendOrder(OrderReq& orderReq);
    void cancelOrder(CancelOrderReq& cancelOrderReq);
    void qryAccount();
    void qryPosition();
    void close();

	std::string readFile(std::string fileName);
	void writeLog(std::string logInfo);

    void query(Datablk& notUse);

    std::atomic<bool> mdConnected = false;
	std::atomic<bool> tdConnected = false;

private:
	EventEngine *m_event_service;
    std::string gateWayName;
    CtpMd *md;
    CtpTd *td;

    int qryCount = 0;          // ��ѯ��������ʱ
    int qryTrigger = 2;        //��ѯ������

    mutable std::recursive_mutex the_mutex;
};

}//cktrader
#endif