//
// Created by Tyler Black on 11/25/25.
//

#ifndef SNAKECLIENT_MENU_H
#define SNAKECLIENT_MENU_H
#include <string>
#include "raylib.h"
#include "utils.h"
#include <vector>


class Button {
public:
    Button(Rectangle rect, const std::string& label, Color fill = LIGHTGRAY, Color text = BLACK, int f_size = 18);
    Button(Vec2 size, Vec2 loc, const std::string& label, Color fill = LIGHTGRAY, Color text = BLACK, int f_size = 18);
    void draw() const;
    bool isPressed(Vector2 mouse_pos) const;

private:
    Rectangle bounds;
    std::string label;
    Color fill_color;
    Color text_color;
    int font_size;
};


class TextBox {
public:
    TextBox(Rectangle &rect,
        const std::string &label="",
        const std::string &def_input="",
        Color fill=LIGHTGRAY, Color
        text=GRAY,
        int f_size=18);
    TextBox(Vec2 size, Vec2 loc,
        const std::string &label="",
        const std::string &def_input="",
        Color fill=LIGHTGRAY,
        Color text=GRAY,
        int f_size=18);
    bool checkCollision(Vector2 mouse_pos);
    void draw(bool beginEndDraw=false);
    bool gatherInput();   // returns true if TAB was pressed
    std::string getInput();
    void setEditing(bool e);
private:
    std::string input;
    std::string label;
    Rectangle bounds;
    Color text_color;
    Color fill_color;
    int font_size;
    bool editing = false;
};


class Menu {
public:
    Menu(int win_width, int win_height);
    void run();
    bool inMenu() const;
    std::vector<std::string> getInputs();   // IP, Port, RoomID, PlayerName
private:
    std::vector<TextBox> textboxes;
    // TextBox num_rounds_input;   TODO make this a drop down instead
    Button start_button;
    std::vector<std::string> inputs;
    bool in_menu = true;
    int win_width, win_height;
};

#endif //SNAKECLIENT_MENU_H