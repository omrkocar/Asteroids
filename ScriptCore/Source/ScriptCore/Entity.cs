using System;

namespace Saz
{
    public class Entity
    {
        public readonly ulong ID;

        public Transform transform;

        protected Entity() { ID = 0; }

        internal Entity(ulong id)
        {
            ID = id;
            transform = GetComponent<Transform>();
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };
            return component;
        }
    }
}
