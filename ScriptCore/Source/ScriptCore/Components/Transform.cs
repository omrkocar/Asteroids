

namespace Saz
{
    public class Transform : Component
    {
        public Vector3 position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(Entity.ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(Entity.ID, ref value);
            }
        }

        public Vector3 rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 scale
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }
    } 
}