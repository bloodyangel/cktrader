#include "eventservice/eventengine.h"

#include <memory>
#include <chrono>
#include <algorithm>

namespace cktrader {

EventEngine::EventEngine():the_mutex_handlers(), m_active(false)
{
	m_task_thread_pool = nullptr;

	task_pool = new TaskContainer;//�¼��������
	
	handlers = new std::multimap<std::string, Handle>;

	m_timer_thread = nullptr;;//for timer
}

EventEngine::EventEngine(EventEngine& engine):the_mutex_handlers(), m_active(false)
{
	m_task_thread_pool = engine.m_task_thread_pool;

	task_pool = engine.task_pool;//�¼��������

	handlers = engine.handlers;

	m_timer_thread = engine.m_timer_thread;//for timer
}

EventEngine::~EventEngine()
{
	m_active = false;

	//ɾ���������̳߳�
	if (m_task_thread_pool)
	{		
		for (std::vector<std::thread*>::iterator it = m_task_thread_pool->begin(); it != m_task_thread_pool->end(); it++)
		{
			(*it)->join();
			delete (*it);
			m_task_thread_pool->erase(it);
		}

		delete m_task_thread_pool;
		m_task_thread_pool = nullptr;
	}

	if (m_timer_thread)
	{
		m_timer_thread->join();
		delete m_timer_thread;
		m_timer_thread = nullptr;
	}	

	if (task_pool)
	{
		delete task_pool;
		task_pool = nullptr;
	}
	
	if (handlers)
	{
		delete handlers;
		handlers = nullptr;
	}	
}

//ע���¼���������typeΪ�¼����ͣ�accepter��ע���߱�ʶ��f��ע��Ĵ�����
bool EventEngine::registerHandler(std::string type, Handler f, std::string accepter)
{
	the_mutex_handlers.lock();
	if (!handlers)
	{
		return false;
	}
	Handle h;
	h.fh = f;
	h.handle_register = accepter;

	handlers->insert(std::make_pair(type,h));
	the_mutex_handlers.unlock();
	return true;
}

//ɾ��accepterע���type���ʹ�����
bool EventEngine::unRegisterHandler(std::string type, std::string accepter)
{
	the_mutex_handlers.lock();
	if (!handlers)
	{
		return false;
	}

	std::pair <std::multimap<std::string, Handle>::iterator, std::multimap<std::string, Handle>::iterator> ret;

	ret = handlers->equal_range(type);
	for (std::multimap<std::string, Handle>::iterator it = ret.first; it != ret.second; ++it)
	{
		if (it->second.handle_register == accepter)
		{
			handlers->erase(it);
		}
	}
	
	the_mutex_handlers.unlock();
	return true;
}

//����task
bool EventEngine::put(Task &t)
{
	if (!task_pool)
	{
		return false;
	}

	task_pool->put_task(t);
	return true;
}

//�������棬�򵥵��̳߳أ������̣߳�������һ��timer�̣߳����ڲ���ʱ���ź�
bool EventEngine::startEngine()
{
	if (m_active || m_task_thread_pool || m_timer_thread)
	{
		return false;
	}

	m_active = true;

	m_task_thread_pool = new std::vector<std::thread*>;

	std::function<void()> f = std::bind(&EventEngine::processTask, this);

	for (unsigned i = 0; i < 2*std::thread::hardware_concurrency(); i++)
	{
		std::thread* thread_worker = new std::thread(f);
		m_task_thread_pool->push_back(thread_worker);
	}

	std::function<void()> f_timer = std::bind(&EventEngine::trigerTimer, this);
	m_timer_thread = new std::thread(f_timer);

	return true;
}

//ֹͣ�¼�����
bool EventEngine::stopEngine()
{
	if (!m_active || !m_task_thread_pool || !m_timer_thread)
	{
		return false;
	}

	m_active = false;

	if (m_timer_thread)
	{
		m_timer_thread->join();
		delete m_timer_thread;
		m_timer_thread = nullptr;
	}

	if (m_task_thread_pool)
	{
		for (std::vector<std::thread*>::iterator it = m_task_thread_pool->begin(); it != m_task_thread_pool->end(); it++)
		{
			(*it)->join();
			delete (*it);
			m_task_thread_pool->erase(it);
		}

		delete m_task_thread_pool;
		m_task_thread_pool = nullptr;
	}

	return true;
}

//�¼�����Ĵ����������ڶ�ȡtask���������¼�������
void EventEngine::processTask()
{
	while (m_active)
	{
		Task task;

		task = task_pool->wait_and_get_task();

		if (task.handle_flag)
		{
			FuncData func;

			try
			{
				func = task.task_data.cast<FuncData>();
				func.h(func.arg);
			}
			catch (...)
			{

			}

		}else
		{
			//�ҵ��¼���Ӧ�Ĵ�����,���߳�ע���Ƿ������⣿
			std::pair <std::multimap<std::string, Handle>::iterator, std::multimap<std::string, Handle>::iterator> ret;
			the_mutex_handlers.lock();
			ret = handlers->equal_range(task.type);
			the_mutex_handlers.unlock();

			//�ַ����񣬲������ȼ��ߵĶ��У��¼����͵����ɾ���ɢ����������ֹ�̵߳ȴ�
			//�¼�����һ��Ϊ���¼�����+task����+������
			for (std::multimap<std::string, Handle>::iterator it = ret.first; it != ret.second; ++it)
			{
				FuncData func;
				func.h = it->second.fh;
				func.arg = task.task_data;

				//���ɺ��������¼�������ͬһ���¼������й�����������ʱ�򣬿��Կ��ٱ�����
				Task handlerTask;
				handlerTask.task_priority = Task::high;
				handlerTask.type = task.type+it->second.handle_register;
				handlerTask.handle_flag = true;
				handlerTask.task_data = func;
				put(handlerTask);
			}//for			
		}//else

		//֪ͨ��һ���߳̿��Դ����¼��ˣ�������֤һ���¼�������ͬһʱ��ֻ�ܴ���һ����Լ
		task_pool->task_run_end(task.type);
		
	}//while
}

//timer�̣߳�����ʱ���ź�
void EventEngine::trigerTimer()
{
	while (m_active)
	{
		Task task = Task();
		task.type = EVENT_TIMER;
		task.task_data = Datablk();
		put(task);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

}//cktrader