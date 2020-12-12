#include "game.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include <iostream>

void Game::start(bool white)
{
    std::scoped_lock lock(mutex);
    this->white = white;
    ready = true;

    std::cout << "We are ";
    if(white) std::cout << "White" << std::endl;
    else      std::cout << "Black" << std::endl;

    // white inits board
}

void Game::on_handle_events(const sf::Event& event)
{}

void Game::on_update(sf::Time dt)
{
    if(turn)
    {
        // std::cout << "Enter Text:";
        // std::cin >> board;
        // std::cout <<"\n";
        // turn = false;
    }
}

void Game::on_render(const sf::RenderTarget& target)
{
    // display the board
    if(turn)
    {
        ImGui::Begin("Make a fkn move!");
        if(ImGui::Button("move"))
        { turn = false; }
        ImGui::End();
    }
}


void Game::go()
{
    std::scoped_lock lock(mutex);
    turn = true;
}

bool Game::done() const
{
    std::scoped_lock lock(mutex);
    return !turn;
}

bool Game::is_ready() const
{
    std::scoped_lock lock(mutex);
    return ready;
}

void Game::set_board(std::string b)
{
    std::scoped_lock lock(mutex);
    board = std::move(b);
}

std::string Game::get_board() const
{
    std::scoped_lock lock(mutex);
    return board;
}

bool Game::is_white()
{
    std::scoped_lock lock(mutex);
    return white;
}