using BenchmarkDotNet.Attributes;
using System;
using System.Collections.Generic;
using MathLib;

namespace StdDevCalculator;

public class StdDevBenchmark
{
    [Params(50, 500, 1000)]
    public int Count;

    private List<double> numbers;

    [GlobalSetup]
    public void Setup()
    {
        var rnd = new Random(42);
        numbers = new List<double>(Count);
        for (int i = 0; i < Count; i++)
            numbers.Add(rnd.NextDouble() * 100);
    }

    [Benchmark]
    public double CalculateStdDev()
    {
        return Operations.StandardDeviation(numbers);
    }
}