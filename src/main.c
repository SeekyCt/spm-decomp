#include <common.h>
#include <spmario.h>
#include <DEMOInit.h>

int main(int argc, char ** argv) {
    (void) argc;
    (void) argv;
    spmarioInit();
    spmarioMain();
    while (true) {
        DEMOBeforeRender();
        spmarioDisp();
        spmarioMain();
        DEMODoneRender();
    }
}
