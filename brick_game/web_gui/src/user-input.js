export async function userInput(action_id, hold) {
    const url = "/api/actions";
    const body = JSON.stringify({ action_id, hold });
    console.log(body);

    try {
        const response = await fetch(url, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: body,
        });

        if (!response.ok) {
            const error = await response.json();
            console.error("Ошибка:", error.message || "Неизвестная ошибка");
        }

        console.log("Действие успешно выполнено");
    } catch (error) {
        console.error("Ошибка сети:", error.message);
    }
}
