#pragma once
#include <chrono>

#include "cjob_of_dll.h"
namespace _example
{
	class cprocess
	{
	public:
		typedef std::shared_ptr<cprocess> type_ptr;

	public:
		cprocess() 
		{
			_ini();
		}
		virtual ~cprocess() {}

		/**
		* @return true - success. false - fail
		*/
		virtual bool main_start(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(true);

			do {
				m_tp_start = std::chrono::high_resolution_clock::now();
			} while (false);
			return b_result;
		}
		/**
		* @return true - success. false - fail
		*/
		virtual bool main_end(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(true);

			do {
				std::chrono::steady_clock::time_point tp_end = std::chrono::high_resolution_clock::now();
				m_run_time = std::chrono::duration_cast<std::chrono::milliseconds>(tp_end - m_tp_start);
			} while (false);
			return b_result;
		}

		std::chrono::milliseconds get_running_time() const
		{
			return m_run_time;
		}

		/**
		* @return first : true - success. false - fail, :: second : true - run more. false - stop
		*/
		virtual _example::cdll_const::type_pair_success_continue main_loop(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob) = 0;
		/* default body form
		{
			bool b_result(true);
			bool b_run_more(true);

			do {


			} while (false);
			return std::make_pair(b_result, b_run_more);
		}
		*/

		/**
		* @return true - success. false - fail
		*/
		virtual bool cancel(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob, const _example::cdll_const::type_result result) = 0;
		/* default body form
		{
			bool b_result(true);

			do {


			} while (false);
			return b_result;
		}
		*/

	protected:
		void _ini()
		{
			m_type = _example::cjob_of_dll::req_type_unknown;
		}
	protected:
		_example::cjob_of_dll::type_requst m_type;
		std::chrono::steady_clock::time_point m_tp_start;
		std::chrono::milliseconds m_run_time;
	};

}