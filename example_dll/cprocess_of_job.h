#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <wchar.h>

#include <atltrace.h>

#include "cprocess.h"

namespace _example
{
	class cprocess_of_job : public cprocess
	{
	public:
		cprocess_of_job() : _example::cprocess(), m_n_wait_mmsec(1000*1)
		{
		}
		virtual ~cprocess_of_job() {}

		/**
		* @return first : true - success. false - fail, :: second : true - run more. false - stop
		*/
		virtual _example::cdll_const::type_pair_success_continue main_loop(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(true);
			bool b_run_more(true);

			do {
				//std::this_thread::sleep_for(std::chrono::milliseconds(1));

				if (m_n_wait_mmsec == 0) {
					b_run_more = false;
					continue;
				}
				ATLTRACE(L" + %09u\n", m_n_wait_mmsec);
				--m_n_wait_mmsec;
			} while (false);
			return std::make_pair(b_result, b_run_more);
		}
		/**
		* @return true - success. false - fail
		*/
		virtual bool cancel(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob, const _example::cdll_const::type_result result)
		{
			ptr_cjob->set_result(result);
			return true;
		}

		virtual bool main_start(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			m_n_wait_mmsec = 1000 * 1;
			return cprocess::main_start(ptr_cjob);
		}

		virtual bool main_end(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			bool b_result(true);
			

			do {
				b_result = cprocess::main_end(ptr_cjob);
				if (!b_result)
					continue;
				if (!ptr_cjob)
					continue;

				std::chrono::milliseconds runtime = get_running_time();
				
				std::wstringstream ss;
				ss << runtime.count();
				std::wstring s(ss.str());

				ctools::type_buffer v_rx((s.size()+1) * sizeof(wchar_t), 0);
				memcpy(&v_rx[0], (unsigned char*)s.c_str(), v_rx.size()- sizeof(wchar_t));

				ptr_cjob->set_result(_example::cdll_const::result_success, v_rx);

				b_result = true;
			} while (false);
			return b_result;
		}

	private:
		size_t m_n_wait_mmsec;
	};

}