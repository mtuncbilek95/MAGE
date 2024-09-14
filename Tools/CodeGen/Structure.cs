using System;
using System.Collections.Generic;

namespace CodeGen
{
    public class ENGINE_API Field
    {
        public string? UsedMacro;
        public string? ActualFieldName;
        public string? DisplayFieldName;
        public SubTypes Type = SubTypes.None;
    }

    public class ENGINE_API Structure
    {
        public string? FileName;
        public string? ActualName;
        public string? DisplayName;

        public List<Field> ReflectedData = [];
        public string? FileDirectory;
        public SuperTypes Type = SuperTypes.None;

        public bool bHasGenInc = false;
        public int AtWhichIndex = 0; // This will be either the last index of the #include or the the actual index of the line that holds generated.h
    }

    public enum SuperTypes
    {
        None = 0,
        Class,
        Struct,
        Enum
    }

    public enum SubTypes
    {
        None = 0,
        Member,
        Function,
        Variable // Its for enum reflection members
    }
}