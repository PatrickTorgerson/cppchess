#pragma once

#include "state.hpp"
#include "connection.hpp"

class App;

class Game final : public State
{
    //Chess Variables
    std::string board;
    bool turn {false};
    bool white {false};
    bool ready {false};

public:

    Game(App& app) : State(app) {}

    void start(bool white);

    virtual void on_handle_events(const sf::Event& event) override;
    virtual void on_update(sf::Time dt) override;
    virtual void on_render(const sf::RenderTarget& target) override;

    void go();
    bool done() const;
    bool is_ready() const;

    bool is_white();

    void set_board(std::string b);
    std::string get_board() const;
};