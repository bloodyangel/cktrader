#pragma once
#ifndef __IBAPI_H__
#define __IBAPI_H__

#include "ibdef.h"

#include "EWrapper.h"
#include "EClientSocket.h"
#include "EReader.h"


#include <string>
#include <thread>
#include <atomic>

namespace cktrader {

class IBWrapper;
class IBGateway;
	
class IBApi
{
private:
	//EClientSocket *client;
	IBWrapper *wrapper;
	EReaderOSSignal signal;
	EReader *reader;

	IBGateway* m_gate;

	std::thread *worker;

	std::atomic<bool> apiStatus =  false;

public:
	EClientSocket *client;

	IBApi(IBGateway *gateWay);
	~IBApi();

	//-------------------------------------------------------------------------------------
	//负责调用checkMessages的线程工作函数
	//-------------------------------------------------------------------------------------
	void run();

	//-------------------------------------------------------------------------------------
	//回调函数
	//-------------------------------------------------------------------------------------

	 void nextValidId(OrderId orderId);

	 void currentTime(long time);

	 void connectAck();

	 void error(const int id, const int errorCode, const std::string errorString);

	 void accountSummary(int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string&
		curency);

	 void accountSummaryEnd(int reqId);

	 void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute);

	 void tickSize(TickerId tickerId, TickType field, int size);

	 void tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta,
		double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice);

	 void tickGeneric(TickerId tickerId, TickType tickType, double value) ;

	 void tickString(TickerId tickerId, TickType tickType, const std::string& value);

	 void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints,
		double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate);

	 void orderStatus(OrderId orderId, const std::string& status, double filled,
		double remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const std::string& whyHeld) ;

	 void openOrder(OrderId orderId, const Contract& contract, const Order& order, const OrderState& orderState);

	 void openOrderEnd();

	 void winError(const std::string& str, int lastError);

	 void connectionClosed();

	 void updateAccountValue(const std::string& key, const std::string& val,
		const std::string& currency, const std::string& accountName);

	 void updatePortfolio(const Contract& contract, double position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const std::string& accountName);

	 void updateAccountTime(const std::string& timeStamp);

	 void accountDownloadEnd(const std::string& accountName);

	 void contractDetails(int reqId, const ContractDetails& contractDetails);

	 void bondContractDetails(int reqId, const ContractDetails& contractDetails);

	 void contractDetailsEnd(int reqId);

	 void execDetails(int reqId, const Contract& contract, const Execution& execution);

	 void execDetailsEnd(int reqId);

	 void updateMktDepth(TickerId id, int position, int operation, int side,
		double price, int size);

	 void updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
		int side, double price, int size) ;

	 void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch);

	 void managedAccounts(const std::string& accountsList);

	 void receiveFA(faDataType pFaDataType, const std::string& cxml) ;

	 void historicalData(TickerId reqId, const std::string& date, double open, double high,
		double low, double close, int volume, int barCount, double WAP, int hasGaps) ;

	 void scannerParameters(const std::string& xml);

	 void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
		const std::string& distance, const std::string& benchmark, const std::string& projection,
		const std::string& legsStr);

	 void scannerDataEnd(int reqId);

	 void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
		long volume, double wap, int count);

	 void fundamentalData(TickerId reqId, const std::string& data);

	 void deltaNeutralValidation(int reqId, const UnderComp& underComp);

	 void tickSnapshotEnd(int reqId);

	 void marketDataType(TickerId reqId, int marketDataType);

	 void commissionReport(const CommissionReport& commissionReport) ;

	 void position(const std::string& account, const Contract& contract, double position, double avgCost);

	 void positionEnd();

	 void verifyMessageAPI(const std::string& apiData) ;

	 void verifyCompleted(bool isSuccessful, const std::string& errorText);

	 void displayGroupList(int reqId, const std::string& groups);

	 void displayGroupUpdated(int reqId, const std::string& contractInfo);

	 void verifyAndAuthMessageAPI(const std::string& apiData, const std::string& xyzChallange);

	 void verifyAndAuthCompleted(bool isSuccessful, const std::string& errorText);

	 void positionMulti(int reqId, const std::string& account, const std::string& modelCode, const Contract& contract, double pos,
		double avgCost);

	 void positionMultiEnd(int reqId);

	 void accountUpdateMulti(int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const
		std::string& value, const std::string& currency);

	 void accountUpdateMultiEnd(int reqId) ;

	// void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string&
	//	tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes){};

	 void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string&
		tradingClass, const std::string& multiplier, std::set<std::string> expirations, std::set<double> strikes);

	 void securityDefinitionOptionalParameterEnd(int reqId);

	 void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers);

	//-------------------------------------------------------------------------------------
	//主动函数
	//-------------------------------------------------------------------------------------

	bool eConnect(std::string host, int port, int clientId, bool extraAuth);

	void eDisconnect();

	std::string TwsConnectionTime();

	void reqMktData(TickerId id, const Contract& contract,
		const std::string& genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions);

	void cancelMktData(TickerId id);

	void placeOrder(OrderId id, const Contract& contract, const Order& order);

	void cancelOrder(OrderId id);

	void reqOpenOrders();

	void reqAccountUpdates(bool subscribe, const std::string& acctCode);

	void reqExecutions(int reqId, const ExecutionFilter& filter);

	void reqIds(int numIds);

	void reqContractDetails(int reqId, const Contract& contract);

	void reqMktDepth(TickerId tickerId, const Contract& contract, int numRows, const TagValueListSPtr& mktDepthOptions);

	void cancelMktDepth(TickerId tickerId);

	void reqNewsBulletins(bool allMsgs);

	void cancelNewsBulletins();

	void setServerLogLevel(int level);

	void reqAutoOpenOrders(bool bAutoBind);

	void reqAllOpenOrders();

	void reqManagedAccts();

	void requestFA(faDataType pFaDataType);

	void replaceFA(faDataType pFaDataType, const std::string& cxml);

	void reqHistoricalData(TickerId id, const Contract& contract,
		const std::string& endDateTime, const std::string& durationStr,
		const std::string&  barSizeSetting, const std::string& whatToShow,
		int useRTH, int formatDate, const TagValueListSPtr& chartOptions);

	void exerciseOptions(TickerId tickerId, const Contract& contract,
		int exerciseAction, int exerciseQuantity,
		const std::string& account, int override);

	void cancelHistoricalData(TickerId tickerId);

	void reqRealTimeBars(TickerId id, const Contract& contract, int barSize,
		const std::string& whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions);

	void cancelRealTimeBars(TickerId tickerId);

	void cancelScannerSubscription(int tickerId);

	void reqScannerParameters();

	void reqScannerSubscription(int tickerId, const ScannerSubscription& subscription, const TagValueListSPtr& scannerSubscriptionOptions);

	void reqCurrentTime();

	void reqFundamentalData(TickerId reqId, const Contract&, const std::string& reportType);

	void cancelFundamentalData(TickerId reqId);

	void calculateImpliedVolatility(TickerId reqId, const Contract& contract, double optionPrice, double underPrice);

	void calculateOptionPrice(TickerId reqId, const Contract& contract, double volatility, double underPrice);

	void cancelCalculateImpliedVolatility(TickerId reqId);

	void cancelCalculateOptionPrice(TickerId reqId);

	void reqGlobalCancel();

	void reqMarketDataType(int marketDataType);

	void reqPositions();

	void cancelPositions();

	void reqAccountSummary(int reqId, const std::string& groupName, const std::string& tags);

	void cancelAccountSummary(int reqId);

	void verifyRequest(const std::string& apiName, const std::string& apiVersion);

	void verifyMessage(const std::string& apiData);

	void verifyAndAuthRequest(const std::string& apiName, const std::string& apiVersion, const std::string& opaqueIsvKey);

	void verifyAndAuthMessage(const std::string& apiData, const std::string& xyzResponse);

	void queryDisplayGroups(int reqId);

	void subscribeToGroupEvents(int reqId, int groupId);

	void updateDisplayGroup(int reqId, const std::string& contractInfo);

	void unsubscribeFromGroupEvents(int reqId);

	void reqPositionsMulti(int reqId, const std::string& account, const std::string& modelCode);

	void cancelPositionsMulti(int reqId);

	void reqAccountUpdatessMulti(int reqId, const std::string& account, const std::string& modelCode, bool ledgerAndNLV);

	void cancelAccountUpdatesMulti(int reqId);

	void reqSecDefOptParams(int reqId, const std::string& underlyingSymbol, const std::string& futFopExchange, const std::string&
		underlyingSecType, int underlyingConId);

	void reqSoftDollarTiers(int reqId);
};

}//cktrader

#endif