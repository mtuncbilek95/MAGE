using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeGenerator
{
    internal class Parser
    {
        public static string GetProjectDirectory()
        {
            string workingDirectory = Environment.CurrentDirectory;
#pragma warning disable CS8602
            string projectDirectory = System.IO.Directory.GetParent(workingDirectory).Parent.Parent.Parent.Parent.FullName;
#pragma warning restore CS8602
            return projectDirectory;
        }

        public static string GetProjectName()
        {
            string projectDirectory = GetProjectDirectory();
            string[] projectDirectoryParts = projectDirectory.Split('\\');
            return projectDirectoryParts[projectDirectoryParts.Length - 1];
        }

        public static IReadOnlyCollection<string> GetReflectableFiles()
        {
            List<string> reflectableFiles = new List<string>();

            string path = GetProjectDirectory() + "\\TestCases";

            string[] headerFiles = Directory.GetFiles(path, "*.h", SearchOption.AllDirectories);

            foreach (string headerFile in headerFiles)
            {
                string fileContent = File.ReadAllText(headerFile);
                if (fileContent.Contains("M_CLASS") || fileContent.Contains("GENERATE_OBJECT") || fileContent.Contains("M_FUNCTION")
                    || fileContent.Contains("M_ENUM") || fileContent.Contains("M_PROPERTY") || fileContent.Contains("M_STRUCT"))
                {
                    reflectableFiles.Add(headerFile);
                }
            }

            return reflectableFiles;
        }

        public static IReadOnlyCollection<string> CatchGenerateNames(string path)
        {
            // Catch the type name that comes after the macro
            string[] lines = File.ReadAllLines(path);

            List<string> typeNames = new List<string>();
            for (int i = 0; i < lines.Length; i++)
            {
                //find the next line after M_CLASS
                if (lines[i].Contains("M_CLASS"))
                {
                    string line = lines[i + 1];
                    // catch the words comes after class
                    string[] words = line.Split(' ');
                    // add the word to the list
                    typeNames.Add(words[1]);
                }

                if(lines[i].Contains("M_STRUCT"))
                {
                    string line = lines[i + 1];
                    // catch the words comes after class
                    string[] words = line.Split(' ');
                    // add the word to the list
                    typeNames.Add(words[1]);
                }
            }

            System.Console.WriteLine("Type names: ");
            foreach (string typeName in typeNames)
            {
                System.Console.WriteLine(typeName);
            }

            return typeNames;
        }

        public static bool GenerateHeader(IReadOnlyCollection<string> filePaths)
        {
            List<string> genFilePath = new List<string>();
            IReadOnlyCollection<string> strings;

            foreach (string filePath in filePaths)
            {
                // Catch the file name without extension
                string fileName = filePath.Substring(filePath.LastIndexOf('\\') + 1, filePath.LastIndexOf(".") - filePath.LastIndexOf("\\") - 1);
                // delete the file name with extension
                string newPath = filePath.Substring(0, filePath.LastIndexOf('\\') + 1);

                File.Create(newPath + fileName + ".gen.h");
                genFilePath.Add(newPath + fileName + ".gen.h");

                string[] lines = File.ReadAllLines(filePath);

                int lastIncludeIndex = Array.FindLastIndex(lines, line => line.Trim().StartsWith("#include"));
                if (lastIncludeIndex != -1)
                {
                    // Create a new list of lines
                    var updatedLines = lines.ToList();

                    string newInclude = $"#include \"{fileName}.gen.h\"";
                    // Insert the new #include after the last one
                    updatedLines.Insert(lastIncludeIndex + 1, newInclude);

                    // Write the updated content back to the file
                    File.WriteAllLines(filePath, updatedLines);

                    Console.WriteLine($"Added {newInclude} after the last #include directive.");
                }

                strings = CatchGenerateNames(filePath); 
            }

            // Fill the header file with the necessary content
            foreach (string path in genFilePath)
            {
                StringBuilder generatedContext = new StringBuilder();

                generatedContext.AppendLine("#pragma warning(push)");
                generatedContext.AppendLine("#pragma warning(disable: 4067)");
            }

            return false;
        }
    }
}
