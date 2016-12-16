#pragma once
#ifndef __TASKCONTAINER_H__
#define __TASKCONTAINER_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"

#include "MapItem.h"
#include "fcmm.hpp"
#include "blockingconcurrentqueue.h"

#include <atomic>
#include <string>

using namespace fcmm;

namespace cktrader {

struct strHash
{
	size_t operator()(std::string const& s) const
	{
		register size_t hash = 0;

		size_t len = s.length();

		for (size_t i = 0; i < len; i++)
		{
			hash = hash * 131 + s[i];
		}
		return hash;
	}
};

typedef Fcmm<std::string, MapItem*, std::hash<std::string>, strHash> CKMap;

class TaskContainer
{
private:
	//装任务类型对应的map,key为任务类型
	CKMap *task_map;

	BlockingConcurrentQueue<std::string> *task_type_queue_high;
	BlockingConcurrentQueue<std::string> *task_type_queue_normal;

public:
	TaskContainer();
	~TaskContainer();
	Task wait_and_get_task();
	void put_task(Task& data);

	//当前task处理完成后，释放一个事件只能有一个线程处理的锁
	void task_run_end(std::string task_type);

protected:
	std::string read_task_type_queue();
	void write_task_type_queue(Task& data);

	void write_task_map(Task& data);
	MapItem* read_task_map(std::string type);
};

}//cktrader

#endif