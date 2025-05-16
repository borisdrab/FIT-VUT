/**************************************************************
// Project:     Calculator
// Team:        Seg_fault
// $NoKeywords: $ivs_project_2 
// $Date:       2025-03-28
 **************************************************************/

using System;
using System.Collections.Generic;
using Xunit;

namespace MathLib.Tests;

public class Tests
{
    private List<double> zoznam = new();

    [Fact]
    public void SetUp()
    {
        for (double i = 0; i < 1000; i++)
        {
            zoznam.Add(i);
        }
    }

    [Fact]
    public void TestStdDev()
    {
        SetUp();
        double check = Operations.StandardDeviation(zoznam);
        double end = 288.81943609575;
        double tolerance = 0.5;
        Assert.True(Math.Abs(check - end) < tolerance);
    }

    [Fact]
    public void TestFactorial()
    {
        double end = Operations.Factorial(5);
        double check = 120;
        Assert.Equal(check, end);

        double end2 = Operations.Factorial(0);
        double check2 = 1;
        Assert.Equal(check2, end2);

        double end3 = Operations.Factorial(1);
        double check3 = 1;
        Assert.Equal(check3, end3);

        var exception = Assert.Throws<ArgumentException>(() => Operations.Factorial(-5));
        Assert.Equal("Factorial is not defined for negative numbers.", exception.Message);
    }

    [Fact]
    public void TestDivision()
    {
        double number = Operations.Divide(10, 2);
        double end = 5;
        Assert.Equal(number, end);

        double number2 = Operations.Divide(10, 3);
        double end2 = 10.0 / 3;
        Assert.Equal(number2, end2);

        double number3 = Operations.Divide(0, 3);
        double end3 = 0;
        Assert.Equal(number3, end3);

        var exception = Assert.Throws<DivideByZeroException>(() => Operations.Divide(10, 0));
        Assert.Equal("Arithmetic operation resulted in diving with zero.", exception.Message);
    }

    [Fact]
    public void TestLog()
    {
        var exception = Assert.Throws<IndexOutOfRangeException>(() => Operations.Log(10, 0));
        Assert.Equal("Logarithm base must be positive and not equal to 1", exception.Message);

        var exception2 = Assert.Throws<IndexOutOfRangeException>(() => Operations.Log(0, 10));
        Assert.Equal("Logarithm argument must be positive", exception2.Message);

        var exception3 = Assert.Throws<IndexOutOfRangeException>(() => Operations.Log(0, 0));
        Assert.Equal("Logarithm argument must be positive", exception3.Message); 

        var exception4 = Assert.Throws<IndexOutOfRangeException>(() => Operations.Log(-5, 3));
        Assert.Equal("Logarithm argument must be positive", exception4.Message);

        var exception5 = Assert.Throws<IndexOutOfRangeException>(() => Operations.Log(3, -5));
        Assert.Equal("Logarithm base must be positive and not equal to 1", exception5.Message);

        double result = Operations.Log(0.5, 4.7);
        double expected = Math.Log(0.5, 4.7); 
        Assert.Equal(expected, result, 10); 
    }


    [Fact]
    public void TestRoot()
    {
        var exception = Assert.Throws<IndexOutOfRangeException>(() => Operations.Root(-1, -2));
        Assert.Equal("Cannot calculate even root of negative number", exception.Message);

        var exception2 = Assert.Throws<IndexOutOfRangeException>(() => Operations.Root(-3, -4));
        Assert.Equal("Cannot calculate even root of negative number", exception2.Message);

        double number = Operations.Root(-27, 3);
        double final = -3;
        Assert.Equal(number, final);
    }

    [Fact]
    public void TestExpressionEvaluation()
    {
        var expression = "3 + 5 * (2 - 8)";
        var result = Utils.EvaluateExpression(expression);
        var expected = 3 + 5 * (2 - 8);
        Assert.Equal(expected, result);
    }

    [Fact]
    public void TestExpressionEvaluationInvalidExpression()
    {
        var expression = "3 + 5 * (2 - 8";
        Assert.Throws<InvalidOperationException>(() => Utils.EvaluateExpression(expression));
    }

    [Fact]
    public void TestSquare()
    {
        var exception = Assert.Throws<ArgumentException>(() => Operations.Power(0, 0));
        Assert.Equal("0^0 is undefined", exception.Message);

        double number = Operations.Power(1, -2);
        double final = 1;
        Assert.Equal(number, final);

        double number2 = Operations.Power(80, 0);
        double final2 = 1;
        Assert.Equal(number2, final2);
    }
}