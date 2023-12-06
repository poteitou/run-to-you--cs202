// #include <MINE/Object.hpp>

// Object::Object(State::Context context, Object::Type type)
//     : mAnimation(context.mTextures->get(Textures::BlueSkirt)),
//       mSprite(context.mTextures->get(Textures::BlueSkirt)),
//       mCollideSound(context.mSoundBuffers->get(Sounds::Jump)),
//       mVelocity(0.f, 0.f),
//       mPlayedCollideSound(false),
//       mIsCollide(false),
//       mGravity(1.f)
// {
//     mCollideSound.setVolume(100);
//     // top, left, width, height
//     mWidth = context.mTextures->get(Textures::BlueSkirt).getSize().x / 4;
//     mHeight = context.mTextures->get(Textures::BlueSkirt).getSize().y / 2;
//     mSprite.setTextureRect(sf::IntRect(mWidth * 2, mHeight, mWidth, mHeight));
//     centerOrigin(mSprite);

//     mAnimation.setFrameSize(sf::Vector2i(mWidth, mHeight));
//     mAnimation.setNumFrames(8);
//     mAnimation.setDuration(sf::seconds(1.f));
//     mAnimation.centerOrigin();
//     mAnimation.setRepeating(true);
// }

// void Object::centerOrigin(sf::Sprite &sprite)
// {
//     sf::FloatRect bounds = sprite.getLocalBounds();
//     sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
// }

// sf::FloatRect Object::getGlobalBounds()
// {
//     if (!mIsCollide)
//     {
//         return mAnimation.getGlobalBounds();
//     }
//     return mSprite.getGlobalBounds();
// }

// // bool Object::isCollide(const Object &object) const
// // {
// //     return getGlobalBounds().intersects(object.getGlobalBounds());
// // }

// void Object::setPosition(float x, float y)
// {
//     mPosition.x = x;
//     mPosition.y = y;
//     mAnimation.setPosition(x, y);
//     mSprite.setPosition(x, y);
// }

// void Object::changeTexture(sf::Texture &texture)
// {
//     mAnimation.setTexture(texture);
//     mSprite.setTexture(texture);
// }

// void Object::handleEvent(User user)
// {
//     if (user.isSpacePressed)
//     {
//         if (!mIsJumping)
//         {
//             mVelocity.y = -25.f;
//             mPlayedCollideSound = false;
//             mIsJumping = true;
//         }
//     }
//     else if (!mIsJumping)
//     {
//         mVelocity.y = 0.f;
//     }
// }

// void Object::update(sf::Time dt, float groundHeight)
// {
//     // jumping mechanic
//     if (mPosition.y < groundHeight) // above ground
//     {
//         mVelocity.y += mGravity; // add gravity
//         mIsJumping = true;
//     }
//     else if (mPosition.y > groundHeight) // below ground
//         mPosition.y = groundHeight;

//     if (mPosition.y == groundHeight)
//         mIsJumping = false;

//     mAnimation.setPosition(mPosition);
//     mPosition.y += mVelocity.y;
//     mPosition.x += mVelocity.x * dt.asSeconds();

//     if (!mPlayedCollideSound)
//     {
//         mCollideSound.play();
//         mPlayedCollideSound = true;
//     }
//     if (mIsJumping)
//         mAnimation.setFrame(0);
//     else 
//         mAnimation.update(dt);
// }

// bool Object::isJumping() const
// {
//     return mIsJumping;
// }

// void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
// {
//     states.transform *= getTransform();
//     target.draw(mAnimation, states);
// }

