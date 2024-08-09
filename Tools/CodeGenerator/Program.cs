using CodeGenerator;
using System;

namespace Portakal
{
    internal static class Program
    {
        internal static void Main(string[] args)
        {
            var test = Parser.GetReflectableFiles();

            Parser.GenerateHeader(test);
        }
    }
}