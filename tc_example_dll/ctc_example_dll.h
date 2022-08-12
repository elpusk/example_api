#pragma once

#include <iostream>
#include <chrono>
#include <map>
#include <functional>
#include <wchar.h>

#include <_safe_cv.h>
#include <cexample_dll.h>
#include <ctools.h>
#include <3thpart/rang/wrang.hpp>

#include <atltrace.h>

class ctc_example_dll
{
public:
	/**
	* console output function
	*/
	static void out_info(const std::wstring& s_msg)
	{
		std::wcout << s_msg << std::endl;
	}
	static void out_ok(const std::wstring& s_msg)
	{
		std::wcout << rang::fg::green << L" [OK] : " << s_msg << std::endl << rang::style::reset;
	}
	static void out_er(const std::wstring& s_msg)
	{
		std::wcout << rang::fg::red << L" [ER] : " << s_msg << std::endl << rang::style::reset;
	}

public:
	typedef	std::function<bool(bool)>	_type_fun_tc;
	typedef	std::pair<ctc_example_dll::_type_fun_tc, ctools::type_v_tuple_tc>	_type_pair_fun_pre_condition;
	typedef	std::map<std::wstring, ctc_example_dll::_type_pair_fun_pre_condition >	_type_map_index_pair_fun_pre_condition;

public:
	static ctc_example_dll& get_instance()
	{
		static ctc_example_dll obj;
		return obj;
	}

	~ctc_example_dll()
	{
		epilog();
	}

	static bool get_test_case_from_user(ctools::type_tuple_tc& out_tc)
	{
		bool b_result(false);
		do {
			unsigned long n_part(0), n_chapter(0), n_section(0), n_paragraph(0), n_loop(0), n_cancel_mmsec(0);
			std::wcout << L" input part number (0 ~ 4) : ";
			std::wcin >> n_part;
			if (n_part > 4) {
				continue;
			}
			//
			std::wcout << L" input chapter number : ";
			std::wcin >> n_chapter;
			//
			if (n_chapter == 5) {//cancel test
				std::wcout << L" input section number : ";
				std::wcin >> n_section;

				std::wcout << L" input cancel time[mmsec] after staring : ";
				std::wcin >> n_cancel_mmsec;
			}
			//
			//std::wcout << L" input paragraph number : ";
			//std::wcin >> n_paragraph;
			//
			//std::wcout << L" input loop counter : ";
			//std::wcin >> n_loop;
			//
			out_tc = std::make_tuple(n_part, n_chapter, n_section, n_paragraph, n_loop, n_cancel_mmsec);

			b_result = true;
		} while (false);
		return b_result;
	}

public:
	bool prolog()
	{
		bool b_result(false);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.load(m_s_dll)) {
				std::wcout << L" fail : load dll : " << m_s_dll << std::endl;
				continue;
			}
			std::wcout << L" success : load dll : " << m_s_dll << std::endl;
			m_b_need_unload = true;
			//
			std::wstring s_version;
			if (ctools::get_dll_version(m_s_dll, s_version)) {
				std::wcout << L" dll version : " << s_version << std::endl;
			}

			// setting sub test function.
			b_result = true;
		} while (false);

		if (b_result)
			ctc_example_dll::out_ok(__WFUNCTION__);
		else
			ctc_example_dll::out_er(__WFUNCTION__);
		return b_result;
	}
	void epilog()
	{
		unsigned long n_result(0);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			if (m_b_need_off) {
				n_result = dll.off();
				if (n_result == _example::cdll_const::result_error) {
					std::wcout << L" fail : example_dll_off() : " << n_result << L"(" << cexample_dll::get_return_value_by_wstring(n_result) << L")" << std::endl;
				}
				else {
					std::wcout << L" success : example_dll_off() : " << n_result << L"(" << cexample_dll::get_return_value_by_wstring(n_result) << L")" << std::endl;
				}
			}
			if (m_b_need_unload) {
				dll.unload();
				std::wcout << L" success : unload dll" << std::endl;
			}
		} while (false);

		_ini();
		ctc_example_dll::out_ok(__WFUNCTION__);
	}

	bool run(const ctools::type_tuple_tc tc)
	{
		bool b_result(false), b_run(false);
		unsigned long n_part(0), n_chapter(0), n_section(0), n_paragraph(0), n_loop(0), n_cancel_mmsec(0);

		std::chrono::steady_clock::time_point tp_start, tp_end;
		std::chrono::milliseconds run_time;

		tp_start = std::chrono::high_resolution_clock::now();
		do {
			std::tie(n_part, n_chapter, n_section, n_paragraph, n_loop, n_cancel_mmsec) = tc;

			switch (n_part) {
			case 2:
				std::tie(b_run,b_result) = _run_in_map_with_pre_condition(true, n_part, n_chapter, n_section, n_paragraph, n_cancel_mmsec);
				break;
			default:
				continue;
			}//end switch
			
		} while (false);

		tp_end = std::chrono::high_resolution_clock::now();
		run_time = std::chrono::duration_cast<std::chrono::milliseconds>(tp_end - tp_start);

		std::wstringstream ss;
		ss << __WFUNCTION__ << L" - TC." << n_part << L'.' << n_chapter << L'.' << n_section << L'.' << n_paragraph << L'(' << n_loop << L')'
			<< L'{' << run_time.count() << L"mm sec}."
			<< std::endl << rang::style::reset;

		if (b_result)
			ctc_example_dll::out_ok(ss.str());
		else
			ctc_example_dll::out_er(ss.str());

		return b_result;
	}

private:
	////////////////////////////////////////////
	// start mapping function 
	// don't call these methods directly.
	bool _run_002_001(bool b_out)
	{
		return _dll_on(b_out);
	}
	bool _run_002_002(bool b_out)
	{
		return _dll_off(b_out);
	}
	bool _run_002_004(bool b_out)
	{
		bool b_result(false);
		_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);
		long n_job(_example::cdll_const::const_invalid_job_number);

		do {
			std::tie(b_result, n_result, n_job) = _dll_run_job1(b_out, _generate_tx_data());
			if (!b_result)
				continue;//error return 

			b_result = false;
			// wait receive response
			_waits_job_result(n_job);
			ctools::type_buffer v_rx;
			if (_dll_get_result(b_out,n_result, v_rx) != _example::cdll_const::result_success)
				continue;

			std::wstring s((wchar_t*)&v_rx[0]);

			std::wstringstream ss;
			ss << L"running time : " << s;
			ctc_example_dll::out_info(ss.str());

			b_result = true;
		} while (false);
		return b_result;
	}
	bool _run_002_005(bool b_out)
	{
		bool b_result(false);
		_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);
		long n_job(_example::cdll_const::const_invalid_job_number);

		do {
			std::tie(b_result, n_result, n_job) = _dll_run_job2(b_out, _generate_tx_data());
			if (!b_result)
				continue;//error return 

			b_result = false;
			// wait receive response
			_waits_job_result(n_job);
			ctools::type_buffer v_rx;
			if (_dll_get_result(b_out, n_result, v_rx) != _example::cdll_const::result_success)
				continue;

			std::wstring s((wchar_t*)&v_rx[0]);

			std::wstringstream ss;
			ss << L"running time : " << s;
			ctc_example_dll::out_info(ss.str());

			b_result = true;
		} while (false);
		return b_result;
	}
	bool _run_002_006_001(bool b_out)
	{
		bool b_result(false), b_canceled(false);
		_example::cdll_const::type_result_index n_result_index(_example::cdll_const::result_index_error);
		long n_job(_example::cdll_const::const_invalid_job_number);

		do {
			std::tie(b_result, n_result_index, n_job) = _dll_run_job1(b_out, _generate_tx_data());
			if (!b_result)
				continue;//error return 

			b_result = false;
			// after 1 mmsec, cancel.
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			b_canceled = _dll_cancel(b_out, n_result_index);
			if (!b_canceled) {
				continue;
			}

			// wait receive response
			_waits_job_result(n_job);
			ctools::type_buffer v_rx;
			switch (_dll_get_result(b_out, n_result_index, v_rx)) {
			case _example::cdll_const::result_cancel:
				break;
			case _example::cdll_const::result_success:
			case _example::cdll_const::result_cancel_by_aonther:
			default:
				continue;//not canceled error
			}//end switch

			b_result = true;
		} while (false);

		if (!b_canceled) {
			// this part also not canceled error.
			_waits_job_result(n_job);//wait the end of job processing
		}
		return b_result;
	}
	bool _run_002_006_002(bool b_out)
	{
		bool b_result(false), b_canceled(false);
		_example::cdll_const::type_result_index n_result_index(_example::cdll_const::result_index_error);
		long n_job(_example::cdll_const::const_invalid_job_number);

		do {
			std::tie(b_result, n_result_index, n_job) = _dll_run_job2(b_out, _generate_tx_data());
			if (!b_result)
				continue;//error return 

			b_result = false;
			// after 2 mmsec, cancel.
			std::this_thread::sleep_for(std::chrono::milliseconds(2));

			b_canceled = _dll_cancel(b_out, n_result_index);
			if (!b_canceled) {
				continue;
			}

			// wait receive response
			_waits_job_result(n_job);
			ctools::type_buffer v_rx;
			switch (_dll_get_result(b_out, n_result_index, v_rx)) {
			case _example::cdll_const::result_cancel:
				break;
			case _example::cdll_const::result_success:
			case _example::cdll_const::result_cancel_by_aonther:
			default:
				continue;//not canceled error
			}//end switch

			b_result = true;
		} while (false);

		if (!b_canceled) {
			// this part also not canceled error.
			_waits_job_result(n_job);//wait the end of job processing
		}
		return b_result;
	}
	
	// end mapping function
	////////////////////////////////////////////
	_example::cdll_const::type_pair_run_success _run_in_map(bool b_out, unsigned long n_part, unsigned long n_chapter, unsigned long n_section, unsigned long n_paragraph , unsigned long n_cancel_mmsec)
	{
		bool b_result(false);
		bool b_run(false);
		do {
			std::wstring s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
			ctc_example_dll::_type_map_index_pair_fun_pre_condition::iterator it = m_map_index_pair_fun_pre_run.find(s_key);
			if (it == std::end(m_map_index_pair_fun_pre_run))
				continue;
			if (!it->second.first)
				continue;

			b_run = true;

			b_result = it->second.first(b_out);
		} while (false);
		return std::make_pair(b_run, b_result);
	}

	_example::cdll_const::type_pair_run_success _run_in_map_with_pre_condition(bool b_out, unsigned long n_part, unsigned long n_chapter, unsigned long n_section, unsigned long n_paragraph, unsigned long n_cancel_mmsec)
	{
		bool b_result(false);
		bool b_run(false);

		do {
			std::wstring s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
			ctc_example_dll::_type_map_index_pair_fun_pre_condition::iterator it = m_map_index_pair_fun_pre_run.find(s_key);
			if (it == std::end(m_map_index_pair_fun_pre_run))
				continue;

			_example::cdll_const::type_pair_run_success b_pair_result(false, false);

			ctools::type_v_tuple_tc::iterator it_pre = std::begin(it->second.second);
			if (it_pre != std::end(it->second.second)) {
				if (!it->second.second.empty()) {

					for (; it_pre != std::end(it->second.second); ++it_pre) {
						n_part = std::get<0>(*it_pre);
						n_chapter = std::get<1>(*it_pre);
						n_section = std::get<2>(*it_pre);
						n_paragraph = std::get<3>(*it_pre);
						n_cancel_mmsec = std::get<4>(*it_pre);
						std::tie(b_run,b_result) = _run_in_map(b_out, n_part, n_chapter, n_section, n_paragraph, n_cancel_mmsec);
						if (!(b_run && b_result)) {
							break;
						}
					}//end for
					if (!(b_run && b_result)) {
						continue;
					}
				}
			}

			b_run = b_result = false;
			if (!it->second.first)
				continue;

			b_run = true;
			b_result = it->second.first(b_out);
		} while (false);
		return std::make_pair(b_run, b_result);
	}

private:
	bool _dll_on(bool b_out)
	{
		bool b_result(false);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			result = dll.on();
			if (result != _example::cdll_const::result_success)
				continue;
			//
			b_result = true;
		} while (false);
		return b_result;
	}
	bool _dll_off(bool b_out)
	{
		bool b_result(false);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			result = dll.off();
			if (result != _example::cdll_const::result_success)
				continue;
			//
			b_result = true;
		} while (false);
		return b_result;
	}

	std::tuple<bool, _example::cdll_const::type_result_index> _dll_create_worker(bool b_out, const std::wstring & s_worker_name)
	{
		bool b_result(false);
		_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			n_result = dll.create_worker(s_worker_name, ctc_example_dll::_cb_for_create_worker, this);
			if (n_result == _example::cdll_const::result_index_error) {
				continue;
			}

			b_result = true;
		} while (false);
		return std::make_tuple(b_result, n_result);
	}
	std::tuple<bool, _example::cdll_const::type_result_index> _dll_remove_worker(bool b_out, long n_worker_number)
	{
		bool b_result(false);
		_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			n_result = dll.remove_worker(n_worker_number, ctc_example_dll::_cb_for_remove_worker, this);
			if (n_result == _example::cdll_const::result_index_error) {
				continue;
			}

			b_result = true;
		} while (false);
		return std::make_tuple(b_result, n_result);
	}

	std::tuple<bool, _example::cdll_const::type_result_index, long> _dll_run_job1(bool b_out, long n_worker_number, const ctools::type_buffer & v_tx)
	{
		bool b_result(false);
		_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);

			n_result = dll.run_job1(n_worker_number, v_tx, ctc_example_dll::_cb_for_job1, this);
			if (n_result == _example::cdll_const::result_index_error) {
				_erase_worker_number(n_worker_number);
				continue;
			}

			b_result = true;
		} while (false);
		return std::make_tuple(b_result, n_result, n_worker_number);
	}
	bool _dll_cancel(bool b_out, const long n_worker_index)
	{
		bool b_result(false);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			result = dll.cancel_jobs(n_index);
			if (result != _example::cdll_const::result_success)
				continue;
			//
			b_result = true;
		} while (false);
		return b_result;
	}

	_example::cdll_const::type_result _dll_get_result(bool b_out, const _example::cdll_const::type_result_index n_index, ctools::type_buffer &v_rx)
	{
		_example::cdll_const::type_result result(_example::cdll_const::result_error);
		cexample_dll& dll = cexample_dll::get_instance();

		do {
			if (!dll.is_loaded())
				continue;
			
			result = dll.get_result_of_job(n_index, v_rx);
		} while (false);
		return result;
	}


private:
	static _example::cdll_const::type_result __stdcall _cb_for_job1(long n_job_number, _example::cdll_const::type_result_index n_result_index, _example::cdll_const::type_result result, void *p_para)
	{
		static size_t n_count(0);

		do {
			if (!p_para)
				continue;

			ATLTRACE(L"%s : %u.\n", __WFUNCTION__, n_count++);

			ctc_example_dll* p_obj((ctc_example_dll*)p_para);

			p_obj->_notify_job_result(n_job_number);
			
		} while (false);
		return result;
	}
	static _example::cdll_const::type_result __stdcall _cb_for_job2(long n_job_number, _example::cdll_const::type_result_index n_result_index, _example::cdll_const::type_result result, void* p_para)
	{
		static size_t n_count(0);

		do {
			if (!p_para)
				continue;

			ATLTRACE(L"%s : %u.\n", __WFUNCTION__, n_count++);

			ctc_example_dll* p_obj((ctc_example_dll*)p_para);

			p_obj->_notify_job_result(n_job_number);
		} while (false);
		return result;
	}

	ctc_example_dll()
	{
		_ini_tc_map();
		_ini();
	}

	void _ini_tc_map()
	{
		unsigned long n_part(0), n_chapter(0), n_section(0), n_paragraph(0), n_loop(0);
		std::wstring s_key;
		ctools::type_v_tuple_tc v_precondition(0);

		n_part = n_chapter = n_section = n_paragraph = n_loop = 0;

		n_part = 2;
		n_chapter = 1;
		v_precondition.resize(0);
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph, n_loop);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_001, this, std::placeholders::_1), v_precondition);

		n_chapter = 2;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_002, this, std::placeholders::_1), v_precondition);

		n_chapter = 3;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_003, this, std::placeholders::_1), v_precondition);

		n_chapter = 4;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 0));
		v_precondition.push_back(std::make_tuple(2, 3, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_004, this, std::placeholders::_1), v_precondition);

		n_chapter = 5;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 0));
		v_precondition.push_back(std::make_tuple(2, 3, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_005, this, std::placeholders::_1), v_precondition);

		n_chapter = 6;
		n_section = 1;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 1)); //after 1 msec, cancel 
		v_precondition.push_back(std::make_tuple(2, 3, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_006_001, this, std::placeholders::_1), v_precondition);

		n_chapter = 6;
		n_section = 2;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 2));//after 2 msec, cancel
		v_precondition.push_back(std::make_tuple(2, 3, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_006_002, this, std::placeholders::_1), v_precondition);

		n_chapter = 7;
		n_section = 0;
		v_precondition.resize(0);
		v_precondition.push_back(std::make_tuple(2, 1, 0, 0, 0, 0));
		v_precondition.push_back(std::make_tuple(2, 3, 0, 0, 0, 0));
		s_key = _generate_tc_map_key(n_part, n_chapter, n_section, n_paragraph);
		m_map_index_pair_fun_pre_run[s_key] = std::make_pair(std::bind(&ctc_example_dll::_run_002_007, this, std::placeholders::_1), v_precondition);
	}
	void _ini()
	{
		m_b_need_unload = m_b_need_off = false;
		m_s_dll = L"example_dll.dll";
		m_n_worker_number = -1;
	}
	void _erase_worker_number(long n_job_index)
	{
		std::lock_guard<std::mutex> lock(m_mutex_map);
		std::map<long, _safe_cv::type_ptr>::iterator it = m_map_long_ptr_safe_cv.find(n_job_index);
		if (it != std::end(m_map_long_ptr_safe_cv)) {
			m_map_long_ptr_safe_cv.erase(it);
		}
	}
	void _waits_job_result(long n_job_index)
	{
		_safe_cv::type_ptr ptr_safe_cv;
		do {
			std::lock_guard<std::mutex> lock(m_mutex_map);
			std::map<long, _safe_cv::type_ptr>::iterator it = m_map_long_ptr_safe_cv.find(n_job_index);
			if (it != std::end(m_map_long_ptr_safe_cv)) {
				ptr_safe_cv = it->second;
			}
		} while (false);

		if (ptr_safe_cv)
			ptr_safe_cv->wait();
	}
	void _notify_job_result(long n_job_index)
	{
		_safe_cv::type_ptr ptr_safe_cv;
		do {
			std::lock_guard<std::mutex> lock(m_mutex_map);
			std::map<long, _safe_cv::type_ptr>::iterator it = m_map_long_ptr_safe_cv.find(n_job_index);
			if (it != std::end(m_map_long_ptr_safe_cv)) {
				ptr_safe_cv = it->second;
			}
		} while (false);

		if (ptr_safe_cv)
			ptr_safe_cv->notify_one();
	}

	ctools::type_buffer _generate_tx_data()
	{
		ctools::type_buffer v_tx(0);

		auto tp(std::chrono::system_clock::now());
		std::time_t t_time = std::chrono::system_clock::to_time_t(tp);

		std::stringstream ss;
		ss << std::ctime(&t_time);

		std::string s_t(ss.str());
		std::wstring s_time;
		std::transform(std::begin(s_t), std::end(s_t), std::back_inserter(s_time), [=](char c)->wchar_t {
			return (wchar_t)c;
			});

		v_tx.resize((s_time.size()+1) * sizeof(wchar_t), 0);//included null termainted
		
		
		wchar_t* ps_des((wchar_t*) & v_tx[0]);
		wcscpy(ps_des, s_time.c_str());
		return v_tx;
	}

	std::wstring _generate_tc_map_key(unsigned long n_part = 0, unsigned long n_chapter = 0, unsigned long n_section = 0, unsigned long n_paragraph = 0, unsigned long n_loop = 0)
	{
		//generate key
		std::wstring s_key = std::to_wstring((unsigned long long)n_part);
		s_key += L":";

		s_key += std::to_wstring((unsigned long long)n_chapter);
		s_key += L":";

		s_key += std::to_wstring((unsigned long long)n_section);
		s_key += L":";

		s_key += std::to_wstring((unsigned long long)n_paragraph);
		s_key += L":";

		s_key += std::to_wstring((unsigned long long)n_loop);
		return s_key;
	}

private:
	_type_map_index_pair_fun_pre_condition m_map_index_pair_fun_pre_run;
	
	long m_n_worker_number;
	bool m_b_need_unload, m_b_need_off;
	std::wstring m_s_dll;

	std::mutex m_mutex_map;
	std::map<long, _safe_cv::type_ptr> m_map_long_ptr_safe_cv;

private://don't call these methods
	ctc_example_dll(const ctc_example_dll&);
	ctc_example_dll& operator=(const ctc_example_dll&);

};

