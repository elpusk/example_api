#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <map>

#include <_safe_cv.h>

template <class T>
class cworker
{
public:
	typedef	std::shared_ptr<cworker<T> >	type_ptr;

public:
	template <class _Fn, class... _Args>
	explicit cworker(_Fn&& _Fx, _Args&&... _Ax) :
		m_ptr_thread( new std::thread(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...))
	{
	}

	virtual ~cworker() 
	{
		if (m_ptr_thread) {
			m_cv.notify_one(true);

			if (m_ptr_thread->joinable())
				m_ptr_thread->join();
		}
	}

	bool joinable()
	{
		return m_ptr_thread->joinable();
	}
	void join()
	{
		m_ptr_thread->join();
	}
	std::thread::id get_id()
	{
		return m_ptr_thread->get_id();
	}
	void detach()
	{
		m_ptr_thread->detach();
	}
	std::thread::native_handle_type native_handle()
	{
		return m_ptr_thread->native_handle();
	}
	static unsigned int hardware_concurrency()
	{
		return std::thread::hardware_concurrency();
	}

	void notify_one(bool fixed_status = false)
	{
		m_cv.notify_one(fixed_status);
	}

	std::cv_status wait_for(const unsigned long dw_mmsec_time_out)
	{
		return m_cv.wait_for(dw_mmsec_time_out);
	}

	bool is_always_notified()
	{
		return m_cv.is_always_notified();
	}

	bool pop_job(T& item)
	{
		bool b_result(false);
		do {
			std::lock_guard<std::mutex> lock(m_mutex_q);
			if (m_q.empty())
				continue;
			item = m_q.front();
			m_q.pop();

			b_result = true;
		} while (false);
		return b_result;
	}

	cworker<T>& push_job(const T& item)
	{
		do {
			std::lock_guard<std::mutex> lock(m_mutex_q);
			m_q.push(item);
			m_cv.notify_one();
		} while (false);

		return *this;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_mutex_q);
		return m_q.empty();
	}

private:
	std::shared_ptr<std::thread> m_ptr_thread;
	_safe_cv m_cv;

	std::mutex m_mutex_q;
	typename std::queue <T> m_q;

private://don't call these method.
	cworker();
	cworker(const cworker&);
	cworker& operator=(const cworker&);
};
