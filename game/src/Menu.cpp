#include "Menu.h"
#include <cmath>


/*******************/
/***** TEXTBOX *****/
/*******************/


TextBox::TextBox(Rectangle &rect, const std::string &label, const std::string &def_input, Color fill, Color text, int f_size) :
    bounds(rect),
    label(label),
    text_color(text),
    fill_color(fill),
    font_size(f_size) {
    if (!def_input.empty()) input = def_input;
}

TextBox::TextBox(Vec2 size, Vec2 loc, const std::string &label, const std::string &def_input, Color fill, Color text, int f_size) :
    label(label),
    text_color(text),
    fill_color(fill),
    font_size(f_size){
    bounds = {float(loc.x), float(loc.y), float(size.x), float(size.y)};
    if (!def_input.empty()) input = def_input;
}

bool TextBox::checkCollision(Vector2 mouse_pos) {
    if (CheckCollisionPointRec(mouse_pos, bounds)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            editing = true;
        return true;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // clicked elsewhere, lose focus
        editing = false;
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    return false;
}

void TextBox::draw(bool beginEndDraw) {
    if (beginEndDraw)
        BeginDrawing();
    DrawRectangleRec(bounds, fill_color);

    int text_width = MeasureText(label.c_str(), font_size);

    // draw label
    if (input.empty() && !editing) {
        DrawText(
            label.c_str(),
            bounds.x + bounds.width / 2 - text_width / 2.0,
            bounds.y + bounds.height / 2 - font_size / 2.0,
            font_size, text_color);
    }

    // draw input
    else {
        DrawText(
            input.c_str(),
            bounds.x + 10,
            bounds.y + bounds.height / 2 - font_size / 2.0,
            font_size, text_color);

        if (editing) {
            double time = GetTime();
            bool visible = fmod(time, 1.0) < 0.5;

            if (visible) {
                int text_w = MeasureText(input.c_str(), font_size);
                int cursor_x = bounds.x + 10 + text_w;
                int cursor_y = bounds.y + bounds.height / 2 - font_size / 2;

                DrawRectangle(cursor_x, cursor_y, 2, font_size, text_color);
            }
        }
    }


    if (beginEndDraw)
        EndDrawing();
}

bool TextBox::gatherInput() {
    if (!editing)
        return false;

    // 1. Text input
    int key = GetCharPressed();
    if (key >= 32 && key <= 125) {
        float maxWidth = bounds.width - 30;
        if (MeasureText(input.c_str(), font_size) < maxWidth)
            input.push_back((char)key);
    }

    // 2. Backspace
    if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) {
        input.pop_back();
    }

    // 3. Confirm / move to next
    if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        editing = false;
        return true;
    }
    return false;
}

std::string TextBox::getInput() {
    return input;
}


/******************/
/***** BUTTON *****/
/******************/


Button::Button(Rectangle rect, const std::string& label,
               Color fill, Color text, int f_size)
    : bounds(rect),
      label(label),
      fill_color(fill),
      text_color(text),
      font_size(f_size) {}

Button::Button(Vec2 size, Vec2 loc, const std::string& label, Color fill, Color text, int f_size) :
      label(label),
      fill_color(fill),
      text_color(text),
      font_size(f_size) {
    bounds = {float(loc.x), float(loc.y), float(size.x), float(size.y)};
}

void Button::draw() const {
    // Fill
    DrawRectangleRec(bounds, fill_color);

    // Center text
    int text_w = MeasureText(label.c_str(), font_size);
    float text_x = bounds.x + bounds.width/2 - text_w/2;
    float text_y = bounds.y + bounds.height/2 - font_size/2;

    DrawText(label.c_str(), text_x, text_y, font_size, text_color);

    // Optional: outline
    DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, BLACK);
}

bool Button::isPressed(Vector2 mouse_pos) const {
    if (CheckCollisionPointRec(mouse_pos, bounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            return true;
    }
    return false;
}

void TextBox::setEditing(bool e) {
    editing = e;
}


/****************/
/***** MENU *****/
/****************/


Menu::Menu(int win_width, int win_height)
    : start_button(
          {120, 40},
          {win_width / 2 - 60, win_height / 2 + 120},   // moved LOWER
          "join",
          dBlue,
          BLACK),
    win_height(win_height),
    win_width(win_width)
{
    const int ip_y   = win_height / 6.0;
    const int room_y = win_height / 2.0 - 40;
    const int name_y = win_height / 2.0 + 20;

    // More spacing between IP + Port
    textboxes.emplace_back(
        Vec2{260, 40},
        Vec2{win_width / 2 - 210, ip_y},   // moved LEFT further
        "IP Address",
        "snake.6amcoffee.org"
    );

    textboxes.emplace_back(
        Vec2{100, 40},
        Vec2{win_width / 2 + 90, ip_y},   // moved RIGHT further
        "Port",
        "9001"
    );

    textboxes.emplace_back(
        Vec2{260, 40},
        Vec2{win_width / 2 - 130, room_y},
        "Room ID"
    );

    textboxes.emplace_back(
        Vec2{260, 40},
        Vec2{win_width / 2 - 130, name_y},
        "Enter Name"
    );
    inputs.resize(4);
}

void Menu::run() {
    // I/O
    Vector2 mouse_pos = GetMousePosition();

    BeginDrawing();
    ClearBackground({ 202, 214, 173, 255 });

    // start button pressed
    if (start_button.isPressed(mouse_pos)) {
        in_menu = false;
        for (int j = 0; j < textboxes.size(); j++)
            inputs[j] = textboxes[j].getInput();
    }

    // 4 textboxes hard-coded
    for (int i = 0; i < textboxes.size(); i++) {
        textboxes[i].checkCollision(mouse_pos);

        // gather input if the textbox is selected
        if (textboxes[i].gatherInput()) {    // returns true if enter/tab pressed
            textboxes[i].setEditing(false);
            if (i == textboxes.size() - 1) {
                in_menu = false;
                for (int j = 0; j < textboxes.size(); j++)
                    inputs[j] = textboxes[j].getInput();
            }
            else
                textboxes[i + 1].setEditing(true);
        }

        // draw textboxes
        textboxes[i].draw();
    }
    // draw
    const char* title = "Welcome to Online 2 Player Snake!";
    DrawText(title,
        win_width / 2 - MeasureText(title, 26) / 2,
        int((win_height / 6.0 + win_height / 2.0 - 40) / 2),
        26, BLACK);

    start_button.draw();
    EndDrawing();
}

bool Menu::inMenu() const {
    return in_menu;
}

std::vector<std::string> Menu::getInputs() {
    std::vector<std::string> res;
    res.reserve(4);
    for (int i = 0; i < 4; i++)
        res.push_back(textboxes[i].getInput());
    return res;
};
