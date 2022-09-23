
namespace Saz
{
    public struct Vector3
    {
        public float x, y, z;

        public static Vector3 zero => new Vector3(0f);
        public static Vector3 one => new Vector3(1f);
        public static Vector3 up => new Vector3(0f, 1f, 0f);
        public static Vector3 down => new Vector3(0f, -1f, 0f);
        public static Vector3 left => new Vector3(-1f, 0f, 0f);
        public static Vector3 right => new Vector3(1f, 0f, 0f);
        public static Vector3 forward => new Vector3(0f, 0f, 1f);
        public static Vector3 back => new Vector3(0f, 0f, -1f);

        public Vector3(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Vector3(Vector2 xy, float z)
        {
            this.x = xy.x;
            this.y = xy.y;
            this.z = z;
        }

        public Vector2 xy
        {
            get => new Vector2(x, y);
            set
            {
                x = value.x;
                y = value.y;
            }
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }

        public static bool operator !=(Vector3 a, Vector3 b)
        {
            return a.x != b.x || a.y != b.y || a.z != b.z;
        }

        public static bool operator ==(Vector3 a, Vector3 b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }

    }
}
