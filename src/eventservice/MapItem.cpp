#include "eventservice/MapItem.h"

namespace cktrader {

MapItem::MapItem()
{
	the_queue = new ConcurrentQueue<Task>;
}

MapItem::~MapItem()
{
	if (!the_queue)
	{
		delete the_queue;
		the_queue = nullptr;
	}
}

void MapItem::write(Task& t)
{
	the_queue->enqueue(t);	
}

bool MapItem::read(Task& t)
{
	//获取执行task的权限
	get_run_lock();	
	bool is_read = the_queue->try_dequeue(t);
			
	return is_read;
}

void MapItem::get_run_lock()
{
	//同一个策略、合约、事件的key，同一时间只能有一个thread处理
	while (unique_thread_flag.test_and_set(std::memory_order_acquire))
	{

	}
}

void MapItem::free_run_lock()
{
	unique_thread_flag.clear(std::memory_order_release);
}

}//cktrader