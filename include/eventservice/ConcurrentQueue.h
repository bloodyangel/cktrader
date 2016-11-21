#pragma once
#ifndef __CONCURRENTQUEUE_H__
#define __CONCURRENTQUEUE_H__

#include <mutex>
#include <queue>
#include <condition_variable>

#include "utils/ckdef.h"
#include "utils/cktypes.h"

namespace cktrader{
///线程安全的队列
class ConcurrentQueue
{
private:
	//三个优先级队列
	std::queue<Task> the_queue_priority_high;
	std::queue<Task> the_queue_priority_middle;
	std::queue<Task> the_queue_priority_low;

	mutable std::recursive_mutex the_mutex;							//互斥锁
	mutable std::condition_variable_any the_condition_variable;			//条件变量

public:
	//存入新的任务
	void push(Task const& data)
	{
		std::unique_lock<std::recursive_mutex> lck(the_mutex);				//获取互斥锁

		//向队列中存入数据
		if (data.task_priority == Task::high)
		{
			the_queue_priority_high.push(data);
		}
		else if (data.task_priority == Task::middle)
		{
			the_queue_priority_middle.push(data);
		}
		else if (data.task_priority == Task::low)
		{
			the_queue_priority_low.push(data);
		}							

		the_condition_variable.notify_one();			//通知正在阻塞等待的线程
	}

	//检查队列是否为空
	bool empty() const
	{
		std::unique_lock<std::recursive_mutex> lck(the_mutex);
		bool isEmpty = false;

		if (the_queue_priority_high.empty() &&
			the_queue_priority_middle.empty() &&
			the_queue_priority_low.empty())
		{
			isEmpty = true;
		}

		return isEmpty;		
	}

	//取出
	Task wait_and_pop()
	{
		std::unique_lock<std::recursive_mutex> lck(the_mutex);

		while (the_queue_priority_high.empty() &&
				the_queue_priority_middle.empty() &&
				the_queue_priority_low.empty())						//当队列为空时
		{
			the_condition_variable.wait(lck);			//等待条件变量通知
		}

		Task popped_value;

		if (!the_queue_priority_high.empty())
		{
			popped_value = the_queue_priority_high.front();			//获取队列中的最后一个任务
			the_queue_priority_high.pop();
			return popped_value;
		}

		if (!the_queue_priority_middle.empty())
		{
			popped_value = the_queue_priority_middle.front();			//获取队列中的最后一个任务
			the_queue_priority_middle.pop();
			return popped_value;
		}

		if (!the_queue_priority_low.empty())
		{
			popped_value = the_queue_priority_low.front();			//获取队列中的最后一个任务
			the_queue_priority_low.pop();
			return popped_value;
		}
	}
};

}//cktrader

#endif