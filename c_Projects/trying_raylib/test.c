#include "raylib.h"

int main(void) {
 int posX = 0;
 int posY = 0;
  InitWindow(800, 800, "raylib [core] example - basic window");
  SetTargetFPS(144);

  while (!WindowShouldClose()) {
   posX = posX + 1;
   posY = posY + 1;
    BeginDrawing();

      ClearBackground(BLACK);
      /* DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY); */
      DrawRectangle(posX, posY, 200, 200, RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
