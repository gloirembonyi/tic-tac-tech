const boardElement = document.getElementById('board');
const currentPlayerEl = document.getElementById('current-player');
const scoreXEl = document.getElementById('score-x');
const scoreOEl = document.getElementById('score-o');
const resetBtn = document.getElementById('reset-btn');
const modeBtn = document.getElementById('mode-btn');

const modal = document.getElementById('modal');
const modalTitle = document.getElementById('modal-title');
const modalMessage = document.getElementById('modal-message');
const modalCloseBtn = document.getElementById('modal-close');

let board = ['', '', '', '', '', '', '', '', ''];
let currentPlayer = 'X';
let gameActive = true;
let scores = { X: 0, O: 0 };
let playAgainstAI = false;
let isAITurn = false;

const winningConditions = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8], // Rows
    [0, 3, 6], [1, 4, 7], [2, 5, 8], // Columns
    [0, 4, 8], [2, 4, 6]             // Diagonals
];

// Initialize Board
function initBoard() {
    boardElement.innerHTML = '';
    board.forEach((cell, index) => {
        const cellElement = document.createElement('div');
        cellElement.classList.add('cell');
        cellElement.dataset.index = index;
        cellElement.addEventListener('click', handleCellClick);
        boardElement.appendChild(cellElement);
    });
}

function handleCellClick(e) {
    if (isAITurn) return; // Prevent clicks while AI is thinking

    const intIndex = parseInt(e.target.dataset.index);
    if (board[intIndex] !== '' || !gameActive) return;

    makeMove(intIndex, currentPlayer);

    if (gameActive && playAgainstAI && currentPlayer === 'O') {
        isAITurn = true;
        setTimeout(makeAIMove, 500); // Small delay for realism
    }
}

function makeMove(index, player) {
    board[index] = player;
    
    // Update DOM
    const cell = boardElement.children[index];
    cell.textContent = player;
    cell.classList.add(player.toLowerCase());

    if (checkWin(player)) {
        handleWin(player);
    } else if (board.indexOf('') === -1) {
        handleDraw();
    } else {
        // Toggle Player
        currentPlayer = currentPlayer === 'X' ? 'O' : 'X';
        updateTurnDisplay();
    }
}

function checkWin(player) {
    for (const condition of winningConditions) {
        if (board[condition[0]] === player &&
            board[condition[1]] === player &&
            board[condition[2]] === player) {
            
            // Highlight winning cells
            condition.forEach(index => {
                boardElement.children[index].classList.add('winning-cell');
            });
            return true;
        }
    }
    return false;
}

function handleWin(player) {
    gameActive = false;
    scores[player]++;
    
    // Update Score
    if(player === 'X') scoreXEl.textContent = scores['X'];
    else scoreOEl.textContent = scores['O'];
    
    showModal(`${player} Wins!`, `Player ${player} has dominated the board.`, true);
}

function handleDraw() {
    gameActive = false;
    showModal('Draw!', 'A fierce battle with no victor.', false);
}

function updateTurnDisplay() {
    currentPlayerEl.textContent = currentPlayer;
    currentPlayerEl.className = `player-${currentPlayer.toLowerCase()}`;
}

function resetGame() {
    board = ['', '', '', '', '', '', '', '', ''];
    currentPlayer = 'X';
    gameActive = true;
    isAITurn = false;
    updateTurnDisplay();
    initBoard();
    if(playAgainstAI && currentPlayer === 'O') {
       isAITurn = true;
       setTimeout(makeAIMove, 500); 
    }
}

function showModal(title, message, isWin) {
    setTimeout(() => {
        modalTitle.textContent = title;
        if(isWin) {
            modalTitle.style.background = currentPlayer === 'X' ? 
                'linear-gradient(135deg, #ef4444 0%, #fca5a5 100%)' : 
                'linear-gradient(135deg, #0ea5e9 0%, #7dd3fc 100%)';
            modalTitle.style.webkitBackgroundClip = 'text';
            modalTitle.style.backgroundClip = 'text';
            modalTitle.style.webkitTextFillColor = 'transparent';
        } else {
            modalTitle.style.background = 'linear-gradient(135deg, #f8fafc 0%, #cbd5e1 100%)';
            modalTitle.style.webkitBackgroundClip = 'text';
            modalTitle.style.backgroundClip = 'text';
            modalTitle.style.webkitTextFillColor = 'transparent';
        }
        modalMessage.textContent = message;
        modal.classList.remove('hidden');
    }, 600); // Wait for winning animation to finish
}

function hideModal() {
    modal.classList.add('hidden');
    resetGame();
}

function toggleMode() {
    playAgainstAI = !playAgainstAI;
    modeBtn.textContent = `vs AI: ${playAgainstAI ? 'ON' : 'OFF'}`;
    modeBtn.classList.toggle('active');
    resetGame();
}

// Simple AI - Plays random empty spot but blocks/wins perfectly
function makeAIMove() {
    isAITurn = false;
    if (!gameActive) return;
    
    const bestMove = getBestMove();
    if(bestMove !== null) {
        makeMove(bestMove, 'O');
    }
}

function getBestMove() {
    // 1. Check for win
    for (let i = 0; i < 9; i++) {
        if (board[i] === '') {
            board[i] = 'O';
            if (checkWinSimulate('O')) { board[i] = ''; return i; }
            board[i] = '';
        }
    }
    
    // 2. Check for block
    for (let i = 0; i < 9; i++) {
        if (board[i] === '') {
            board[i] = 'X';
            if (checkWinSimulate('X')) { board[i] = ''; return i; }
            board[i] = '';
        }
    }
    
    // 3. Take center
    if(board[4] === '') return 4;
    
    // 4. Random empty edge/corner
    const emptyC = [0, 2, 6, 8, 1, 3, 5, 7].filter(i => board[i] === '');
    if(emptyC.length > 0) return emptyC[Math.floor(Math.random() * emptyC.length)];
    
    return null;
}

function checkWinSimulate(player) {
    for (const c of winningConditions) {
        if (board[c[0]] === player && board[c[1]] === player && board[c[2]] === player) return true;
    }
    return false;
}

// Event Listeners
resetBtn.addEventListener('click', resetGame);
modeBtn.addEventListener('click', toggleMode);
modalCloseBtn.addEventListener('click', hideModal);

// Init
initBoard();
