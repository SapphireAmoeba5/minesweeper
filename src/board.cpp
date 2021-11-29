#include <board.h>

#include <macros.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Random.h>

#include <vector>

// Quickly be able to render the board in order to debug
//#define DEBUG_RENDER


namespace Board
{
    // Local function declarations
    int Index(int x, int y);
    int Index(const glm::ivec2& index);
    void FirstClick(int x, int y);
    bool ValidIndex(int x, int y);
    bool ValidIndex(int index);
    bool ValidIndex(const glm::ivec2& index);
    uint32_t RecursivelyUncoverTiles(glm::ivec2 index);
    void UncoverTile(const glm::ivec2& index);
    std::vector<glm::ivec2> GetSurroundingCells(glm::ivec2 index);

    struct Tile
    {
        int bombs = 0;
        bool isBomb = false;
        bool isFlagged = false;
        bool isClicked = false;
    };

    // Local variable declarations
    glm::vec2 windowSize;
    glm::vec2 tileSize;
    glm::ivec2 boardSize2D;

    Tile* board;
    int boardSize1D;

    bool firstClick;
    bool failed;
    // The number of tiles that are safe to click on
    uint32_t safeCount;
    // If the player won the game yet
    bool won;

    uint32_t textureClicked[9];
    uint32_t sqbomb;
    uint32_t sqbombred;
    uint32_t sqflag;
    uint32_t sqflagwrong;
    uint32_t unclicked;
    uint32_t winner;
    // True when the board is first initialized. This is never set to false after
    bool texturesLoaded = false;

    // Function declarations
    void Init(const glm::ivec2& boardSize, const glm::vec2& iwindowSize)
    {
        boardSize2D = boardSize;
        boardSize1D = boardSize.x * boardSize.y;
        windowSize = iwindowSize;
        tileSize = { windowSize.x / boardSize2D.x, windowSize.y / boardSize2D.y };
        board = new Tile[boardSize1D];

        firstClick = true;
        failed = false;
        won = false;
        safeCount = 0;

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
            sqflagwrong = Renderer::LoadTexture("assets/textures/tiles/squareflagwrong.png");
            unclicked = Renderer::LoadTexture("assets/textures/tiles/unclicked.png");

            winner = Renderer::LoadTexture("assets/textures/winner.png");
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
        static float deltaTime = 0.0f;
        static float previousTime = glfwGetTime();

        // Calculate the tile size each frame
        windowSize = window.Size();
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


                if ((!board[index].isClicked || board[index].bombs != 0) && tileX == x && tileY == y && cursorPos.x >= 0 && cursorPos.y >= 0 && !failed && !won)
                    tint = { 0.80f, 0.80f, 0.80f, 1.0f };

                // Tint the squares surrounding the one being hovered if the right mouse button is held
                if (!won && !failed && board[Index(tileX, tileY)].isClicked && cursorPos.x >= 0 && cursorPos.y >= 0 && !board[index].isFlagged && !board[index].isClicked && (((tileY == y || tileY + 1 == y || tileY - 1 == y) && (tileX - 1 == x || tileX + 1 == x)) || ((tileX == x || tileX - 1 == x || tileX + 1 == x) && (tileY + 1 == y || tileY - 1 == y))) && glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
                    tint = { 0.50f, 0.50f, 0.50f, 1.0f };



                // The texture ID that will be rendered
                uint32_t id;

                // Find which texture ID to render with
                #ifdef DEBUG_RENDER
                if(board[index].isFlagged)
                    id = sqflag;
                else if(board[index].isBomb)
                    id = sqbomb;
                else if(board[index].isClicked == false)
                    id = unclicked;
                else
                    id = textureClicked[board[index].bombs];
                #else
                if (failed && !board[index].isBomb && board[index].isFlagged)
                    id = sqflagwrong;
                else if (board[index].isBomb && board[index].isClicked)
                    id = sqbombred;
                else if (board[index].isBomb && failed)
                    id = sqbomb;
                else if (board[index].isFlagged)
                    id = sqflag;
                else if (board[index].isClicked)
                    id = textureClicked[board[index].bombs];
                else if (!board[index].isClicked)
                    id = unclicked;
                #endif

                Renderer::DrawQuad(pos, tileSize, tint, id);
            }
        }

        static float alpha = 0.0f;
        if(won)
        {
            Renderer::DrawQuad({windowSize.x / 2, windowSize.y / 2}, {windowSize.x, windowSize.y}, {1.0f, 1.0f, 1.0f, alpha / 10.0f});
            Renderer::DrawQuad({windowSize.x / 2, 800.0f / 2}, {800.0f, 800.0f}, {1.0f, 1.0f, 1.0f, alpha}, winner);
            
            if(alpha < 1.0f)
                alpha += 1.0f * deltaTime;
        }
        else
            alpha = 0.0f;

        deltaTime = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();
    }

    /*
        This function is called in minesweeper.cpp whenever the left or right mouse button is clicked
    */
    void Click(const glm::vec2& pos, Action action)
    {
        // The x and y index if the tile that was clicked
        int x = pos.x / tileSize.x;
        int y = pos.y / tileSize.y;

        // If failed or the mouse click is out of bounds then exit the function
        if (won || failed || x < 0 || y < 0 || x >= boardSize2D.x || y >= boardSize2D.y)
            return;

        int index = Index(x, y);

        if (action == Action::LeftClick)
        {
            if (firstClick)
            {
                FirstClick(x, y);
                firstClick = false;
            }

            UncoverTile({ x,y });
        }
        else if (action == Action::RightClick)
        {
            // Automatically uncovers all surrounding tiles if the flag count is equal to the bomb count
            if(board[index].bombs != 0 && board[index].isClicked)
            {
                std::vector<glm::ivec2> indicies = GetSurroundingCells({x,y});

                std::vector<glm::ivec2> unflagged;
                int flagCount = 0;

                for(int i = 0; i < indicies.size(); i++)
                {
                    int curIndex = Index(indicies[i]);

                    if(!board[curIndex].isFlagged)
                        unflagged.push_back(indicies[i]);
                    else
                        flagCount++;
                }

                if(flagCount == board[index].bombs)
                {
                    for(int i = 0; i < unflagged.size(); i++)
                    {
                        UncoverTile(unflagged[i]);
                    }
                }
            }
            else if (!board[index].isClicked)
                board[index].isFlagged = !board[index].isFlagged;
        }

    }

    bool GetFailStatus()
    {
        return failed;
    }
    bool GetWinStatus()
    {
        return won;
    }

    
}


// LOCAL FUNCTION DECLARATIONS

/*
    Returns 1D index when given 2D index
*/
int Board::Index(int x, int y)
{
    return y * boardSize2D.x + x;
}

int Board::Index(const glm::ivec2& index)
{
    return index.y * boardSize2D.x + index.x;
}


bool Board::ValidIndex(int x, int y)
{
    return x >= 0 && y >= 0 && x < boardSize2D.x && y < boardSize2D.y;
}
bool Board::ValidIndex(int index)
{
    return index >= 0 && index < boardSize1D;
}
bool Board::ValidIndex(const glm::ivec2& index)
{
    return index.x >= 0 && index.y >= 0 && index.x < boardSize2D.x && index.y < boardSize2D.y;
}

// Initializes the board when it is clicked for the first time
void Board::FirstClick(int x, int y)
{
    Random ran;

    int mineCount = 40;
    safeCount = boardSize1D - mineCount;
    for (int i = 0; i < mineCount; i++)
    {
        // Generates the random index and verifies it
        glm::ivec2 index;
        do
        {
            index.x = ran() % boardSize2D.x; // TODO OPTIMIZE THIS  
            index.y = ran() % boardSize2D.y;
        } while (Index(x, y) == Index(index.x, index.y) || board[Index(index)].isBomb || board[Index(index)].isClicked);
        board[Index(index)].isBomb = true;
    
        // Adds one to the bomb count of all surrounding tiles
        std::vector<glm::ivec2> indicies = GetSurroundingCells(index);

        for(int i = 0; i < indicies.size(); i++)
        {
            board[Index(indicies[i])].bombs++;
        }
    }

}

// Uncovers the bomb at the index given.
void Board::UncoverTile(const glm::ivec2& index)
{
    int index1D = Index(index.x, index.y);

    // Return if the tile is already clicked or flagged
    if(board[index1D].isClicked == true || board[index1D].isFlagged == true)
        return;

    safeCount--;

    if (board[index1D].isBomb)
    {
        failed = true;
        board[index1D].isClicked = true;
    }
    else if (!board[index1D].isBomb && board[index1D].bombs == 0)
    {
        safeCount -= RecursivelyUncoverTiles({ index.x,index.y });
    }
    else if (!board[index1D].isBomb)
    {
        board[index1D].isClicked = true;
    }

    // If safeCount is 0 then set won to true
    won = (safeCount == 0 && !failed);
}

// Returns everything surrounding the parameter. Each index is always a valid index
std::vector<glm::ivec2> Board::GetSurroundingCells(glm::ivec2 index)
{
    std::vector<glm::ivec2> indicies;

    index = {index.x, index.y + 1};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x+1, index.y};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x, index.y-1};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x, index.y-1};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x-1, index.y};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x-1, index.y};
    if(ValidIndex(index))
        indicies.push_back(index);

    index = {index.x, index.y+1};
    if(ValidIndex(index))
        indicies.push_back(index);

   index = {index.x, index.y+1};
    if(ValidIndex(index))
        indicies.push_back(index);

    return indicies;
}

/*
    Uncovers the tiles surrounding the index given,
    but if the bomb being removed also has a bomb count of 0,
    it will also remove the tiles surrounding that tile
*/
uint32_t Board::RecursivelyUncoverTiles(glm::ivec2 index)
{
    uint32_t uncoveredTiles = 0;
    std::vector<glm::ivec2> indicies = GetSurroundingCells(index);

    board[Index(index.x, index.y)].isClicked = true;

    for(int i = 0; i < indicies.size(); i++)
    {
        int index = Index(indicies[i].x, indicies[i].y);

        if(board[index].isClicked == false)
            uncoveredTiles++;

        if (!board[index].isClicked && board[index].bombs == 0) { uncoveredTiles += RecursivelyUncoverTiles(indicies[i]); }

        board[index].isFlagged = false;
        board[index].isClicked = true;
    }

    return uncoveredTiles;
}