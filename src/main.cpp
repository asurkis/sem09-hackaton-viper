#include "raylib.h"
#include <stdio.h>

int main(int argc, char **argv) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "Example window");

  Texture2D tex;
  if (argc > 1) {
    tex = LoadTexture(argv[1]);
  } else {
    Image img = GenImageColor(32, 32, LIME);
    tex = LoadTextureFromImage(img);
    UnloadImage(img);
  }

  int scale = 8;
  int cursorX = 0;
  int cursorY = 0;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_H)) {
      cursorX--;
    }
    if (IsKeyPressed(KEY_J)) {
      cursorY++;
    }
    if (IsKeyPressed(KEY_K)) {
      cursorY--;
    }
    if (IsKeyPressed(KEY_L)) {
      cursorX++;
    }
    if (cursorX < 0) {
      cursorX = 0;
    }
    if (cursorX >= tex.width) {
      cursorX = tex.width - 1;
    }
    if (cursorY < 0) {
      cursorY = 0;
    }
    if (cursorY >= tex.height) {
      cursorY = tex.height - 1;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    int imgLeft = (w - scale * tex.width) / 2;
    int imgTop = (h - scale * tex.height) / 2;
    DrawTextureEx(tex, Vector2{(float)imgLeft, (float)imgTop}, 0.0f, scale,
                  WHITE);
    DrawRectangleLines(imgLeft + scale * cursorX - 1,
                       imgTop + scale * cursorY - 1, scale + 2, scale + 2,
                       LIGHTGRAY);
    DrawRectangleLines(imgLeft + scale * cursorX - 2,
                       imgTop + scale * cursorY - 2, scale + 4, scale + 4,
                       GRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
