#pragma once

#include <string>
#include <vector>
#include <set>
#include <sstream> 
#include <algorithm>
#include <thread>

#include <Windows.h>

#include <cdk_map.h>
#include <_safe_cv.h>

#pragma comment(lib, "Version.lib")

class ctools
{
public:
    typedef	std::vector<unsigned char>	type_buffer;
    typedef	std::set<std::wstring>	type_set_wstring;

    typedef std::shared_ptr<std::thread>    type_ptr_thread;
    typedef std::pair< type_ptr_thread, _safe_cv::type_ptr > type_pair_ptr_thread_ptr_cv;
    

    /**
    * type_tuple_tc -> unsigned long n_part , unsigned long n_chapter , unsigned long n_section , unsigned long n_paragraph, unsigned long n_loop, unsigned long n_cancel_mmsec
    */
    typedef std::tuple<unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long>   type_tuple_tc;
    typedef std::vector< ctools::type_tuple_tc>    type_v_tuple_tc;
    

public:
    virtual ~ctools(){}

    static bool get_dll_version( const std::wstring & s_dll, std::wstring & s_version)
    {
        bool b_result(false);
        unsigned long dw_handle(0), dw_len(0);
        unsigned int n_buffer(0);
        VS_FIXEDFILEINFO *p_file_info;
        ctools::type_buffer v_data(0);
        std::vector<unsigned short> v_w_version(0);

        do{
            s_version.clear();

            dw_len = GetFileVersionInfoSize( s_dll.c_str(), &dw_handle );
            if( !dw_len )
                continue;
            //
            v_data.resize(dw_len,0);
            if( !GetFileVersionInfo( s_dll.c_str(), dw_handle, dw_len, &v_data[0] ) )
                continue;
            if( !VerQueryValue( &v_data[0] , L"\\", (LPVOID *) &p_file_info, &n_buffer ) )
                continue;
            v_w_version.push_back( HIWORD(p_file_info->dwFileVersionMS) );
            v_w_version.push_back( LOWORD(p_file_info->dwFileVersionMS) );
            v_w_version.push_back( HIWORD(p_file_info->dwFileVersionLS) );
            v_w_version.push_back( LOWORD(p_file_info->dwFileVersionLS) );

            std::wstringstream ss_v;

            std::for_each( std::begin(v_w_version), std::end(v_w_version), [&]( unsigned short w ){
                ss_v << w << L".";
            });

            s_version = ss_v.str();
            //
            b_result = true;
        }while(false);
        return b_result;
    }

    static std::size_t change( ctools::type_set_wstring & set_s_dst, const wchar_t *p_ss_multi_src)
    {//p_ss_multi_src is multi string
        const wchar_t *pDest;
        std::wstring stemp;
        size_t n_count = 0;
        size_t nOffset = 0;

        do {
            if (p_ss_multi_src == NULL)
                continue;

            set_s_dst.clear();
            //
            while (p_ss_multi_src[nOffset] != NULL) {

                pDest = &(p_ss_multi_src[nOffset]);
                stemp = pDest;
                std::pair<ctools::type_set_wstring::iterator, bool> result = set_s_dst.insert(stemp);

                nOffset += stemp.length() + 1;//for passing null termination

                if(result.second)//inserted.
                    n_count++;
            }//while
        } while (false);
        return n_count;
    }

    /**
    * @brief uncode multi string to mbcs multi string
    * @return the number of string
    */
    static std::size_t change(char* p_s_multi_dst, const wchar_t* p_ss_multi_src)
    {//p_ss_multi_src is multi string
        size_t n_count = 0;
        size_t nOffset = 0;

        do {
            if (p_ss_multi_src == NULL)
                continue;
            //
            do {
                while (p_ss_multi_src[nOffset] != NULL) {
                    p_s_multi_dst[nOffset] = (char)p_ss_multi_src[nOffset];
                    ++nOffset;
                }//while
                p_s_multi_dst[nOffset] = NULL;
                ++n_count;
                ++nOffset;
            } while (p_ss_multi_src[nOffset] != NULL);

            p_s_multi_dst[nOffset] = NULL;

        } while (false);
        return n_count;
    }

    static std::wstring get_string(const ctools::type_buffer& v_string_null_terminated)
    {
        std::wstring s_out;

        do {
            if (v_string_null_terminated.size()<2)
                continue;
            std::transform(
                std::cbegin(v_string_null_terminated), 
                std::cend(v_string_null_terminated)-1,
                std::back_inserter(s_out), [=](unsigned char c)->wchar_t {
                return (wchar_t)c;
                });

        } while (false);
        return s_out;
    }

private://don't call these methods.
    ctools();
	ctools( const ctools & );
	ctools &operator=( const ctools );
}; // ctools


