#include <common.h>
#include <spm/main.h>
#include <spm/DEMOInit.h>
#include <spm/spmario.h>

int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    spmarioInit();
    spmarioMain();

    while (true)
    {
        DEMOBeforeRender();

        spmarioDisp();
        spmarioMain();

        DEMODoneRender();
    }
}
