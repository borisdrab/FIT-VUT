using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using MathLib;

namespace StdDevCalculator;

/// @file StdDevCalculator.cs
/// @brief Trieda na výpočet výběrové směrodatné odchylky zo súboru.

/**
 * @class StdDevCalculator
 * @brief Obsahuje metódy na načítanie údajov zo súboru a výpočet výběrové směrodatné odchylky.
 */
public class StdDevCalculator
{
    /**
     * @brief Spúšťa proces načítania údajov zo súboru a výpočet výběrové směrodatné odchylky.
     * @param args Argumenty príkazového riadku, prvý obsahuje cestu k súboru.
     */
    public static void Run(string[] args)
    {
        var values = new List<double>();

        if (args.Length == 0)
        {
            Console.WriteLine("Please provide the path to the data file.");
            Console.ReadLine();
            return;
        }

        string filePath = args[0];

        try
        {
            string input = File.ReadAllText(filePath);
            Console.WriteLine("File contents:");
            Console.WriteLine(input);

            var numbers = input.Split(new[] { ' ', '\n', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            var culture = new CultureInfo("en-US");

            foreach (var number in numbers)
            {
                var cleanedNumber = number.Trim();
                Console.WriteLine($"Processing number: '{cleanedNumber}'");

                if (double.TryParse(cleanedNumber, NumberStyles.Any, culture, out var value))
                {
                    values.Add(value);
                }
                else
                {
                    Console.WriteLine($"Warning: Ignored invalid input '{cleanedNumber}'.");
                }
            }

            if (values.Count == 0)
            {
                Console.WriteLine("No valid numbers found in the input.");
                return;
            }

            var stddev = Operations.StandardDeviation(values);
            Console.WriteLine($"Standard Deviation: {stddev:F10}");
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error: {ex.Message}");
        }
        Console.WriteLine("Press Enter to exit...");
        Console.ReadLine();

    }
}
