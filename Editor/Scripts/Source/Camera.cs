using Saz;

namespace Game
{
    public class Camera : Entity
    {
        private float m_Speed = 0f;

        void Init()
        {
            m_Speed = 100.0f;
        }

        void Update(float deltaTime)
        {

            Vector3 velocity = Vector3.zero;

            if (Input.IsKeyHeld(KeyCode.W))
                velocity.y = 1.0f;
            else if (Input.IsKeyHeld(KeyCode.S))
                velocity.y = -1.0f;
            if (Input.IsKeyHeld(KeyCode.A))
                velocity.x = -1.0f;
            else if (Input.IsKeyHeld(KeyCode.D))
                velocity.x = 1.0f;

            velocity *= m_Speed;

            Vector3 translation = transform.position;
            translation += velocity * deltaTime;
            transform.position = translation;
        }
    }
}
