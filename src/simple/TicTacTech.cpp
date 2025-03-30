#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

const int CELL_SIZE = 100;
const int BOARD_SIZE = 3;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

enum class CellState
{
	Empty,
	X,
	O
};
enum class GameState
{
	Playing,
	XWins,
	OWins,
	Draw
};
enum class Theme
{
	Classic,
	Dark,
	Colorful
};

class TicTacToe
{
private:
	std::vector<std::vector<CellState>> board;
	CellState currentPlayer;
	GameState gameState;
	Theme currentTheme;

	sf::Texture xTexture;
	sf::Texture oTexture;
	sf::Texture xDarkTexture;
	sf::Texture oDarkTexture;
	sf::Texture xColorTexture;
	sf::Texture oColorTexture;
	sf::Texture backgroundTexture;
	sf::Texture buttonTexture;
	sf::Texture buttonHoverTexture;
	sf::Texture buttonPressedTexture;

	sf::Sprite backgroundSprite;

	sf::SoundBuffer clickBuffer;
	sf::SoundBuffer hoverBuffer;
	sf::SoundBuffer moveXBuffer;
	sf::SoundBuffer moveOBuffer;
	sf::SoundBuffer winBuffer;
	sf::SoundBuffer drawBuffer;

	sf::Sound clickSound;
	sf::Sound hoverSound;
	sf::Sound moveXSound;
	sf::Sound moveOSound;
	sf::Sound winSound;
	sf::Sound drawSound;

	bool loadResources()
	{
		// Load textures
		if (!xTexture.loadFromFile("../resources/images/x.png") ||
				!oTexture.loadFromFile("../resources/images/o.png") ||
				!xDarkTexture.loadFromFile("../resources/images/x_dark.png") ||
				!oDarkTexture.loadFromFile("../resources/images/o_dark.png") ||
				!xColorTexture.loadFromFile("../resources/images/x_color.png") ||
				!oColorTexture.loadFromFile("../resources/images/o_color.png") ||
				!backgroundTexture.loadFromFile("../resources/images/background.png") ||
				!buttonTexture.loadFromFile("../resources/images/button.png") ||
				!buttonHoverTexture.loadFromFile("../resources/images/button_hover.png") ||
				!buttonPressedTexture.loadFromFile("../resources/images/button_pressed.png"))
		{
			std::cerr << "Failed to load textures" << std::endl;
			return false;
		}

		// Load sounds
		if (!clickBuffer.loadFromFile("../resources/sounds/button_click.wav") ||
				!hoverBuffer.loadFromFile("../resources/sounds/button_hover.wav") ||
				!moveXBuffer.loadFromFile("../resources/sounds/x_move.wav") ||
				!moveOBuffer.loadFromFile("../resources/sounds/o_move.wav") ||
				!winBuffer.loadFromFile("../resources/sounds/win.wav") ||
				!drawBuffer.loadFromFile("../resources/sounds/draw.wav"))
		{
			std::cerr << "Failed to load sounds" << std::endl;
			return false;
		}

		clickSound.setBuffer(clickBuffer);
		hoverSound.setBuffer(hoverBuffer);
		moveXSound.setBuffer(moveXBuffer);
		moveOSound.setBuffer(moveOBuffer);
		winSound.setBuffer(winBuffer);
		drawSound.setBuffer(drawBuffer);

		backgroundSprite.setTexture(backgroundTexture);

		return true;
	}

	void checkGameState()
	{
		// Check rows
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[i][0] != CellState::Empty &&
					board[i][0] == board[i][1] &&
					board[i][1] == board[i][2])
			{
				gameState = (board[i][0] == CellState::X) ? GameState::XWins : GameState::OWins;
				winSound.play();
				return;
			}
		}

		// Check columns
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[0][i] != CellState::Empty &&
					board[0][i] == board[1][i] &&
					board[1][i] == board[2][i])
			{
				gameState = (board[0][i] == CellState::X) ? GameState::XWins : GameState::OWins;
				winSound.play();
				return;
			}
		}

		// Check diagonals
		if (board[0][0] != CellState::Empty &&
				board[0][0] == board[1][1] &&
				board[1][1] == board[2][2])
		{
			gameState = (board[0][0] == CellState::X) ? GameState::XWins : GameState::OWins;
			winSound.play();
			return;
		}

		if (board[0][2] != CellState::Empty &&
				board[0][2] == board[1][1] &&
				board[1][1] == board[2][0])
		{
			gameState = (board[0][2] == CellState::X) ? GameState::XWins : GameState::OWins;
			winSound.play();
			return;
		}

		// Check for draw
		bool isBoardFull = true;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (board[i][j] == CellState::Empty)
				{
					isBoardFull = false;
					break;
				}
			}
			if (!isBoardFull)
				break;
		}

		if (isBoardFull)
		{
			gameState = GameState::Draw;
			drawSound.play();
		}
	}

public:
	TicTacToe() : currentPlayer(CellState::X), gameState(GameState::Playing), currentTheme(Theme::Classic)
	{
		// Initialize the board
		board.resize(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty));

		if (!loadResources())
		{
			throw std::runtime_error("Failed to load game resources");
		}
	}

	void reset()
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				board[i][j] = CellState::Empty;
			}
		}
		currentPlayer = CellState::X;
		gameState = GameState::Playing;
	}

	bool makeMove(int row, int col)
	{
		if (gameState != GameState::Playing || row < 0 || row >= BOARD_SIZE ||
				col < 0 || col >= BOARD_SIZE || board[row][col] != CellState::Empty)
		{
			return false;
		}

		board[row][col] = currentPlayer;

		if (currentPlayer == CellState::X)
		{
			moveXSound.play();
		}
		else
		{
			moveOSound.play();
		}

		checkGameState();

		if (gameState == GameState::Playing)
		{
			currentPlayer = (currentPlayer == CellState::X) ? CellState::O : CellState::X;
		}

		return true;
	}

	void changeTheme()
	{
		switch (currentTheme)
		{
		case Theme::Classic:
			currentTheme = Theme::Dark;
			break;
		case Theme::Dark:
			currentTheme = Theme::Colorful;
			break;
		case Theme::Colorful:
			currentTheme = Theme::Classic;
			break;
		}
		clickSound.play();
	}

	void draw(sf::RenderWindow &window)
	{
		// Draw background
		window.draw(backgroundSprite);

		// Draw board grid
		float boardStartX = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2.0f;
		float boardStartY = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2.0f;

		// Draw cells
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
				cell.setPosition(boardStartX + j * CELL_SIZE, boardStartY + i * CELL_SIZE);
				cell.setFillColor(sf::Color(30, 30, 30, 150));
				cell.setOutlineThickness(2);
				cell.setOutlineColor(sf::Color(100, 100, 100));
				window.draw(cell);

				if (board[i][j] != CellState::Empty)
				{
					sf::Sprite marker;

					if (board[i][j] == CellState::X)
					{
						switch (currentTheme)
						{
						case Theme::Classic:
							marker.setTexture(xTexture);
							break;
						case Theme::Dark:
							marker.setTexture(xDarkTexture);
							break;
						case Theme::Colorful:
							marker.setTexture(xColorTexture);
							break;
						}
					}
					else
					{
						switch (currentTheme)
						{
						case Theme::Classic:
							marker.setTexture(oTexture);
							break;
						case Theme::Dark:
							marker.setTexture(oDarkTexture);
							break;
						case Theme::Colorful:
							marker.setTexture(oColorTexture);
							break;
						}
					}

					marker.setPosition(boardStartX + j * CELL_SIZE, boardStartY + i * CELL_SIZE);
					window.draw(marker);
				}
			}
		}

		// Draw game state
		sf::Text text;
		sf::Font font;
		// Use system default font as fallback if custom font loading fails
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		std::string message;
		switch (gameState)
		{
		case GameState::Playing:
			message = "Current Player: " + std::string(currentPlayer == CellState::X ? "X" : "O");
			break;
		case GameState::XWins:
			message = "X Wins!";
			break;
		case GameState::OWins:
			message = "O Wins!";
			break;
		case GameState::Draw:
			message = "Draw!";
			break;
		}

		// Draw UI buttons
		sf::Sprite resetButton;
		resetButton.setTexture(buttonTexture);
		resetButton.setPosition(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 70);
		window.draw(resetButton);

		sf::Sprite themeButton;
		themeButton.setTexture(buttonTexture);
		themeButton.setPosition(50, WINDOW_HEIGHT - 70);
		window.draw(themeButton);

		// Draw game info
		text.setString("TicTacTech Game");
		text.setPosition(WINDOW_WIDTH / 2 - 80, 20);
		window.draw(text);

		text.setString(message);
		text.setPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT - 50);
		window.draw(text);

		text.setString("Reset");
		text.setPosition(WINDOW_WIDTH - 130, WINDOW_HEIGHT - 60);
		window.draw(text);

		text.setString("Theme");
		text.setPosition(70, WINDOW_HEIGHT - 60);
		window.draw(text);
	}

	bool isResetButtonClicked(int x, int y)
	{
		return (x >= WINDOW_WIDTH - 150 && x <= WINDOW_WIDTH - 150 + 100 &&
						y >= WINDOW_HEIGHT - 70 && y <= WINDOW_HEIGHT - 70 + 50);
	}

	bool isThemeButtonClicked(int x, int y)
	{
		return (x >= 50 && x <= 50 + 100 &&
						y >= WINDOW_HEIGHT - 70 && y <= WINDOW_HEIGHT - 70 + 50);
	}

	bool isCellClicked(int x, int y, int &row, int &col)
	{
		float boardStartX = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2.0f;
		float boardStartY = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2.0f;

		if (x < boardStartX || y < boardStartY ||
				x >= boardStartX + BOARD_SIZE * CELL_SIZE ||
				y >= boardStartY + BOARD_SIZE * CELL_SIZE)
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
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TicTacTech Game");
	window.setFramerateLimit(60);

	try
	{
		TicTacToe game;

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

						int row, col;
						if (game.isCellClicked(mouseX, mouseY, row, col))
						{
							game.makeMove(row, col);
						}
						else if (game.isResetButtonClicked(mouseX, mouseY))
						{
							game.reset();
						}
						else if (game.isThemeButtonClicked(mouseX, mouseY))
						{
							game.changeTheme();
						}
					}
				}
			}

			window.clear(sf::Color(20, 20, 20));
			game.draw(window);
			window.display();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}