#pragma once

/**
* easy for use of example_dll in C++ envirment.
* dynamic binding and unbinding.
* 
*/
#include <string>
#include <mutex>
#include <vector>

#include <Windows.h>

#include "example_dll.h"

class cexample_dll
{
public:
	enum {
		const_size_max_rx_data = 4096

	};

private:
	typedef	_example::cdll_const::type_result(__stdcall* _type_on)();
	typedef	_example::cdll_const::type_result(__stdcall* _type_off)();

	typedef	_example::cdll_const::type_result_index(__stdcall* _type_create_worker_a)(const char*, const unsigned long, const _example::cdll_const::type_dll_callback, void*);
	typedef	_example::cdll_const::type_result_index(__stdcall* _type_create_worker_w)(const wchar_t*, const unsigned long, const _example::cdll_const::type_dll_callback, void*);
	typedef	_example::cdll_const::type_result_index(__stdcall* _type_remove_worker)(long, const _example::cdll_const::type_dll_callback, void*);

	typedef	_example::cdll_const::type_result_index(__stdcall* _type_run_job)(const long, const unsigned char*, const unsigned long, const _example::cdll_const::type_dll_callback, void*, const long);

	typedef	_example::cdll_const::type_result(__stdcall* _type_cancel_jobs)(const long);
	typedef	_example::cdll_const::type_result(__stdcall* _type_get_result_of_job)(const _example::cdll_const::type_result_index, unsigned char*, unsigned long*, long);
public:
	static cexample_dll& get_instance()
	{
		static cexample_dll obj;
		return obj;

	}
	static std::wstring get_return_value_by_wstring(unsigned long n_result)
	{
		std::wstring s_result(L"unknown");

		switch (n_result)
		{
		case _example::cdll_const::result_success:
			s_result = L"result_success";
			break;
		case _example::cdll_const::result_error:
			s_result = L"result_error";
			break;
		case _example::cdll_const::result_ing:
			s_result = L"result_ing";
			break;
		default:
			break;
		}//end switch

		return s_result;
	}

	~cexample_dll()
	{
		_unload();
	}

	bool is_loaded()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_b_load;
	}

	bool load(const std::wstring& s_dll_path = std::wstring(L"example_dll.dll"))
	{
		bool b_result(false);
		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			if (m_b_load) {
				b_result = true;
				continue;//already loaded
			}

			if (s_dll_path.empty()) {
				continue;
			}
			m_h_mode = ::LoadLibrary(s_dll_path.c_str());
			if (!m_h_mode)
				continue;
			//
			m_on = reinterpret_cast<cexample_dll::_type_on>(::GetProcAddress(m_h_mode, "example_dll_on"));
			m_off = reinterpret_cast<cexample_dll::_type_on>(::GetProcAddress(m_h_mode, "example_dll_off"));

			m_run_job = reinterpret_cast<cexample_dll::_type_run_job>(::GetProcAddress(m_h_mode, "example_dll_run_job"));

			m_cancel_jobs = reinterpret_cast<cexample_dll::_type_cancel_jobs>(::GetProcAddress(m_h_mode, "example_dll_cancel_jobs"));
			m_get_result_of_job = reinterpret_cast<cexample_dll::_type_get_result_of_job>(::GetProcAddress(m_h_mode, "example_dll_get_result_of_job"));

			m_remove_worker = reinterpret_cast<cexample_dll::_type_remove_worker>(::GetProcAddress(m_h_mode, "example_dll_remove_worker"));
#ifdef UNICODE
			m_create_worker = reinterpret_cast<cexample_dll::_type_create_worker_w>(::GetProcAddress(m_h_mode, "example_dll_create_worker_w"));
#else
			m_create_worker = reinterpret_cast<cexample_dll::_type_create_worker_a>(::GetProcAddress(m_h_mode, "example_dll_create_worker_a"));
#endif//UNICODE

			do {
				if (m_on == nullptr)
					continue;
				if (m_off == nullptr)
					continue;
				if (m_run_job == nullptr)
					continue;
				if (m_cancel_jobs == nullptr)
					continue;
				if (m_get_result_of_job == nullptr)
					continue;
				if (m_remove_worker == nullptr)
					continue;
				if (m_create_worker == nullptr)
					continue;
				//
				b_result = true;
			} while (false);

			if (!b_result) {
				::FreeLibrary(m_h_mode);
				_ini();
				continue;
			}

			m_b_load = true;

		} while (false);
		return b_result;
	}

	bool unload()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return _unload();
	}

	_example::cdll_const::type_result on()
	{
		_example::cdll_const::type_result result(_example::cdll_const::result_error);
		if (m_on) {
			result = m_on();
		}
		return result;
	}
	_example::cdll_const::type_result off()
	{
		_example::cdll_const::type_result result(_example::cdll_const::result_error);
		if (m_off) {
			result = m_off();
		}
		return result;
	}

	_example::cdll_const::type_result_index run_job(
		const long n_woker_number,
		const std::vector<unsigned char> & v_tx,
		const _example::cdll_const::type_dll_callback cb, 
		void *p_cb_parameter,
		const long b_cancel_waiting_job
		)
	{
		_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);
		if (m_run_job) {
			if (v_tx.empty()) {
				result_index = m_run_job(n_woker_number,NULL,0,cb, p_cb_parameter, b_cancel_waiting_job);
			}
			else {
				result_index = m_run_job(n_woker_number, &v_tx[0], (const unsigned long)v_tx.size(), cb, p_cb_parameter, b_cancel_waiting_job);
			}
		}
		return result_index;
	}

	_example::cdll_const::type_result cancel_jobs(const long n_woker_number)
	{
		_example::cdll_const::type_result result(_example::cdll_const::result_error);
		if (m_cancel_jobs) {
			result = m_cancel_jobs(n_woker_number);
		}
		return result;
	}

	_example::cdll_const::type_result get_result_of_job(
		const _example::cdll_const::type_result_index n_index,
		std::vector<unsigned char> & v_out,
		bool b_remove_after_get
	)
	{
		_example::cdll_const::type_result result(_example::cdll_const::result_error);
		if (m_get_result_of_job) {
			v_out.resize(cexample_dll::const_size_max_rx_data,0);

			unsigned long n_size((unsigned long)v_out.size());

			long b_remove(0);
			if (b_remove_after_get)
				b_remove = 1;
			//
			result = m_get_result_of_job(n_index, &v_out[0], &n_size, b_remove);
		}
		return result;
	}

	_example::cdll_const::type_result_index remove_worker(
		const long n_woker_number,
		const _example::cdll_const::type_dll_callback cb,
		void* p_cb_parameter
	)
	{
		_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);
		if (m_remove_worker) {
			result_index = m_remove_worker(n_woker_number, cb, p_cb_parameter);
		}
		return result_index;
	}

	_example::cdll_const::type_result_index create_worker(
#ifdef UNICODE
		const std::wstring & s_name,
#else
		const std::string& s_name,
#endif
		const _example::cdll_const::type_dll_callback cb,
		void* p_cb_parameter
	)
	{
		_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);
		if (m_create_worker) {
			result_index = m_create_worker(s_name.c_str(), s_name.size()+1, cb, p_cb_parameter);
		}
		return result_index;
	}

private:
	cexample_dll()
	{
		_ini();
	}

	void _ini()
	{
		m_b_load = false;
		m_h_mode = NULL;
		m_on = nullptr;
		m_off = nullptr;
		m_run_job = nullptr;
		m_cancel_jobs = nullptr;
		m_get_result_of_job = nullptr;

		m_create_worker = nullptr;
		m_remove_worker = nullptr;
	}

	bool _unload()
	{
		bool b_result(false);
		do {
			if (!m_b_load) {
				b_result = true;
				continue;//already unloaded
			}

			::FreeLibrary(m_h_mode);
			_ini();

			b_result = true;
		} while (false);
		return b_result;
	}

private:
	std::mutex m_mutex;
	bool m_b_load;
	HMODULE m_h_mode;

	cexample_dll::_type_on m_on;
	cexample_dll::_type_off m_off;
	cexample_dll::_type_run_job m_run_job;
	cexample_dll::_type_cancel_jobs m_cancel_jobs;
	cexample_dll::_type_get_result_of_job m_get_result_of_job;

	cexample_dll::_type_remove_worker m_remove_worker;
#ifdef UNICODE
	cexample_dll::_type_create_worker_w m_create_worker;
#else
	cexample_dll::_type_create_worker_a m_create_worker;
#endif // UNICODE

	

private://don't call these methods
	cexample_dll(const cexample_dll&);
	cexample_dll& operator=(const cexample_dll&);
};

