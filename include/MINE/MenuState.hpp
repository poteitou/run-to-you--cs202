#ifndef MINE_MENUSTATE_HPP
#define MINE_MENUSTATE_HPP

#include <SFML/Graphics.hpp>

#include <MINE/Animation.hpp>
#include <MINE/State.hpp>
#include <MINE/Button.hpp>

#include <array>
#include <string>
#include <fstream>

namespace sf
{
    class RenderStates;
    class RenderTarget;
}

class MenuState : public State
{
public:
    MenuState(StateStack &stack, Context context);

    virtual bool handleEvent(User user);
    virtual bool update(sf::Time dt);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mBackgroundSprite;
    sf::Sprite mGroundSprite, mPopupRect, mPopup, mRank;
    sf::RectangleShape mBackgroundRect;
    sf::RectangleShape mScrollRect;
    sf::View mView;
    std::vector<sf::Text> mPopupText;
    std::array<Button::Ptr, 8> mButtons;
    sf::Music mMusic;
    sf::Text mTitle;
    sf::Sprite mTitleSprite;
    Animation mGirl;
    std::array<bool, 6> mButtonPressed;
    int mDrawPopup;
};

#endif // MINE_MENUSTATE_HPP
