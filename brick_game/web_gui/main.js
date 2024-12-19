import {applyRootStyles} from "./src/utils.js";
import {rootStyles} from "./src/config.js";
import {GameBoard} from "./src/game-board.js";
import {NextBoard} from "./src/next-board.js";
import {updateCurrentState} from "./src/update-current-state.js";
import {userInput} from "./src/user-input.js";

let intervalId;
const gameBoard = new GameBoard(document.querySelector('#game-board'));
const nextBoard = new NextBoard(document.querySelector("#next-board"));

let keysPressed = {};

async function executeUserInput(event) {
    const key = event.key.toLowerCase();

    const isKeyPressed = keysPressed[event.code] || false;
    if (event.type === 'keydown') {
        keysPressed[event.code] = true;
    }

    if (event.code === 'Enter') {
        console.log('Enter pressed');
        await userInput(0, isKeyPressed);
    } else if (event.code === 'Escape') {
        console.log('Escape pressed');

        document.getElementById('game-container').style.display = 'none';
        document.getElementById('game-buttons').style.display = 'block';

        document.getElementById('hi-score').textContent = `Hi-Score:`;
        document.getElementById('score').textContent = `Score:`;
        document.getElementById('level').textContent = `Level:`;
        document.getElementById('speed').textContent = `Speed:`;
        document.querySelector("#next-board").style.display = 'none';

        document.removeEventListener('keydown', executeUserInput);
        document.removeEventListener('keyup', keyUpHandler);

        gameBoard.clean();
        nextBoard.clean();

        clearInterval(intervalId);

        await userInput(2, isKeyPressed);
    } else if (event.code === 'Space') {
        console.log('Space pressed');
        await userInput(7, isKeyPressed);
    } else if (key === 'p') {
        console.log('P pressed');
        await userInput(1, isKeyPressed);
    } else if (event.code === 'ArrowUp') {
        console.log('Arrow Up pressed');
        await userInput(5, isKeyPressed);
    } else if (event.code === 'ArrowDown') {
        console.log('Arrow Down pressed');
        await userInput(6, isKeyPressed);
    } else if (event.code === 'ArrowLeft') {
        console.log('Arrow Left pressed');
        await userInput(3, isKeyPressed);
    } else if (event.code === 'ArrowRight') {
        console.log('Arrow Right pressed');
        await userInput(4, isKeyPressed);
    }
    console.log(isKeyPressed);
}

function keyUpHandler(event) {
  keysPressed[event.code] = false;
}


export async function loadGames() {
    try {
        const response = await fetch('/api/games');
        if (!response.ok) {
            throw new Error(response.json().message);
        }

        const games = await response.json();
        const buttonsDiv = document.getElementById('game-buttons');

        buttonsDiv.innerHTML = '';

        games.forEach(game => {
            const button = document.createElement('button');
            button.textContent = game.name;
            button.addEventListener('click', () => loadGame(game));
            buttonsDiv.appendChild(button);
        });
    } catch (error) {
        console.error('Ошибка загрузки игр:', error.message);
    }
}

async function loadGame(gameNameId) {
    try {
        const response = await fetch(`/api/games/${gameNameId.id}`, {method: 'POST'}); // Замените на ваш маршрут API
        if (!response.ok) {
            const gameData = await response.json();
            throw new Error(gameData.message);
        }
        document.getElementById('game-container').style.display = 'flex';
        document.getElementById('game-buttons').style.display = 'none';

        if (gameNameId.name === 'Tetris') {
          document.querySelector("#next-board").style.display = 'flex';
        }

        document.getElementById('hi-score').textContent = `Hi-Score:`;
        document.getElementById('score').textContent = `Score:`;
        document.getElementById('level').textContent = `Level:`;
        document.getElementById('speed').textContent = `Speed:`;

        intervalId = setInterval(async () => {
            const gameState = await updateCurrentState();
            const statusElement = document.getElementById('status');
            statusElement.style.color = '';

            if (gameState.pause) {
              statusElement.textContent = 'Pause. Press P to unpause or ESC to exit';
              statusElement.style.color = 'red';
          } else if (gameState.level === -1) {
              statusElement.textContent = 'Press ENTER to start or ESC to exit';
              statusElement.style.color = 'red';
          } else if (gameState.level === -2) {
              statusElement.textContent = 'Game Over. Press ENTER to restart or ESC to exit';
              statusElement.style.color = 'red';
          } else if (gameState.pause) {
              statusElement.textContent = 'Pause. Press P to unpause or ESC to exit';
              statusElement.style.color = 'red';
          } else if (gameState.level > 0) {
            statusElement.textContent = '';
            gameBoard.render(gameState.field);
            if (gameNameId.name === 'Tetris') {
              nextBoard.render(gameState.next);
            }
            updateSidePanel(gameState);
          }
        }, 50);
        document.addEventListener('keydown', executeUserInput);
        document.addEventListener('keyup', keyUpHandler);
    } catch (error) {
        console.error('Ошибка загрузки игры:', error.message);
    }
}

function updateSidePanel(stats) {
    document.getElementById('hi-score').textContent = `Hi-Score: ${stats.high_score}`;
    document.getElementById('score').textContent = `Score: ${stats.score}`;
    document.getElementById('level').textContent = `Level: ${stats.level}`;
    document.getElementById('speed').textContent = `Speed: ${stats.speed}`;
}


applyRootStyles(rootStyles);
document.addEventListener('DOMContentLoaded', loadGames);
