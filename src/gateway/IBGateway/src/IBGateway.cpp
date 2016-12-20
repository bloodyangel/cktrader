#include "IBGateway.h"
#include "utils/json11.hpp"
#include "IBApi.h"
#include "Order.h"

#include <fstream>

namespace cktrader {

IBGateway::IBGateway(EventEngine* pEvent, std::string gateWayName)
{
	this->m_event_service = pEvent;
	gatewayName = gateWayName;

	api = new IBApi(this);
}

IBGateway::~IBGateway()
{
	delete api;
	api = nullptr;
}

std::string IBGateway::getName()
{
	return gatewayName;
}

EventEngine* IBGateway::getEventEngine()
{
	return m_event_service;
}

void IBGateway::connect(std::string& userID, std::string& password)
{
	std::string data = readFile(CKTRADER_IB_FILE);

	if (data.empty())
	{
		return;
	}

	std::string err;
	auto setting_json = json11::Json::parse(data, err);
	if (!err.empty())
	{
		return;
	}

	this->host = setting_json["host"].string_value();
	this->port = setting_json["port"].int_value();
	this->clientId = setting_json["clientId"].int_value();

	this->api->eConnect(this->host, this->port, this->clientId, false);

	api->reqCurrentTime();

	api->reqAccountUpdates(true,userID);
}

void IBGateway::subscribe(SubscribeReq& subReq)
{
	if (!connected)
	{
		subscribeReqDict.insert(std::pair<std::string,SubscribeReq>(subReq.symbol, subReq));
	}

	Contract contract;
	contract.localSymbol = subReq.symbol;
	contract.exchange = subReq.exchange;

	if (subReq.productClass == PRODUCT_EQUITY)
	{
		contract.secType = "STK";
	}
	else if (subReq.productClass == PRODUCT_FUTURES)
	{
		contract.secType = "FUT";
	}
	else if (subReq.productClass == PRODUCT_OPTION)
	{
		contract.secType = "OPT";
	}
	else if (subReq.productClass == PRODUCT_FOREX)
	{
		contract.secType = "CASH";
	}
	contract.currency = subReq.currency;
	//contract.expiry = orderReq.expiry;
	contract.strike = subReq.strikePrice;

	if (subReq.optionType == OPTION_CALL)
	{
		contract.right = "CALL";
	}
	else
	{
		contract.right = "PUT";
	}

	tickerId++;
	api->reqContractDetails(tickerId, contract);

	// 创建合约对象并保存到字典中
	ContractData ct;
	ct.gateWayName = gatewayName;
	ct.symbol = subReq.symbol;
	ct.exchange = subReq.exchange;
	ct.tSymbol = ct.symbol + "." + ct.exchange;
	ct.productClass = subReq.productClass;
	contractDict.insert(std::make_pair(ct.tSymbol, ct));

	// 订阅行情
	tickerId++;
	api->reqMktData(tickerId, contract, "", false, TagValueListSPtr());

	// 创建Tick对象并保存到字典中
	TickData tick;
	tick.symbol = subReq.symbol;
	tick.exchange = subReq.exchange;
	tick.tSymbol = tick.symbol + "." + tick.exchange;
	tick.gateWayName = gatewayName;
	tickDict.insert(std::pair<long, TickData>(tickerId, tick));
	tickProductDict.insert(std::pair<long,std::string>(tickerId, subReq.productClass));
}

std::string IBGateway::sendOrder(OrderReq& orderReq)
{
	//增加报单号1，最后再次进行查询
	//这里双重设计的目的是为了防止某些情况下，连续发单时，nextOrderId的回调推送速度慢导致没有更新
	orderId++;

	//创建合约对象
	Contract contract;
	contract.localSymbol = orderReq.symbol;
	contract.exchange = orderReq.exchange;
	if (orderReq.productClass == PRODUCT_EQUITY)
	{
		contract.secType = "STK";
	}
	else if (orderReq.productClass == PRODUCT_FUTURES)
	{
		contract.secType = "FUT";
	}
	else if (orderReq.productClass == PRODUCT_OPTION)
	{
		contract.secType = "OPT";
	}
	else if (orderReq.productClass == PRODUCT_FOREX)
	{
		contract.secType = "CASH";
	}

	contract.currency = orderReq.currency;
	//contract.expiry = orderReq.expiry;
	contract.strike = orderReq.strikePrice;
	if (orderReq.optionType == OPTION_CALL)
	{
		contract.right = "CALL";
	}
	else
	{
		contract.right = "PUT";
	}

	//创建委托对象
	Order order;
	order.orderId = orderId;
	order.clientId = clientId;

	if (orderReq.direction == DIRECTION_LONG)
	{
		order.action = "BUY";
	}
	else
	{
		order.action = "SELL";
	}

	order.lmtPrice = orderReq.price;
	order.totalQuantity = orderReq.volume;

	if (orderReq.priceType == PRICETYPE_LIMITPRICE)
	{
		order.orderType = "LMT";
	}
	else
	{
		order.orderType = "MKT";
	}

	//发送委托
	api->placeOrder(orderId, contract, order);

	//查询下一个有效编号
	api->reqIds(1);

	// 返回委托编号
	std::string vtOrderID = gatewayName + "." + std::to_string(orderId);
	return vtOrderID;
}

void IBGateway::cancelOrder(CancelOrderReq& cancelOrderReq)
{
	api->cancelOrder(atol(cancelOrderReq.orderID.c_str()));
}

void IBGateway::qryAccount()
{
	writeLog("IB接口账户信息提供主推更新，无需查询");
}

void IBGateway::qryPosition()
{
	writeLog("IB接口持仓信息提供主推更新，无需查询");
}

void IBGateway::close()
{
	api->eDisconnect();
}

void IBGateway::writeLog(std::string logInfo)
{
	LogData log;
	log.gateWayName = gatewayName;
	log.logContent = logInfo;
	onLog(log);
}

std::string IBGateway::readFile(std::string fileName)
{
	std::ifstream is(fileName, std::ifstream::binary);
	std::string ret("");

	if (is)
	{
		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		char * buffer = new char[length + 1];
		memset(buffer, 0, length + 1);

		// read data as a block:
		is.read(buffer, length);

		if (is)
		{
			ret = std::string(buffer);
		}

		is.close();

		// ...buffer contains the entire file...
		delete[] buffer;
	}
	return ret;
}

}//cktrader