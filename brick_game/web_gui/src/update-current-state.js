export async function updateCurrentState() {
    const url = "/api/state";

    try {
        const response = await fetch(url);

        if (!response.ok) {
            const error = await response.json();
            console.error("Ошибка:", error.message || "Неизвестная ошибка");
        }

        const gameState = await response.json();
        // updateBoard(gameState);
        // console.log("Текущее состояние игры:", gameState);
        return gameState;
    } catch (error) {
        console.error("Ошибка сети:", error.message);
    }
}
