#pragma once
#ifndef __TGATEWAY_H__
#define __TGATEWAY_H__

#include <string>
#include <mutex>

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "eventservice/eventengine.h"

namespace cktrader {

class IServiceMgr;

class IGateway
{
public:
	IGateway()
	{
		m_event_service = nullptr;
	}

	IGateway(EventEngine *event_service)
	{
		m_event_service = event_service;
	}

public:
	//下层数据接口主动调用函数
	virtual void onTick(TickData& tick)
	{
		Task e1;
		e1.type = EVENT_TICK;
		e1.task_data = tick;
		m_event_service->put(e1);

		Task e2;
		e2.type = std::string(EVENT_TICK) + tick.symbol;
		e2.task_data = tick;
		m_event_service->put(e2);
	}

	virtual void onTrade(TradeData& trade)
	{
		Task e1;
		e1.type = EVENT_TRADE;
		e1.task_data = trade;
		m_event_service->put(e1);

		Task e2;
		e2.type = std::string(EVENT_TRADE) + trade.symbol;
		e2.task_data = trade;
		m_event_service->put(e2);
	}

	virtual void onOrder(OrderData& order)
	{
		Task e1;
		e1.type = EVENT_ORDER;
		e1.task_data = order;
		m_event_service->put(e1);

		Task e2;
		e2.type = std::string(EVENT_ORDER) + order.symbol;
		e2.task_data = order;
		m_event_service->put(e2);
	}

	virtual void onPosition(PositionData& position)
	{
		Task e1;
		e1.type = EVENT_POSITION;
		e1.task_data = position;
		m_event_service->put(e1);

		Task e2;
		e2.type = std::string(EVENT_POSITION) + position.symbol;
		e2.task_data = position;
		m_event_service->put(e2);
	}

	virtual void onAccount(AccountData& account)
	{
		Task e1;
		e1.type = EVENT_ACCOUNT;
		e1.task_data = account;
		m_event_service->put(e1);

		Task e2;
		e2.type = std::string(EVENT_ACCOUNT) + account.tAccountID;
		e2.task_data = account;
		m_event_service->put(e2);
	}

	virtual void onError(ErrorData& error)
	{
		Task e1;
		e1.type = EVENT_ERROR;
		e1.task_data = error;
		m_event_service->put(e1);
	}

	virtual void onLog(LogData& log)
	{
		Task e1;
		e1.type = EVENT_LOG;
		e1.task_data = log;
		m_event_service->put(e1);
	}

	virtual void onContract(ContractData& contract)
	{
		Task e1;
		e1.type = EVENT_CONTRACT;
		e1.task_data = contract;
		m_event_service->put(e1);
	}

public:
	//上层主动调用函数，如strategy，servicemgr
	virtual std::string getName()=0;

	virtual void connect(std::string& userID, std::string& password) = 0;
	virtual void subscribe(SubscribeReq& subReq) = 0;
	virtual std::string sendOrder(OrderReq& orderReq) = 0;
	virtual void cancelOrder(CancelOrderReq& cancelOrderReq) = 0;
	virtual void qryAccount() = 0;
	virtual void qryPosition() = 0;
	virtual void close() = 0;

private:
	EventEngine *m_event_service;
};

}//cktrader
#endif
