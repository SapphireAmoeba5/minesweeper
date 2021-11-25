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
    bool ValidIndex(int x, int y);
    bool ValidIndex(int index);
    void RecursivelyUncoverTiles(glm::ivec2 index, int count = 0);
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

        if (!texturesLoaded)
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
            for (int i = 0; i < 9; i++)
            {
                if (textureClicked[i] == 0)
                {
                    DEBUG_WARN("Failure loading textures!");
                    loadFailure = true;
                    break;
                }
            }

            if (!loadFailure)
            {
                if (sqbomb == 0)
                    loadFailure = true;
                else if (sqbombred == 0)
                    loadFailure = true;
                else if (sqflag == 0)
                    loadFailure = true;
                else if (unclicked == 0)
                    loadFailure = true;

                if (loadFailure)
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

        glm::ivec2 cursorPos = window.CursorPos();

        // The tile the mouse is hovering over
        int tileX = cursorPos.x / tileSize.x;
        int tileY = cursorPos.y / tileSize.y;

        for (int x = 0; x < boardSize2D.x; x++)
        {
            for (int y = 0; y < boardSize2D.y; y++)
            {
                // The position of the current tile
                glm::vec2 pos = { tileSize.x * x + tileSize.x / 2, tileSize.y * y + tileSize.y / 2 };

                // The index of the current tile
                int index = Index(x, y);

                glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };



                if (tileX == x && tileY == y && cursorPos.x >= 0 && cursorPos.y >= 0 && !failed)
                    tint = { 0.80f, 0.80f, 0.80f, 1.0f };

                // Tint the squares surrounding the one being hovered if the middle mouse button is held
                if(!board[Index(x,y)].isClicked && (((tileY == y || tileY + 1 == y || tileY - 1 == y) && (tileX - 1 == x || tileX + 1 == x)) || ((tileX == x || tileX - 1 == x || tileX + 1 == x) && (tileY + 1 == y || tileY - 1 == y))) && glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
                    tint = {0.50f, 0.50f, 0.50f, 1.0f};
                

                // The texture ID that will be rendered
                uint32_t id;

                // Find which texture ID to render with
                if (board[index].isBomb && board[index].isClicked)
                    id = sqbombred;
                else if (board[index].isBomb && failed)
                    id = sqbomb;
                else if (board[index].isFlagged)
                    id = sqflag;
                else if (board[index].isClicked)
                    id = textureClicked[board[index].bombs];
                else if (!board[index].isClicked)
                    id = unclicked;

                Renderer::DrawQuad(pos, tileSize, tint, id);
            }
        }
    }

    void Click(const glm::vec2& pos, Action action)
    {
        // The x and y index if the tile that was clicked
        int x = pos.x / tileSize.x;
        int y = pos.y / tileSize.y;

        // If failed or the mouse click is out of bounds then exit the function
        if (failed || x < 0 || y < 0 || x >= boardSize2D.x || y >= boardSize2D.y)
            return;

        int index = Index(x, y);
        if (action == Action::LeftClick)
        {
            if (firstClick)
            {
                FirstClick(x, y);
                firstClick = false;
            }

            if (!board[index].isFlagged && board[index].isBomb)
            {
                failed = true;
                board[index].isClicked = true;
            }
            else if (!board[index].isFlagged && !board[index].isBomb && board[index].bombs == 0)
            {
                RecursivelyUncoverTiles({ x,y });
            }
            else if (!board[index].isFlagged && !board[index].isBomb)
            {
                board[index].isClicked = true;
            }
        }
        else if (action == Action::RightClick && !board[index].isClicked)
        {
            board[index].isFlagged = !board[index].isFlagged;
        }

    }

    bool GetFailStatus()
    {
        return failed;
    }

    bool ValidIndex(int x, int y)
    {
        return x >= 0 && y >= 0 && x < boardSize2D.x&& y < boardSize2D.y;
    }
    bool ValidIndex(int index)
    {
        return index >= 0 && index < boardSize1D;
    }
}


// LOCAL FUNCTION DECLARATIONS
int Board::Index(int x, int y)
{
    return y * boardSize2D.x + x;
}

// Initializes the board when it is clicked for the first time
void Board::FirstClick(int x, int y)
{
    Random ran;

    // The number of tiles that the game starts you off with
    int startingTiles = 1;
    for (int i = 0; i < startingTiles; i++)
    {

    }

    // Randomly place bombs on the board
    int mineCount = 40;
    for (int i = 0; i < mineCount; i++)
    {
        int index;
        do
        {
            index = ran() % boardSize1D;
        } while (Index(x, y) == index || board[index].isBomb || board[index].isClicked);
        board[index].isBomb = true;
    }


    // Iterate through each tile and count the number of bombs that surround it
    for (int x = 0; x < boardSize2D.x; x++)
    {
        for (int y = 0; y < boardSize2D.y; y++)
        {
            countBombs({ x,y }, { x,y + 1 });
            countBombs({ x,y }, { x + 1, y + 1 });
            countBombs({ x,y }, { x + 1, y });
            countBombs({ x,y }, { x + 1, y - 1 });
            countBombs({ x,y }, { x, y - 1 });
            countBombs({ x,y }, { x - 1, y - 1 });
            countBombs({ x,y }, { x - 1, y });
            countBombs({ x,y }, { x - 1, y + 1 });
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

void Board::RecursivelyUncoverTiles(glm::ivec2 index, int count /*= 0*/)
{
    board[Index(index.x, index.y)].isClicked = true;

    index = { index.x, index.y + 1 };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x + 1, index.y };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x, index.y - 1 };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x, index.y - 1 };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x - 1, index.y };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x - 1, index.y };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x, index.y + 1 };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }

    index = { index.x, index.y + 1 };
    if (ValidIndex(index.x, index.y))
    {
        if (!board[Index(index.x, index.y)].isClicked && board[Index(index.x, index.y)].bombs == 0) { RecursivelyUncoverTiles(index); }
        board[Index(index.x, index.y)].isClicked = true;
    }
}