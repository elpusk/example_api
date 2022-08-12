#include <iostream>

#include "ctc_example_dll.h"

int main()
{
    int n_exit(0);

    rang::setControlMode(rang::control::Auto);

    ctc_example_dll::out_info( L" = test case of example_dll.dll." );

    unsigned long n_part(0), n_chapter(0), n_section(0), n_paragraph(0), n_loop(0);
    bool b_run(true);
    ctc_example_dll& tc_mgmt = ctc_example_dll::get_instance();
    ctools::type_tuple_tc tc;
    bool b_need_epilog(false);

    do {
        b_need_epilog = false;

        if (!ctc_example_dll::get_test_case_from_user(tc)) {
            ctc_example_dll::out_er(L"invalied test case.");
            b_run = false;
            continue;
        }

        if (!tc_mgmt.prolog()) {
            continue;
        }
        b_need_epilog = true;

        if (!tc_mgmt.run(tc)) {
            continue;
        }

        tc_mgmt.epilog();
        b_need_epilog = false;

        b_run = true;
    } while (b_run);

    if(b_need_epilog)
        tc_mgmt.epilog();

    return n_exit;
}
