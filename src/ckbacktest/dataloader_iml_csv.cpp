#include "backtest/dataloader_iml_csv.h"
#include "utils/strptime.h"

#include <time.h>
#include <thread>

namespace cktrader {

DataLoaderCsv::DataLoaderCsv(IGateway* gate)
{
	backtestGate = gate;
}

DataLoaderCsv::~DataLoaderCsv()
{

}

void DataLoaderCsv::setStartDate(std::string startDate)
{
	this->startDate = startDate;
}

void DataLoaderCsv::setEndDate(std::string endDate)
{
	this->endDate = endDate;
}

void DataLoaderCsv::setPath(std::string path)
{
	this->path = path;
}

void DataLoaderCsv::startLoadData()
{
	if (!dataPull_thread)
	{
		return;
	}

	dataPull_thread = new std::thread(&DataLoaderCsv::pullTick);
}

bool DataLoaderCsv::stopLoadData()
{

}

void DataLoaderCsv::pullTick()
{

}

}//cktrader