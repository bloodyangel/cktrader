#pragma once
#ifndef __BACKTESTGATE_H__
#define __BACKTESTGATE_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"

namespace cktrader{

class EventEngine;

class BacktestGate :public IGateway
{
public:
	BacktestGate(std::string gateWayName = "backtest");
	~BacktestGate();

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

	void connect(std::string& userID, std::string& password, std::string& brokerID, std::string& mdAddress, std::string& tdAddress);
	void subscribe(SubscribeReq& subReq);
	std::string sendOrder(OrderReq& orderReq);
	void cancelOrder(CancelOrderReq& cancelOrderReq);
	void qryAccount();
	void qryPosition();
	void close();

	void query(Datablk& notUse);

	std::atomic<bool> mdConnected = false;
	std::atomic<bool> tdConnected = false;

	void calculateBacktestingResult();

private:
	EventEngine *m_event_service;
	std::string gateWayName;
};

}//cktrader

#endif