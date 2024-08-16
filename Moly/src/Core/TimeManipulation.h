#pragma once

namespace Moly
{

    class TimeManipulation 
    {

    public:
        
        static float DeltaTime;
        static float LastFrame;
        static float GameTime;    

        static void Initialize() 
        {
            DeltaTime = 0.0f;
            LastFrame = 0.0f;
            GameTime = 0.0f;
        }

        static void Update(float _gameTime) 
        {
            GameTime = _gameTime;
            DeltaTime = GameTime - LastFrame;
            LastFrame = GameTime;
        }
    };

}
