using System;
using Saz;

namespace Game
{
    public class Player : Entity
    {
        private Rigidbody2D m_Rigidbody;
        private float m_JumpForce = 1500f;

        void Init()
        {
            m_Rigidbody = GetComponent<Rigidbody2D>();
            
        }

        void Update(float deltaTime)
        {
            float speed = 1750f;
            Vector3 velocity = Vector3.zero;

            if (Input.IsKeyHeld(KeyCode.W))
                velocity.y = 1f;
            if (Input.IsKeyHeld(KeyCode.S))
                velocity.y = -1f;
            if (Input.IsKeyHeld(KeyCode.D))
                velocity.x = 1f;
            if (Input.IsKeyHeld(KeyCode.A))
                velocity.x = -1f;

            if (Input.IsKeyPressed(KeyCode.Space))
                m_Rigidbody.ApplyForce(Vector2.up * m_JumpForce * deltaTime, true);

            velocity *= speed;
            if (velocity != Vector3.zero)
                m_Rigidbody.ApplyForce(velocity.xy * deltaTime, true);

        }
    }
}
