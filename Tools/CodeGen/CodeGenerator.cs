using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeGen
{
    public class ENGINE_API CodeGenerator
    {
        public CodeGenerator() { }

        public void ReadAll(string path)
        {
            // for now the path will be hardcoded.
            string currentPath = "D:\\Projects\\MAGE\\Examples\\ReflectionExample\\";

            string[] files = System.IO.Directory.GetFiles(currentPath, "*.h", SearchOption.AllDirectories);

            string[] filteredFiles = files
           .Where(file => !file.EndsWith(".generated.h", StringComparison.OrdinalIgnoreCase))
           .ToArray();

            foreach (string file in filteredFiles)
            {
                ReadTheInnerFile(file);
            }
        }

        public void WriteAll()
        {
            // Write all the files according to the Structure.
        }

        private void ReadTheInnerFile(string path)
        {
            // Read the file as a string line by line.
            string[] fileLines = System.IO.File.ReadAllLines(path);

            for (int lineIndex = 0; lineIndex < fileLines.Length; lineIndex++)
            {
                if(fileLines[lineIndex].Contains("REFLECT_CLASS();"))
                {
                    Structure structure = new Structure();
                    structure.FileDirectory = Path.GetDirectoryName(path) + "\\";
                    structure.FileName = Path.GetFileNameWithoutExtension(path);

                    // Check if there is .generated.h file for this file. If there is no find the last #include's index and set the AtWhichIndex to that index.
                    if (!System.IO.File.Exists(structure.FileDirectory + structure.FileName + ".generated.h"))
                    {
                        for (int localIndex = 1; localIndex < fileLines.Length; localIndex++)
                        {
                            if (fileLines[localIndex - 1].Contains("#include"))
                                structure.AtWhichIndex = localIndex;
                        }
                    }
                    else
                    {
                        structure.bHasGenInc = true;
                        // Find the index of the line that holds the generated.h
                        for (int localIndex = 0; localIndex < fileLines.Length; localIndex++)
                        {
                            if (fileLines[localIndex].Contains(structure.FileName + ".generated.h"))
                                structure.AtWhichIndex = localIndex;
                        }
                    }

                    structure.Type = SuperTypes.Class;
                    string className;
                    // Get the class ENGINE_API name from the next line by trimming the "class ENGINE_API".
                    if(fileLines[lineIndex + 1].Contains("RESOURCE_API"))
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length + ("RESOURCE_API ").Length);
                    else if(fileLines[lineIndex + 1].Contains("EDITOR_API"))
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length + ("EDITOR_API ").Length);
                    else if (fileLines[lineIndex + 1].Contains("GAME_API"))
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length + ("GAME_API ").Length);
                    else if (fileLines[lineIndex + 1].Contains("ENGINE_API"))
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length + ("ENGINE_API ").Length);
                    else if (fileLines[lineIndex + 1].Contains("PLUGIN_API"))
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length + ("PLUGIN_API ").Length);
                    else
                        className = fileLines[lineIndex + 1].Trim().Substring(("class ENGINE_API ").Length);

                    structure.ActualName = className;
                    structure.DisplayName = className;

                    for(int localIndex = 0; localIndex < fileLines.Length; localIndex++)
                    {
                        if (fileLines[localIndex].Contains("REFLECT_MEMBER();"))
                        {
                            Field field = new Field();
                            field.UsedMacro = "REFLECT_MEMBER";
                            string fieldFull = fileLines[localIndex + 1].Trim();

                            int index = fieldFull.Length - 1;
                            int length = 0;
                            while(index > 0)
                            {
                                if (fieldFull[index] == ' ')
                                    break;

                                index--;
                                length++;
                            }

                            field.ActualFieldName = fieldFull.Substring(index + 1, length - 1);

                            // check if there is a lowercase m in the beginning of the field name such as mPosition. if there is, remove m.
                            if (field.ActualFieldName[0] == 'm')
                                field.DisplayFieldName = field.ActualFieldName.Remove(0, 1);
                            else
                                field.DisplayFieldName = field.ActualFieldName;

                            field.Type = SubTypes.Member;

                            structure.ReflectedData.Add(field);
                        }
                        else if (fileLines[localIndex].Contains("REFLECT_FUNCTION();"))
                        {
                        }
                    }

                    Structures.Add(structure);
                }

                if (fileLines[lineIndex].Contains("REFLECT_ENUM();"))
                { 

                }
            }
        }

        public void PrintAll()
        {
            foreach (Structure structure in Structures)
            {
                Console.WriteLine("FileName: " + structure.FileName);
                Console.WriteLine("FileDir: " + structure.FileDirectory);
                Console.WriteLine("ActualName: " + structure.ActualName);
                Console.WriteLine("DisplayName: " + structure.DisplayName);
                Console.WriteLine("Type: " + structure.Type.ToString());
                Console.WriteLine("Has generated.h: " + structure.bHasGenInc.ToString());
                if (structure.bHasGenInc)
                    Console.WriteLine("AtWhichIndex: " + structure.AtWhichIndex.ToString());
                else
                    Console.WriteLine("Gen will be at: " + (structure.AtWhichIndex + 1).ToString());

                if (structure.ReflectedData.Count > 0)
                    Console.WriteLine("--------------------------------------");

                foreach (Field field in structure.ReflectedData)
                {
                    Console.WriteLine(structure.FileName + " in UsedMacro: " + field.UsedMacro);
                    Console.WriteLine(structure.FileName + " in ActualFieldName: " + field.ActualFieldName);
                    Console.WriteLine(structure.FileName + " in DisplayFieldName: " + field.DisplayFieldName);
                    Console.WriteLine(structure.FileName + " in Type: " + field.Type.ToString());
                }

                Console.WriteLine("=====================================");
            }
        }

        private List<Structure> Structures = [];
    }
}


