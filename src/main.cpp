#include "app.hpp"

#include "menu.hpp"
#include "game.hpp"

int main(int argc, char** argv)
{
    App app;
    app.add_state<Menu>("menu");
    app.add_state<Game>("game");

    app.run("menu");
}