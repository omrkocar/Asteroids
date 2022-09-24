using System;
using Saz;

namespace Game
{
    public class Player : Entity
    {
        private Rigidbody2D m_Rigidbody;
        public float JumpForce = 5f;
        public float Speed;

        void Init()
        {
            m_Rigidbody = GetComponent<Rigidbody2D>();
        }

        void Update(float deltaTime)
        {
 
            Vector3 velocity = Vector3.zero;

            if (Input.IsKeyHeld(KeyCode.D))
                velocity.x = 1f;
            if (Input.IsKeyHeld(KeyCode.A))
                velocity.x = -1f;

            if (Input.IsKeyPressed(KeyCode.Space))
            {
                m_Rigidbody.ApplyLinearImpulse(Vector2.up * JumpForce, true);
            }

            velocity *= Speed;
            if (velocity != Vector3.zero)
                m_Rigidbody.ApplyLinearImpulse(velocity.xy, true);

        }
    }
}
