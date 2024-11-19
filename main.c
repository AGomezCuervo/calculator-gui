#include "include/raylib.h"
#include "include/main.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "include/calc.h"

int input_length = 0;
char input[256] = "\0";
const char *text_buttons[] = {
  "7","8","9","DEL","AC",
  "4","5","6","x","/",
  "1","2","3","+","-",
  "0",".","^","%","=",
};

enum buttons {
  delete = 3,
  clear = 4,
  multiply = 8,
  divide = 9,
  plus = 13,
  minus = 14,
  module = 18,
  equal = 19,
};

int main()
{
  Calculator *calculator = init_calculator(10);
  InitWindow(WIDTH, HEIGHT, "minicalc");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  Font MonoLisa_regular = LoadFontEx("./fonts/MonoLisa-Regular.ttf", font_size, 0, 0);
  Font MonoLisa_title = LoadFontEx("./fonts/MonoLisa-Regular.ttf", title_size, 0, 0);
  Font MonoLisa_symbols = LoadFontEx("./fonts/MonoLisa-Regular.ttf", symbol_size, 0, 0);

  SetTargetFPS(60);

  // Set variables
  int w = WIDTH;
  int h = HEIGHT;
  float current_font_size = title_size;
  double result;
  float input_height = h*0.45;
  float button_padding = 5.0f;
  float button_width;
  float button_height;
  float default_char_width = MeasureTextEx(MonoLisa_title, "5", title_size, 0).x + 1;
  float current_input_length = default_char_width;
  float current_char_width;

  uint64_t offset = 1;

  // Half Top
  Rectangle preview_box = { 0 };
  Vector2 m_input_position = { 0 };

  // Half Bottom
  Rectangle input_box = {0};

  while (!WindowShouldClose()) {
    w = GetScreenWidth();
    h = GetScreenHeight();

    // Set sizes every frame
    current_char_width = MeasureTextEx(MonoLisa_title, "5", current_font_size, 0).x + 1;
    input_height = h*0.45;
    button_padding = 5.0f;
    button_width = (w - button_padding * 6)/5.0f;
    button_height = (input_height - button_padding * 5)/4.0f;
    preview_box.width = w;
    preview_box.height = h - input_height;

    m_input_position.x = w - (current_char_width*offset);
    m_input_position.y = preview_box.height - title_size;
    input_box.y = preview_box.height;
    input_box.width = w;
    input_box.height = input_height;

    //Start Drawing
    BeginDrawing();
    ClearBackground(solarized_bg);
    DrawTextEx(MonoLisa_title, input, m_input_position, current_font_size, 1, WHITE);

    // Buttons
    int actual_row = 0;
    int actual_column = 0;
    bool mouse_over = false;
    for (int i = 1; i <= 20; i++)
    {
      Rectangle button = {
        .x = (input_box.x) + actual_column *(button_width + button_padding) + button_padding,
        .y = (input_box.y) + actual_row *(button_height + button_padding) + button_padding,
        .width = button_width,
        .height = button_height,
      };

      if(CheckCollisionPointRec(GetMousePosition(), button))
      {
          mouse_over = true;
          if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
          {
            int button_pressed = i-1;

            // Manage buttons
            switch (button_pressed) {
              case delete: 
                if(input_length)
                {
                  current_input_length = MeasureTextEx(MonoLisa_title, input, current_font_size, 0).x + 1;
                  if(current_font_size < title_size)
                    current_font_size += 10;

                  --input_length;
                  input[input_length] = '\0';
                  --offset;
                }
                break;
              case clear:
                for (int j = 0; j < input_length; j++)
                {
                  input[j] = '\0'; 
                }
                current_input_length = default_char_width;
                input_length = 0;
                offset = 1;
                break;
              case equal:
                result = calculate_expr(calculator, input);
                sprintf(input, "%f", result);
                break;
              default:
                if(input_length < 255)
                {
                  current_input_length = MeasureTextEx(MonoLisa_title, input, current_font_size, 0).x + 1;
                  if(current_input_length >= (w - button_width))
                    current_font_size -= 10;

                  input[input_length] = *text_buttons[i - 1];
                  ++input_length;
                  input[input_length] = '\0';
                  ++offset;
                }
                break;
            }
            DrawRectangleRec(button, GRAY);
          } else {
            DrawRectangleRec(button, solarized_hr);
          }
      }
      else
      {
          DrawRectangleRec(button, solarized_fg);
      }

      ++actual_column;

      Vector2 text_size = MeasureTextEx(MonoLisa_regular, text_buttons[i - 1], font_size , 1);

      Vector2 position = {
        .x = button.x + (button.width - text_size.x)/2,
        .y = button.y + (button.height - text_size.y)/2
      };

      // Buttons Text
      int actual_button = i - 1;
      switch (actual_button)
      {
        case multiply: 
          DrawTextEx(MonoLisa_symbols, text_buttons[i - 1], position, symbol_size, 1, pink);
          break;
        case divide:
          DrawTextEx(MonoLisa_symbols, text_buttons[i - 1], position, symbol_size, 1, blue);
          break;
        case plus:
          DrawTextEx(MonoLisa_symbols, text_buttons[i - 1], position, symbol_size, 1, green);
          break;
        case minus: 
          DrawTextEx(MonoLisa_symbols, text_buttons[i - 1], position, symbol_size, 1, yellow);
          break;
        case module: 
          DrawTextEx(MonoLisa_symbols, text_buttons[i - 1], position, symbol_size, 1, gray);
          break;
        default:
          DrawTextEx(MonoLisa_regular, text_buttons[i - 1], position, font_size, 1, WHITE);
          break;
      }
      if(actual_column >= 5)
      {
        ++actual_row;
        actual_column = 0;
      }
    }

    if(mouse_over)
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    else
      SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    EndDrawing(); 
  }
  destroy_calculator(calculator);
  UnloadFont(MonoLisa_regular);
  CloseWindow(); 
  return 0;
}
