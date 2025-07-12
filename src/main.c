#include "gui/application.h"

// TODO: Remake GUI:
//      1° Section) rocket shape
//      2° Section) phases
//      3° Section) final phase
// TODO: Create MAX_ANALYSIS_TIME
// TODO: make sure program won't crash
// TODO: Make RocketVariables a hashmap

int main(void)
{
    Application app;

    SetupApplication(&app);
    RunApplication(&app);
    CloseApplication(&app);

    return 0;
}

