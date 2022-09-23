using System;

namespace Game
{
    public class Player : Saz.Entity
    {
        void Init()
        {
            Console.WriteLine("Player.Init");
        }

        void Update(float deltaTime)
        {
            Console.WriteLine($"Player.Update: {deltaTime}");
        }
    }
}
