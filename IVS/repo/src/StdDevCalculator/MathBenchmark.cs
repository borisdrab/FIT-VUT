using BenchmarkDotNet.Attributes;
using System;

namespace StdDevCalculator;

public class MathBenchmark
{
    private readonly double a = 42.5;
    private readonly double b = 7.3;

    [Benchmark]
    public double Addition() => a + b;

    [Benchmark]
    public double Subtraction() => a - b;

    [Benchmark]
    public double Multiplication() => a * b;

    [Benchmark]
    public double Division() => a / b;

    [Benchmark]
    public double SquareRoot() => Math.Sqrt(a);

    [Benchmark]
    public double Power() => Math.Pow(a, b);

    [Benchmark]
    public double Logarithm() => Math.Log(a);

    [Benchmark]
    public double SinCalculation() => Math.Sin(a);

    [Benchmark]
    public double CosCalculation() => Math.Cos(a);
}