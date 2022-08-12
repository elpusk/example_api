#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <thread>
#include <map>

#include <_safe_cv.h>
#include <cworker.h>

#include "example_dll_const.h"
#include "cjob_of_dll.h"
#include "cresult.h"

#include "cprocess_of_job.h"

namespace _example
{
	class cjob_mgmt
	{
	public:
		typedef std::shared_ptr<cjob_mgmt>	type_ptr;
	private:
		typedef	cworker<_example::cjob_of_dll::type_ptr_cjob_of_dll>	_type_worker;
		typedef	cworker<_example::cjob_of_dll::type_ptr_cjob_of_dll>::type_ptr	_type_ptr_worker;

	public:
		template <class _Fn, class... _Args>
		explicit cjob_mgmt(_Fn&& _Fx, _Args&&... _Ax) : m_n_worker_number(_example::cdll_const::const_invalid_worker_number)
		{
			map_job_process[_example::cjob_of_dll::req_type_job] = _example::cprocess::type_ptr(new _example::cprocess_of_job());
			m_ptr_woker = _type_ptr_worker(new _type_worker(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...));
		}
		explicit cjob_mgmt(long n_worker_number) : 
			m_n_worker_number(n_worker_number)
		{
			map_job_process[_example::cjob_of_dll::req_type_job] = _example::cprocess::type_ptr(new _example::cprocess_of_job());
			m_ptr_woker = _type_ptr_worker(new _type_worker(cjob_mgmt::_worker, this));
		}

		explicit cjob_mgmt() : m_n_worker_number(_example::cdll_const::const_invalid_worker_number)
		{
			map_job_process[_example::cjob_of_dll::req_type_job] = _example::cprocess::type_ptr(new _example::cprocess_of_job());
			m_ptr_woker = _type_ptr_worker(new _type_worker(cjob_mgmt::_worker, this));
		}

		virtual ~cjob_mgmt()
		{
			m_ptr_woker.reset();
		}

		void set_worker_number(long n_worker_number)
		{
			m_n_worker_number = n_worker_number;
		}

		std::cv_status wait_for(const unsigned long dw_mmsec_time_out)
		{
			return m_ptr_woker->wait_for(dw_mmsec_time_out);
		}
		bool is_always_notified()
		{
			return m_ptr_woker->is_always_notified();
		}

		cdll_const::type_result_index push_job(
			_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_cjob_of_dll
		)
		{
			cdll_const::type_result_index result_index(cdll_const::result_index_error);

			do {
				if (!ptr_cjob_of_dll)
					continue;

				m_ptr_woker->push_job(ptr_cjob_of_dll).notify_one();
				result_index = ptr_cjob_of_dll->get_result_index();
			} while (false);
			return result_index;
		}

		cdll_const::type_result_index push_job(
			const _example::cjob_of_dll::type_requst req_type,
			const unsigned char* ps_data,
			const unsigned long n_data,
			const _example::cdll_const::type_dll_callback cb,
			void* p_user_of_cb,
			long b_cancel_waiting_job
		)
		{
			return push_job(req_type, m_n_worker_number, ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
		}
		cdll_const::type_result_index push_job(
			const _example::cjob_of_dll::type_requst req_type,
			const long n_woker_number,
			const unsigned char* ps_data,
			const unsigned long n_data,
			const _example::cdll_const::type_dll_callback cb,
			void* p_user_of_cb,
			long b_cancel_waiting_job
		)
		{
			cdll_const::type_result_index result_index(cdll_const::result_index_error);

			do {
				_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_cjob_of_dll(
					new _example::cjob_of_dll(
						cjob_mgmt::_get_result_storage().generate_new_result_index(),
						req_type,
						n_woker_number,
						ps_data,
						n_data,
						cb,
						p_user_of_cb,
						b_cancel_waiting_job
					)
				);
				if (!ptr_cjob_of_dll)
					continue;

				m_ptr_woker->push_job(ptr_cjob_of_dll).notify_one();
				result_index = ptr_cjob_of_dll->get_result_index();
			} while (false);
			return result_index;
		}

		cdll_const::type_result_index push_job()
		{
			cdll_const::type_result_index result_index(cdll_const::result_index_error);

			do {
				_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_cjob_of_dll(
					new _example::cjob_of_dll(
						cjob_mgmt::_get_result_storage().generate_new_result_index(),
						m_n_worker_number
					)
				);
				if (!ptr_cjob_of_dll)
					continue;

				m_ptr_woker->push_job(ptr_cjob_of_dll).notify_one();
				result_index = ptr_cjob_of_dll->get_result_index();
			} while (false);
			return result_index;
		}

		bool pop_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_job_out)
		{
			bool b_result(false);
			do {
				if (m_ptr_woker->empty())
					continue;
				b_result = m_ptr_woker->pop_job(ptr_job_out);
			} while (false);
			return b_result;
		}

		static cdll_const::type_result_index generate_new_result_index()
		{
			return cjob_mgmt::_get_result_storage().generate_new_result_index();
		}
		static bool emplace_result_notify_remove_ptr_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_job)
		{
			return cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job);
		}
		static _example::cdll_const::type_result get_result(const _example::cdll_const::type_result_index result_index, std::vector<unsigned char>& v_rx, bool b_remove_result_after_get = true)
		{
			return cjob_mgmt::_get_result_storage().get_result(result_index, v_rx, b_remove_result_after_get);
		}
	private:
		static void _worker(cjob_mgmt* p_mgmt)
		{
			bool b_result(false), b_continue(false);
			bool b_run(true), b_notify(false);
			_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_job_new, ptr_job_ing;
			std::cv_status cs(std::cv_status::timeout);

			do {
				do {
					do {
						cs = p_mgmt->m_ptr_woker->wait_for(1);
						if (cs == std::cv_status::timeout)
							continue;
						//new requst case
						if (p_mgmt->m_ptr_woker->is_always_notified()) {
							//terminates thread.
							b_run = false;
							continue;
						}
						if (!p_mgmt->m_ptr_woker->pop_job(ptr_job_new))
							continue;
						if (ptr_job_new->is_cancel_type()) {
							//
							if (ptr_job_ing){
								//cancel the current job
								p_mgmt->_cancel_job(ptr_job_ing, _example::cdll_const::result_cancel);
								cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job_ing);
							}
							ptr_job_new.reset();
							continue;
						}

						if (ptr_job_ing) {
							if (!ptr_job_new->is_cancel_waiting_job()) {
								ptr_job_new->set_result(_example::cdll_const::result_error);
								cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job_new);
								continue;
							}
							//cancel the current job
							p_mgmt->_cancel_job(ptr_job_ing, _example::cdll_const::result_cancel_by_aonther);
							cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job_ing);
						}

						ptr_job_ing.swap(ptr_job_new);
						if (!p_mgmt->_start_job(ptr_job_ing)) {
							cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job_ing);
						}
					} while (false);

					if (!ptr_job_ing || !b_run)
						continue;
					// execute request
					std::tie(b_result, b_continue) = p_mgmt->_run_job_loop(ptr_job_ing);
					if (b_continue)
						continue;
					p_mgmt->_stop_job(ptr_job_ing);
					cjob_mgmt::_get_result_storage().emplace_result_notify_remove_ptr_job(ptr_job_ing);

				} while (b_run);


			} while (false);
		}
	private:
		_example::cdll_const::type_pair_success_continue _run_job_loop(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(false), b_continue(false);

			do {
				if (!ptr_cjob)
					continue;
				std::map< _example::cjob_of_dll::type_requst, _example::cprocess::type_ptr>::iterator it = map_job_process.find(ptr_cjob->get_type());
				if (it == std::end(map_job_process))
					continue;

				std::tie(b_result, b_continue) = it->second->main_loop(ptr_cjob);

			} while (false);
			return std::make_pair(b_result, b_continue);
		}
		bool _cancel_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob, _example::cdll_const::type_result result)
		{
			bool b_result(false);

			do {
				if (!ptr_cjob)
					continue;
				std::map< _example::cjob_of_dll::type_requst, _example::cprocess::type_ptr>::iterator it = map_job_process.find(ptr_cjob->get_type());
				if (it == std::end(map_job_process))
					continue;

				b_result = it->second->cancel(ptr_cjob, result);

			} while (false);
			return b_result;
		}

		bool _start_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(false);

			do {
				if (!ptr_cjob)
					continue;
				std::map< _example::cjob_of_dll::type_requst, _example::cprocess::type_ptr>::iterator it = map_job_process.find(ptr_cjob->get_type());
				if (it == std::end(map_job_process))
					continue;

				b_result = it->second->main_start(ptr_cjob);

			} while (false);
			return b_result;
		}
		bool _stop_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(false);

			do {
				if (!ptr_cjob)
					continue;
				std::map< _example::cjob_of_dll::type_requst, _example::cprocess::type_ptr>::iterator it = map_job_process.find(ptr_cjob->get_type());
				if (it == std::end(map_job_process))
					continue;

				b_result = it->second->main_end(ptr_cjob);

			} while (false);
			return b_result;
		}

		static _example::cresult& _get_result_storage()
		{
			static _example::cresult result_storage;
			return result_storage;
		}
	private:
		long m_n_worker_number;
		cjob_mgmt::_type_ptr_worker m_ptr_woker;

		std::map< _example::cjob_of_dll::type_requst, _example::cprocess::type_ptr> map_job_process;
		
	private://don't call these methods.
		cjob_mgmt(const cjob_mgmt&);
		cjob_mgmt& operator=(const cjob_mgmt&);

	};
}

