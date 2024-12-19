from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
from game_router import game_router
import uvicorn
import os

app = FastAPI()


app.include_router(game_router, prefix="/api")


static_dir = os.path.abspath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "../web_gui")
)
app.mount("/static", StaticFiles(directory=static_dir, html=True), name="static")


@app.get("/")
def root():
    return FileResponse(static_dir + "/index.html")


if __name__ == "__main__":
    uvicorn.run("main:app", host="localhost", port=8080)
