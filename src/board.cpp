#include <board.h>

#include <macros.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Random.h>

namespace Board
{
    int Index(int x, int y);
    void FirstClick(int x, int y);
    void countBombs(const glm::ivec2& currentIndex, const glm::ivec2& nextIndex);
    struct Tile
    {
        int bombs = 0;
        bool isBomb = false;
        bool isFlagged = false;
        bool isClicked = false;
    };


    glm::vec2 windowSize;
    glm::vec2 tileSize;
    glm::ivec2 boardSize2D;

    Tile* board;
    int boardSize1D;

    bool firstClick;
    bool failed;

    uint32_t textureClicked[9];
    uint32_t sqbomb;
    uint32_t sqbombred;
    uint32_t sqflag;
    uint32_t unclicked;

    bool texturesLoaded = false;
    void Init(const glm::ivec2& boardSize, const glm::vec2& iwindowSize)
    {
        boardSize2D = boardSize;
        boardSize1D = boardSize.x * boardSize.y;
        windowSize = iwindowSize;
        tileSize = { windowSize.x / boardSize2D.x, windowSize.y / boardSize2D.y };
        board = new Tile[boardSize1D];

        firstClick = true;
        failed = false;

        if(!texturesLoaded)
        {
            textureClicked[0] = Renderer::LoadTexture("assets/textures/tiles/empty.png");
            textureClicked[1] = Renderer::LoadTexture("assets/textures/tiles/square1.png");
            textureClicked[2] = Renderer::LoadTexture("assets/textures/tiles/square2.png");
            textureClicked[3] = Renderer::LoadTexture("assets/textures/tiles/square3.png");
            textureClicked[4] = Renderer::LoadTexture("assets/textures/tiles/square4.png");
            textureClicked[5] = Renderer::LoadTexture("assets/textures/tiles/square5.png");
            textureClicked[6] = Renderer::LoadTexture("assets/textures/tiles/square6.png");
            textureClicked[7] = Renderer::LoadTexture("assets/textures/tiles/square7.png");
            textureClicked[8] = Renderer::LoadTexture("assets/textures/tiles/square8.png");

            sqbomb = Renderer::LoadTexture("assets/textures/tiles/squarebomb.png");
            sqbombred = Renderer::LoadTexture("assets/textures/tiles/squarebombred.png");
            sqflag = Renderer::LoadTexture("assets/textures/tiles/squareflag.png");
            unclicked = Renderer::LoadTexture("assets/textures/tiles/unclicked.png");

            bool loadFailure = false;
            for(int i = 0; i < 9; i++)
            {
                if(textureClicked[i] == 0)
                {
                    DEBUG_WARN("Failure loading textures!");
                    loadFailure = true;
                    break;
                }
            }

            if(!loadFailure)
            {
                if(sqbomb == 0)
                    loadFailure = true;
                else if(sqbombred == 0)
                    loadFailure = true;
                else if(sqflag == 0)
                    loadFailure = true;
                else if(unclicked == 0)
                    loadFailure = true;

                if(loadFailure)
                    DEBUG_WARN("Failure loading textures!");
            }
        }

        texturesLoaded = true;

    }
    void Shutdown()
    {

        delete[] board;
    }

    void Draw(Window& window)
    {
        // Calculate the tile size each frame
        tileSize = { windowSize.x / boardSize2D.x, windowSize.y / boardSize2D.y };

        // Draw each tile in a nested for loop
        for (int x = 0; x < boardSize2D.x; x++)
        {
            for (int y = 0; y < boardSize2D.y; y++)
            {
                // The position of the current tile
                glm::vec2 pos = { tileSize.x * x + tileSize.x / 2, tileSize.y * y + tileSize.y / 2 };

                // The index of the current tile
                int index = Index(x, y);

                glm::ivec2 cursorPos = window.CursorPos();
                glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
                if(!failed && (int)(cursorPos.x / tileSize.x) == x && (int)(cursorPos.y / tileSize.y) == y)
                {   
                    color = {0.80f, 0.80f, 0.80f, 1.0f};
                }

                // The texture ID that will be rendered
                uint32_t id;

                // Find which texture ID to render with
                if(board[index].isBomb && board[index].isClicked)
                    id = sqbombred;
                else if(board[index].isBomb && failed)
                    id = sqbomb;
                else if(board[index].isFlagged)
                    id = sqflag;
                else if(board[index].isClicked)
                    id = textureClicked[board[index].bombs];
                else if(!board[index].isClicked)
                    id = unclicked;

                Renderer::DrawQuad(pos, tileSize, color, id);
            }
        }
    }

    void Click(const glm::vec2& pos,  Action action)
    {
        // The x and y index if the tile that was clicked
        int x = pos.x / tileSize.x;
        int y = pos.y / tileSize.y;

        // If failed or the mouse click is out of bounds then exit the function
        if(failed || x < 0 || y < 0 || x >= boardSize2D.x || y >= boardSize2D.y)
            return;

        int index = Index(x,y);
        if(action == Action::LeftClick)
        {
            if (firstClick)
            {
                FirstClick(x, y);
                firstClick = false;
            }

            if(!board[index].isFlagged && board[index].isBomb)
            {
                failed = true;
                board[index].isClicked = true;
            }
            else if(!board[index].isFlagged && !board[index].isBomb)
            {
                board[index].isClicked = true;
            }
        }
        else if(action == Action::RightClick && !board[index].isClicked)
        {
            board[index].isFlagged = !board[index].isFlagged;
        }
        
    }

    bool GetFailStatus()
    {
        return failed;
    }
}


// LOCAL FUNCTION DECLARATIONS
int Board::Index(int x, int y)
{
    return y * boardSize2D.x + x;
}

void Board::FirstClick(int x, int y)
{
    Random ran;

    // Randomly place bombs on the board
    ran.SetRange(boardSize1D / 5, boardSize1D / 3);
    int mineCount = ran();


    ran.SetRange(0, (uint32_t)-1);
    for(int i = 0; i < mineCount; i++)
    {
        int index;
        do
        {
            index = ran() % boardSize1D;
        } while (Index(x, y) == index || board[index].isBomb);
        board[index].isBomb = true;
    }


    // Iterate through each tile and count the number of bombs that surround it
    for (int x = 0; x < boardSize2D.x; x++)
    {
        for (int y = 0; y < boardSize2D.y; y++)
        {
            int curIndex = Index(x, y);

            glm::ivec2 nexIndex;

            countBombs({x,y}, {x,y+1});
            countBombs({x,y}, { x + 1, y + 1 });
            countBombs({x,y}, { x + 1, y });
            countBombs({x,y}, { x + 1, y - 1 });
            countBombs({x,y}, { x, y - 1 });
            countBombs({x,y}, { x - 1, y - 1 });
            countBombs({x,y}, { x - 1, y });
            countBombs({x,y}, { x - 1, y + 1 });
        }
    }
}

void Board::countBombs(const glm::ivec2& currentIndex, const glm::ivec2& nextIndex)
{
    // Bounds checking
    if (nextIndex.x >= 0 && nextIndex.y >= 0 && nextIndex.x < boardSize2D.x && nextIndex.y < boardSize2D.y)
    {
        // If the bomb specified by nextIndex is a bomb then add one the tile specified by currentIndex's bomb count
        board[Index(currentIndex.x, currentIndex.y)].bombs += board[Index(nextIndex.x, nextIndex.y)].isBomb;    
    }
}