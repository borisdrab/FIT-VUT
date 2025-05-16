using System;
using BenchmarkDotNet.Running;

namespace StdDevCalculator;

public static class Program
{
    static void Main(string[] args)
    {
        if (args.Length > 0)
        {
            switch (args[0])
            {
                case "benchmark":
                    // Spustí benchmark pre výpočet smerodajnej odchýlky
                    BenchmarkRunner.Run<StdDevBenchmark>();
                    break;

                case "benchmark_math":
                    // Spustí benchmark pre matematické operácie
                    BenchmarkRunner.Run<MathBenchmark>();
                    break;

                default:
                    Console.WriteLine("Unknown parameter. Use 'benchmark' or 'benchmark_math'.");
                    break;
            }
        }
        else
        {
            // Spustí normálny výpočet, ak nebol špecifikovaný žiadny parameter
            StdDevCalculator.Run(args);
        }
    }
}