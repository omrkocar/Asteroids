namespace Saz
{
    public class Rigidbody2D : Component
    {
        public void ApplyForce(Vector2 force, Vector2 point, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyForce(Entity.ID, ref force, ref point, wake);
        }

        public void ApplyForce(Vector2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyForceToCenter(Entity.ID, ref impulse, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }
}
