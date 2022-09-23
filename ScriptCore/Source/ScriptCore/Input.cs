
namespace Saz
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyPressed(keycode);
        }

        public static bool IsKeyHeld(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyHeld(keycode);
        }
    }
}
