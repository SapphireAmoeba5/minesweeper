#include <macros.h>
#include <minesweeper.h>

int main()
{
    DEBUG_INFO("Initializing minesweeper");
    if(!Minesweeper::Init())
    {
        DEBUG_ERROR("Error initializing the game. Closing application");
        return 1;
    }
    
    DEBUG_INFO("Running minesweeper");
    Minesweeper::Run();

    DEBUG_INFO("Shutting down minesweeper");
    Minesweeper::Shutdown();
}