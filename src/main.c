#include <common.h>
#include <spm/main.h>
#include <wii/DEMOInit.h>
#include <spm/spmario.h>

extern "C" {

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

}
