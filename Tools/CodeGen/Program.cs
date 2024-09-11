using System;

namespace CodeGen
{
    class ENGINE_API Program
    {
        static void Main(string[] args)
        {
            CodeGenerator gen = new CodeGenerator();
            gen.ReadAll("");
            gen.PrintAll();
        }
    }
}