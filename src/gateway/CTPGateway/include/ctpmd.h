#pragma once
#ifndef __CTPMD_H__
#define __CTPMD_H__

#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"
#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "ctpdefs.h"

#include <set>
#include <string>
#include <mutex>
#include <atomic>

namespace cktrader {

	class EventEngine;
	class CTPGateWay;

	class CtpMd :public CThostFtdcMdSpi
	{
	public:
		CtpMd(EventEngine *event_service, CTPGateWay *gateWay);
		~CtpMd();

		///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
		virtual void OnFrontConnected();

		///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
		///@param nReason ����ԭ��
		///        0x1001 �����ʧ��
		///        0x1002 ����дʧ��
		///        0x2001 ����������ʱ
		///        0x2002 ��������ʧ��
		///        0x2003 �յ�������
		virtual void OnFrontDisconnected(int nReason);

		///��¼������Ӧ
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�ǳ�������Ӧ
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///����Ӧ��
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�������֪ͨ
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

		///�¼�������

		void processFrontConnected(Datablk& data);

		void processFrontDisconnected(Datablk& data);

		void processRspUserLogin(Datablk& data);

		void processRspUserLogout(Datablk& data);

		void processRspError(Datablk& data);

		void processRtnDepthMarketData(Datablk& data);

		///////////////////////////////////////////////////////
		/// �������ú���
		///
		void connect(std::string userID, std::string password, std::string brokerID, std::string mdAddress);

		void subscribe(SubscribeReq& subscribeReq);

		int close();

		std::string getTradingDay();

		int subscribeMarketData(std::string instrumentID);

		int unSubscribeMarketData(std::string instrumentID);

		int subscribeForQuoteRsp(std::string instrumentID);

		int unSubscribeForQuoteRsp(std::string instrumentID);

	private:
		EventEngine *m_event_service;
		CTPGateWay *gateWay;
		CThostFtdcMdApi* api;

		std::string gateWayName;
		std::atomic<int> reqID = 0; //����id

		std::atomic<bool> connectionStatus = false;      // ����״̬
		std::atomic<bool> loginStatus = false;//            ��¼״̬

		std::set<std::string> *subscribedSymbols = nullptr;//    �Ѷ��ĺ�Լ����

		std::string userID;// �˺�
		std::string password;//����
		std::string brokerID;//        # �����̴���
		std::string address;//        # ��������ַ

		mutable std::recursive_mutex the_mutex;
	};

}//cktrader

#endif // CTPMD_H