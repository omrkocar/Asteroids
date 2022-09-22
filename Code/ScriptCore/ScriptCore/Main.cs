using System;

namespace Saz
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# PrintInt: {value}");
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C# PrintInts: {value1} and {value2}");
        }

        public void PrintMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }
    }
}

