#include "IBApi.h"
#include "IbWrapper.h"
#include "IBGateway.h"

#include "Order.h"
#include "Execution.h"

#include <functional>
#include <ctime>

namespace cktrader{

IBApi::IBApi(IBGateway *gateWay)
{
	m_gate = gateWay;
	this->signal = EReaderOSSignal(2000);
	this->wrapper = new IBWrapper(this);
	this->client = new EClientSocket(this->wrapper, &this->signal);
};

IBApi::~IBApi()
{
	delete this->client;
	delete this->wrapper;
};
///-------------------------------------------------------------------------------------
///负责调用processMsgs的线程工作函数
///-------------------------------------------------------------------------------------
void IBApi::run()
{
	while (this->client->isConnected())
	{
		this->reader->checkClient();
		signal.waitForSignal();
		this->reader->processMsgs();
	}
};


///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

bool IBApi::eConnect(std::string host, int port, int clientId, bool extraAuth)
{
	bool r = this->client->eConnect(host.c_str(), port, clientId, extraAuth);

	if (r)
	{
		//启动EReader从socket读取信息
		this->reader = new EReader(this->client, &this->signal);
		this->reader->start();

		//启动数据推送线程
		auto f = std::bind(&IBApi::run, this);
		this->worker = new std::thread(f);
	};

	return r;
};

void IBApi::eDisconnect()
{
	this->client->eDisconnect();
};

std::string IBApi::TwsConnectionTime()
{
	return this->client->TwsConnectionTime();
};

void IBApi::reqMktData(TickerId id, const Contract& contract, const std::string& genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions)
{
	this->client->reqMktData(id, contract, genericTicks, snapshot, mktDataOptions);
};

void IBApi::cancelMktData(TickerId id)
{
	this->client->cancelMktData(id);
};

void IBApi::placeOrder(OrderId id, const Contract& contract, const Order& order)
{
	this->client->placeOrder(id, contract, order);
};

void IBApi::cancelOrder(OrderId id)
{
	this->client->cancelOrder(id);
};

void IBApi::reqOpenOrders()
{
	this->client->reqOpenOrders();
};

void IBApi::reqAccountUpdates(bool subscribe, const std::string& acctCode)
{
	this->client->reqAccountUpdates(subscribe, acctCode);
};

void IBApi::reqExecutions(int reqId, const ExecutionFilter& filter)
{
	this->client->reqExecutions(reqId, filter);
};

void IBApi::reqIds(int numIds)
{
	this->client->reqIds(numIds);
};

void IBApi::reqContractDetails(int reqId, const Contract& contract)
{
	this->client->reqContractDetails(reqId, contract);
};

void IBApi::reqMktDepth(TickerId tickerId, const Contract& contract, int numRows, const TagValueListSPtr& mktDepthOptions)
{
	this->client->reqMktDepth(tickerId, contract, numRows, mktDepthOptions);
};

void IBApi::cancelMktDepth(TickerId tickerId)
{
	this->client->cancelMktDepth(tickerId);
};

void IBApi::reqNewsBulletins(bool allMsgs)
{
	this->client->reqNewsBulletins(allMsgs);
};

void IBApi::cancelNewsBulletins()
{
	this->client->cancelNewsBulletins();
};

void IBApi::setServerLogLevel(int level)
{
	this->client->setServerLogLevel(level);
};

void IBApi::reqAutoOpenOrders(bool bAutoBind)
{
	this->client->reqAutoOpenOrders(bAutoBind);
};

void IBApi::reqAllOpenOrders()
{
	this->client->reqAllOpenOrders();
};

void IBApi::reqManagedAccts()
{
	this->client->reqManagedAccts();
};

void IBApi::requestFA(faDataType pFaDataType)
{
	this->client->requestFA(pFaDataType);
};

void IBApi::replaceFA(faDataType pFaDataType, const std::string& cxml)
{
	this->client->replaceFA(pFaDataType, cxml);
};

void IBApi::reqHistoricalData(TickerId id, const Contract& contract, const std::string& endDateTime, const std::string& durationStr, const std::string&  barSizeSetting, const std::string& whatToShow, int useRTH, int formatDate, const TagValueListSPtr& chartOptions)
{
	this->client->reqHistoricalData(id, contract, endDateTime, durationStr, barSizeSetting, whatToShow, useRTH, formatDate, chartOptions);
};

void IBApi::exerciseOptions(TickerId tickerId, const Contract& contract, int exerciseAction, int exerciseQuantity, const std::string& account, int override)
{
	this->client->exerciseOptions(tickerId, contract, exerciseAction, exerciseQuantity, account, override);
};

void IBApi::cancelHistoricalData(TickerId tickerId)
{
	this->client->cancelHistoricalData(tickerId);
};

void IBApi::reqRealTimeBars(TickerId id, const Contract& contract, int barSize, const std::string& whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions)
{
	this->client->reqRealTimeBars(id, contract, barSize, whatToShow, useRTH, realTimeBarsOptions);
};

void IBApi::cancelRealTimeBars(TickerId tickerId)
{
	this->client->cancelRealTimeBars(tickerId);
};

void IBApi::cancelScannerSubscription(int tickerId)
{
	this->client->cancelScannerSubscription(tickerId);
};

void IBApi::reqScannerParameters()
{
	this->client->reqScannerParameters();
};

void IBApi::reqScannerSubscription(int tickerId, const ScannerSubscription& subscription, const TagValueListSPtr& scannerSubscriptionOptions)
{
	this->client->reqScannerSubscription(tickerId, subscription, scannerSubscriptionOptions);
};

void IBApi::reqCurrentTime()
{
	this->client->reqCurrentTime();
};

void IBApi::reqFundamentalData(TickerId reqId, const Contract& contract, const std::string& reportType)
{
	this->client->reqFundamentalData(reqId, contract, reportType);
};

void IBApi::cancelFundamentalData(TickerId reqId)
{
	this->client->cancelFundamentalData(reqId);
};

void IBApi::calculateImpliedVolatility(TickerId reqId, const Contract& contract, double optionPrice, double underPrice)
{
	this->client->calculateImpliedVolatility(reqId, contract, optionPrice, underPrice);
};

void IBApi::calculateOptionPrice(TickerId reqId, const Contract& contract, double volatility, double underPrice)
{
	this->client->calculateOptionPrice(reqId, contract, volatility, underPrice);
};

void IBApi::cancelCalculateImpliedVolatility(TickerId reqId)
{
	this->client->cancelCalculateImpliedVolatility(reqId);
};

void IBApi::cancelCalculateOptionPrice(TickerId reqId)
{
	this->client->cancelCalculateOptionPrice(reqId);
};

void IBApi::reqGlobalCancel()
{
	this->client->reqGlobalCancel();
};

void IBApi::reqMarketDataType(int marketDataType)
{
	this->client->reqMarketDataType(marketDataType);
};

void IBApi::reqPositions()
{
	this->client->reqPositions();
};

void IBApi::cancelPositions()
{
	this->client->cancelPositions();
};

void IBApi::reqAccountSummary(int reqId, const std::string& groupName, const std::string& tags)
{
	this->client->reqAccountSummary(reqId, groupName, tags);
};

void IBApi::cancelAccountSummary(int reqId)
{
	this->client->cancelAccountSummary(reqId);
};

void IBApi::verifyRequest(const std::string& apiName, const std::string& apiVersion)
{
	this->client->verifyRequest(apiName, apiVersion);
};

void IBApi::verifyMessage(const std::string& apiData)
{
	this->client->verifyMessage(apiData);
};

void IBApi::verifyAndAuthRequest(const std::string& apiName, const std::string& apiVersion, const std::string& opaqueIsvKey)
{
	this->client->verifyAndAuthRequest(apiName, apiVersion, opaqueIsvKey);
};

void IBApi::verifyAndAuthMessage(const std::string& apiData, const std::string& xyzResponse)
{
	this->client->verifyAndAuthMessage(apiData, xyzResponse);
};

void IBApi::queryDisplayGroups(int reqId)
{
	this->client->queryDisplayGroups(reqId);
};

void IBApi::subscribeToGroupEvents(int reqId, int groupId)
{
	this->client->subscribeToGroupEvents(reqId, groupId);
};

void IBApi::updateDisplayGroup(int reqId, const std::string& contractInfo)
{
	this->client->updateDisplayGroup(reqId, contractInfo);
};

void IBApi::unsubscribeFromGroupEvents(int reqId)
{
	this->client->unsubscribeFromGroupEvents(reqId);
};

void IBApi::reqPositionsMulti(int reqId, const std::string& account, const std::string& modelCode)
{
	this->client->reqPositionsMulti(reqId, account, modelCode);
};

void IBApi::cancelPositionsMulti(int reqId)
{
	this->client->cancelPositionsMulti(reqId);
};

void IBApi::reqAccountUpdatessMulti(int reqId, const std::string& account, const std::string& modelCode, bool ledgerAndNLV)
{
	this->client->reqAccountUpdatessMulti(reqId, account, modelCode, ledgerAndNLV);
};

void IBApi::cancelAccountUpdatesMulti(int reqId)
{
	this->client->cancelAccountUpdatesMulti(reqId);
};

void IBApi::reqSecDefOptParams(int reqId, const std::string& underlyingSymbol, const std::string& futFopExchange, const std::string& underlyingSecType, int underlyingConId)
{
	this->client->reqSecDefOptParams(reqId, underlyingSymbol, futFopExchange, underlyingSecType, underlyingConId);
};

void IBApi::reqSoftDollarTiers(int reqId)
{
	this->client->reqSoftDollarTiers(reqId);
};

void IBApi::nextValidId(OrderId orderId)
{
	m_gate->orderId = orderId;
};

 void IBApi::currentTime(long time)
{
	 time_t rawtime = time;
	 char buffer[40];
	 struct tm * timeinfo = localtime(&rawtime);
	 strftime(buffer,40, "%Y-%m-%d %H:%M:%S.%f", timeinfo);

	 m_gate->connected = true;
	 apiStatus = true;	 

	 m_gate->writeLog("IB接口连接成功，当前服务器时间" + std::string(buffer));

	 for (auto item : m_gate->subscribeReqDict)
	 {
		 m_gate->subscribe(item.second);

		 m_gate->subscribeReqDict.erase(item.second.symbol);
	 }
};

 void IBApi::connectAck()
{
	
};

 void IBApi::error(const int id, const int errorCode, const std::string errorString)
{
	ErrorData err;
	err.gateWayName = m_gate->getName();
	err.errorID = errorCode;
	err.errorMsg = errorString;
	m_gate->onError(err);
};

 void IBApi::accountSummary(int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency)
{
	
};

 void IBApi::accountSummaryEnd(int reqId)
{
	
};

 void IBApi::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
 {
	 TickData tick = m_gate->tickDict[tickerId];
	 //需要添加其他字段
	 switch (field)
	 {
	 case 1:
		 tick.bidPrice1 = price;
		 break;

	 case 2:
		 tick.askPrice1 = price;
		 break;

	 case 4:
		 tick.lastPrice = price;
		 break;

	 case 6:
		 tick.highPrice = price;
		 break;

	 case 7:
		 tick.lowPrice = price;
		 break;

	 case 9:
		 tick.preClosePrice = price;
		 break;

	 case 14:
		 tick.openPrice = price;
		 break;
	 }
	 if (m_gate->tickProductDict[tickerId] == PRODUCT_FOREX)
	 {
		 tick.lastPrice = (tick.bidPrice1 + tick.askPrice1) / 2;
	 }
			 
	 time_t rawtime;
	 struct tm * timeinfo;

	 time(&rawtime);
	 timeinfo = localtime(&rawtime);

	 char buffer[40];
	 memset(buffer, 0, 40);
	 strftime(buffer, 40, "%H:%M:%S.%f", timeinfo);
	 tick.time = std::string(buffer);

	 memset(buffer, 0, 40);
	 strftime(buffer, 40, "%Y%m%d", timeinfo);
	 tick.date = std::string(buffer);

	//行情数据更新
	 m_gate->onTick(tick);
	 m_gate->tickDict[tickerId] = tick;
};

 void IBApi::tickSize(TickerId tickerId, TickType field, int size)
{
	 TickData tick = m_gate->tickDict[tickerId];
	 //需要添加其他字段
	 switch (field)
	 {
	 case 0:
		 tick.bidVolume1 = size;
		 break;

	 case 3:
		 tick.askVolume1 = size;
		 break;

	 case 5:
		 tick.lastVolume = size;
		 break;

	 case 8:
		 tick.volume = size;
		 break;

	 case 22:
		 tick.openInterest = size;
		 break;
	 }

	 time_t rawtime;
	 struct tm * timeinfo;

	 time(&rawtime);
	 timeinfo = localtime(&rawtime);

	 char buffer[40];
	 memset(buffer, 0, 40);
	 strftime(buffer, 40, "%H:%M:%S.%f", timeinfo);
	 tick.time = std::string(buffer);

	 memset(buffer, 0, 40);
	 strftime(buffer, 40, "%Y%m%d", timeinfo);
	 tick.date = std::string(buffer);

	 //行情数据更新
	 m_gate->onTick(tick);
	 m_gate->tickDict[tickerId] = tick;
};

 void IBApi::tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta, double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice)
{
	
};

 void IBApi::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
	
};

 void IBApi::tickString(TickerId tickerId, TickType tickType, const std::string& value)
{
	
};

 void IBApi::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints, double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate)
{
	
};

 void IBApi::orderStatus(OrderId orderId, const std::string& status, double filled, double remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string& whyHeld)
{
	 OrderData od;
	 std::map<long, OrderData>::iterator it = m_gate->orderDict.find(orderId);
	 if (it != m_gate->orderDict.end())
	 {
		 od = m_gate->orderDict[orderId];
	 }
	 else
	 {
		od.orderID = std::to_string(orderId);
		od.gateWayName = m_gate->getName();
		od.tOrderID = od.gateWayName +"."+std::to_string(orderId);
	 }
	
	 if (status == "Submitted")
	 {
		 od.status = STATUS_NOTTRADED;
	 }
	 else if(status == "Filled")
	 {
		 od.status = STATUS_ALLTRADED;
	 }
	 else if(status == "Cancelled")
	 {
		 od.status = STATUS_CANCELLED;
	 }
	 else
	 {
		 od.status = STATUS_UNKNOWN;
	 }

	 od.tradedVolume = filled;

	 m_gate->onOrder(od);
	 m_gate->orderDict[orderId] = od;
};

 void IBApi::openOrder(OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState)
{
	 OrderData od;
	 std::map<long, OrderData>::iterator it = m_gate->orderDict.find(orderId);
	 if (it != m_gate->orderDict.end())
	 {
		 od = m_gate->orderDict[orderId];
	 }
	 else
	 {
		 od.orderID = std::to_string(orderId);
		 od.gateWayName = m_gate->getName();
		 od.tOrderID = od.gateWayName + "." + std::to_string(orderId);
		 od.symbol = contract.localSymbol;
		 od.exchange = contract.exchange;
		 od.tSymbol = od.symbol + od.exchange;
	 }

	 if (order.action == "BOT")
	 {
		 od.direction = DIRECTION_LONG;
	 }
	 else 
	 {
		 od.direction = DIRECTION_SHORT;
	 }

	 od.price = order.lmtPrice;
	 od.totalVolume = order.totalQuantity;

	 m_gate->onOrder(od);
	 m_gate->orderDict[orderId] = od;
};

 void IBApi::openOrderEnd()
{
	
};

 void IBApi::winError(const std::string& str, int lastError)
{
	
};

 void IBApi::connectionClosed()
{
	 apiStatus = false;
	 m_gate->connected = false;

	 m_gate->writeLog("IB接口连接断开");
};

 void IBApi::updateAccountValue(const std::string& key, const std::string& val, const std::string& currency, const std::string& accountName)
{
	// 仅逐个字段更新数据，这里对于没有currency的推送忽略
	 if (!currency.empty())
	 {
		 std::string name = accountName + "." + currency;

		 std::map<std::string, AccountData >::iterator it = m_gate->accountDict.find(name);

		 AccountData acc;
		 if (it != m_gate->accountDict.end())
		 {
			 acc = m_gate->accountDict[name];
		 }
		 else
		 {
			 acc.accountID = accountName;
			 acc.tAccountID = name;
			 acc.gateWayName = m_gate->getName();
		 }

		 if (key == "NetLiquidationByCurrency")
		 {
			 acc.balance = atof(val.c_str());
		 }
		 else if (key == "NetLiquidation")
		 {
			 acc.balance = atof(val.c_str());
		 }
		 else if (key == "UnrealizedPnL")
		 {
			 acc.positionProfit = atof(val.c_str());
		 }
		 else if (key == "AvailableFunds")
		 {
			 acc.available = atof(val.c_str());
		 }
		 else if (key == "AvailableFunds")
		 {
			 acc.margin = atof(val.c_str());
		 }

		 m_gate->accountDict[name] = acc;
	 }
};

 void IBApi::updatePortfolio(const Contract& contract, double position, double marketPrice, double marketValue, double averageCost, double unrealizedPNL, double realizedPNL, const std::string& accountName)
{
	 PositionData pos;
	 pos.symbol = contract.localSymbol;
	 pos.exchange = contract.exchange;
	 pos.tSymbol = pos.symbol+"."+pos.exchange;
	 pos.direction = DIRECTION_NET;
	 pos.position = position;
	 pos.price = averageCost;
	 pos.tPositionName = pos.tSymbol;
	 pos.gateWayName = m_gate->getName();

	m_gate->onPosition(pos);
};

 void IBApi::updateAccountTime(const std::string& timeStamp)
{
	 for (auto item : m_gate->accountDict)
	 {
		 m_gate->onAccount(item.second);
	 }
};

 void IBApi::accountDownloadEnd(const std::string& accountName)
{

};

 void IBApi::contractDetails(int reqId, const ContractDetails& contractDetails)
{
	 std::string symbol = contractDetails.summary.localSymbol;
	 std::string exchange = contractDetails.summary.exchange;
	 std::string tSymbol = symbol+"."+exchange;

	 std::map<std::string, ContractData>::iterator it = m_gate->contractDict.find(tSymbol);
	
	 ContractData cd;
	 if (it != m_gate->contractDict.end())
	 {
		 cd = m_gate->contractDict[tSymbol];

		 cd.name = contractDetails.longName;
		 cd.priceTick = contractDetails.minTick;

		 m_gate->onContract(cd);
		 m_gate->contractDict[tSymbol] = cd;
	 }
};

 void IBApi::bondContractDetails(int reqId, const ContractDetails& contractDetails)
{
	
};

 void IBApi::contractDetailsEnd(int reqId)
{
	
};

 void IBApi::execDetails(int reqId, const Contract& contract, const Execution& execution)
{
	 TradeData trade;
	 trade.gateWayName = m_gate->getName();
	 trade.tradeID = execution.execId;
	 trade.tTradeID = trade.gateWayName + "." + trade.tradeID;

	 trade.symbol = contract.localSymbol;
	 trade.exchange = contract.exchange;
	 trade.tSymbol = trade.symbol + "." + trade.exchange;

	 trade.orderID = std::to_string(execution.orderId);
	 if (execution.side == "BOT")
	 {
		 trade.direction = DIRECTION_LONG;
	 }
	 else
	 {
		 trade.direction = DIRECTION_SHORT;
	 }
	 trade.price = execution.price;
	 trade.volume = execution.shares;
	 trade.tradeTime = execution.time;

	 m_gate->onTrade(trade);
};

 void IBApi::execDetailsEnd(int reqId)
{

};

 void IBApi::updateMktDepth(TickerId id, int position, int operation, int side, double price, int size)
{

};

 void IBApi::updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation, int side, double price, int size)
{

};

 void IBApi::updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch)
{

};

 void IBApi::managedAccounts(const std::string& accountsList)
{

};

 void IBApi::receiveFA(faDataType pFaDataType, const std::string& cxml)
{

};

 void IBApi::historicalData(TickerId reqId, const std::string& date, double open, double high, double low, double close, int volume, int barCount, double WAP, int hasGaps)
{

};

 void IBApi::scannerParameters(const std::string& xml)
{

};

 void IBApi::scannerData(int reqId, int rank, const ContractDetails& contractDetails, const std::string& distance, const std::string& benchmark, const std::string& projection, const std::string& legsStr)
{

};

 void IBApi::scannerDataEnd(int reqId)
{
	
};

 void IBApi::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close, long volume, double wap, int count)
{

};

 void IBApi::fundamentalData(TickerId reqId, const std::string& data)
{

};

 void IBApi::deltaNeutralValidation(int reqId, const UnderComp& underComp)
{

};

 void IBApi::tickSnapshotEnd(int reqId)
{

};

 void IBApi::marketDataType(TickerId reqId, int marketDataType)
{

};

 void IBApi::commissionReport(const CommissionReport& commissionReport)
{

};

 void IBApi::position(const std::string& account, const Contract& contract, double position, double avgCost)
{

};

 void IBApi::positionEnd()
{

};

 void IBApi::verifyMessageAPI(const std::string& apiData)
{

};

 void IBApi::verifyCompleted(bool isSuccessful, const std::string& errorText)
{

};

 void IBApi::displayGroupList(int reqId, const std::string& groups)
{

};

 void IBApi::displayGroupUpdated(int reqId, const std::string& contractInfo)
{

};

 void IBApi::verifyAndAuthMessageAPI(const std::string& apiData, const std::string& xyzChallange)
{

};

 void IBApi::verifyAndAuthCompleted(bool isSuccessful, const std::string& errorText)
{

};

 void IBApi::positionMulti(int reqId, const std::string& account, const std::string& modelCode, const Contract& contract, double pos, double avgCost)
{

};

 void IBApi::positionMultiEnd(int reqId)
{

};

 void IBApi::accountUpdateMulti(int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency)
{

};

 void IBApi::accountUpdateMultiEnd(int reqId)
{

};

// void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes)
void IBApi::securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string&tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes)
{

};

 void IBApi::securityDefinitionOptionalParameterEnd(int reqId)
{

};


 void IBApi::softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers)
{

};

}//cktrader
