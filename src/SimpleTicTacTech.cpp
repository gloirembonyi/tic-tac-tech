#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int BOARD_SIZE = 3;
const int CELL_SIZE = 100;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

enum class Player
{
    None,
    X,
    O
};

class TicTacToeGame
{
private:
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    bool gameOver;
    sf::Font font;

public:
    TicTacToeGame() : currentPlayer(Player::X), gameOver(false)
    {
        // Initialize the 3x3 board
        board.resize(BOARD_SIZE, std::vector<Player>(BOARD_SIZE, Player::None));

        // Load font - using system font for simplicity
        if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc"))
        {
            std::cout << "Error loading font" << std::endl;
        }
    }

    void reset()
    {
        // Reset the board
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                board[i][j] = Player::None;
            }
        }
        currentPlayer = Player::X;
        gameOver = false;
    }

    bool makeMove(int row, int col)
    {
        if (gameOver || row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != Player::None)
        {
            return false;
        }

        board[row][col] = currentPlayer;

        // Check for win or draw
        if (checkWin())
        {
            gameOver = true;
            return true;
        }

        if (checkDraw())
        {
            gameOver = true;
            return true;
        }

        // Switch player
        currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
        return true;
    }

    bool checkWin()
    {
        // Check rows
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            if (board[i][0] != Player::None && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            {
                return true;
            }
        }

        // Check columns
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            if (board[0][i] != Player::None && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            {
                return true;
            }
        }

        // Check diagonals
        if (board[0][0] != Player::None && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        {
            return true;
        }

        if (board[0][2] != Player::None && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        {
            return true;
        }

        return false;
    }

    bool checkDraw()
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == Player::None)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void render(sf::RenderWindow &window)
    {
        // Draw the board
        float boardStartX = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2.0f;
        float boardStartY = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2.0f;

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(boardStartX + j * CELL_SIZE, boardStartY + i * CELL_SIZE);
                cell.setFillColor(sf::Color(50, 50, 50));
                cell.setOutlineColor(sf::Color(100, 100, 100));
                cell.setOutlineThickness(2);
                window.draw(cell);

                // Draw X or O
                if (board[i][j] != Player::None)
                {
                    sf::Text symbol;
                    symbol.setFont(font);
                    symbol.setString(board[i][j] == Player::X ? "X" : "O");
                    symbol.setCharacterSize(72);
                    symbol.setFillColor(board[i][j] == Player::X ? sf::Color::Red : sf::Color::Blue);

                    // Center the text in the cell
                    sf::FloatRect bounds = symbol.getLocalBounds();
                    symbol.setPosition(
                        boardStartX + j * CELL_SIZE + (CELL_SIZE - bounds.width) / 2,
                        boardStartY + i * CELL_SIZE + (CELL_SIZE - bounds.height) / 2 - 10);

                    window.draw(symbol);
                }
            }
        }

        // Draw status text
        sf::Text statusText;
        statusText.setFont(font);
        statusText.setCharacterSize(24);

        if (gameOver)
        {
            if (checkDraw())
            {
                statusText.setString("Game Over: Draw!");
                statusText.setFillColor(sf::Color::White);
            }
            else
            {
                std::string winner = (currentPlayer == Player::X) ? "X" : "O";
                statusText.setString("Game Over: " + winner + " Wins!");
                statusText.setFillColor((currentPlayer == Player::X) ? sf::Color::Red : sf::Color::Blue);
            }
        }
        else
        {
            std::string player = (currentPlayer == Player::X) ? "X" : "O";
            statusText.setString("Current Player: " + player);
            statusText.setFillColor((currentPlayer == Player::X) ? sf::Color::Red : sf::Color::Blue);
        }

        // Center the text at the bottom
        sf::FloatRect bounds = statusText.getLocalBounds();
        statusText.setPosition((WINDOW_WIDTH - bounds.width) / 2, WINDOW_HEIGHT - 50);

        window.draw(statusText);

        // Draw reset button
        sf::RectangleShape resetButton(sf::Vector2f(100, 40));
        resetButton.setPosition(WINDOW_WIDTH - 120, 20);
        resetButton.setFillColor(sf::Color(60, 60, 100));
        resetButton.setOutlineColor(sf::Color(100, 100, 150));
        resetButton.setOutlineThickness(2);
        window.draw(resetButton);

        sf::Text resetText;
        resetText.setFont(font);
        resetText.setString("Reset");
        resetText.setCharacterSize(20);
        resetText.setFillColor(sf::Color::White);
        bounds = resetText.getLocalBounds();
        resetText.setPosition(
            WINDOW_WIDTH - 120 + (100 - bounds.width) / 2,
            20 + (40 - bounds.height) / 2 - 5);
        window.draw(resetText);
    }

    bool isResetButtonClicked(int x, int y)
    {
        return (x >= WINDOW_WIDTH - 120 && x <= WINDOW_WIDTH - 20 &&
                y >= 20 && y <= 60);
    }

    bool isCellClicked(int x, int y, int &row, int &col)
    {
        float boardStartX = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2.0f;
        float boardStartY = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2.0f;

        if (x < boardStartX || x >= boardStartX + BOARD_SIZE * CELL_SIZE ||
            y < boardStartY || y >= boardStartY + BOARD_SIZE * CELL_SIZE)
        {
            return false;
        }

        col = (x - boardStartX) / CELL_SIZE;
        row = (y - boardStartY) / CELL_SIZE;
        return true;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TicTacToe Game");
    window.setFramerateLimit(60);

    TicTacToeGame game;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    // Handle reset button
                    if (game.isResetButtonClicked(mouseX, mouseY))
                    {
                        game.reset();
                    }
                    else
                    {
                        // Handle board click
                        int row, col;
                        if (game.isCellClicked(mouseX, mouseY, row, col))
                        {
                            game.makeMove(row, col);
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 40));
        game.render(window);
        window.display();
    }

    return 0;
}