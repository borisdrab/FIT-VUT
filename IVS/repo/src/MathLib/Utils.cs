using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text.RegularExpressions;

namespace MathLib;

/// @file Utils.cs
/// @brief Obsahuje pomocné funkcie na tokenizáciu a evaluáciu matematických výrazov.
/**
 * @class Utils
 * @brief Trieda obsahujúca metódy na spracovanie matematických výrazov.
 */
public static class Utils
{
    private const string PATTERN = @"log|[0-9]*\.?[0-9]+|[+\-*/^!()]";

    /**
     * @brief Rozdelí matematický výraz na jednotlivé tokeny.
     * @param expression Matematický výraz vo forme reťazca.
     * @return Zoznam tokenov reprezentujúcich matematický výraz.
     */
    private static List<string> Tokenize(string expression)
    {
        var tokens = new List<string>();
        var matches = Regex.Matches(expression, PATTERN);
        foreach (Match match in matches)
        {
            tokens.Add(match.Value);
        }

        return tokens;
    }

    /**
     * @brief Konvertuje infixový zápis tokenov do postfixového zápisu (RPN - Reverse Polish Notation).
     * @param infix Zoznam tokenov v infixovom zápise.
     * @return Zoznam tokenov v postfixovom zápise.
     * @exception InvalidOperationException Vyvolaná, ak sú zátvorky nesprávne alebo neznámy token.
     */
    private static List<string> ConvertInfixTokensToPostfix(List<string> infix)
    {
        var outputQueue = new List<string>();
        var operatorStack = new Stack<string>();
        
        var operatorData = Tokens.OperatorData;

        foreach (var token in infix)
        {
            if (double.TryParse(token, NumberStyles.Float, CultureInfo.InvariantCulture, out _))
            {
                outputQueue.Add(token);
            }
            else if (operatorData.TryGetValue(token, out var currentInfo))
            {
                while (operatorStack.Count > 0 && operatorData.TryGetValue(operatorStack.Peek(), out var topInfo))
                {
                    var shouldPop =
                        (currentInfo.rightAssociative && currentInfo.precedence < topInfo.precedence) ||
                        (!currentInfo.rightAssociative && currentInfo.precedence <= topInfo.precedence);

                    if (!shouldPop)
                        break;

                    outputQueue.Add(operatorStack.Pop());
                }

                operatorStack.Push(token);
            }
            else if (token == Tokens.LEFT_PARENTHESIS)
            {
                operatorStack.Push(token);
            }
            else if (token == Tokens.RIGHT_PARENTHESIS)
            {
                while (operatorStack.Count > 0 && operatorStack.Peek() != Tokens.LEFT_PARENTHESIS)
                {
                    outputQueue.Add(operatorStack.Pop());
                }

                if (operatorStack.Count == 0 || operatorStack.Peek() != Tokens.LEFT_PARENTHESIS)
                {
                    throw new InvalidOperationException("Mismatched parentheses");
                }

                operatorStack.Pop();

                // Pop the function after ")"
                if (operatorStack.Count > 0 && operatorData.ContainsKey(operatorStack.Peek()) && operatorStack.Peek() == Tokens.LOGARITHM)
                {
                    outputQueue.Add(operatorStack.Pop());
                }
            }
            else
            {
                throw new InvalidOperationException($"Unknown token: {token}");
            }
        }
        
        while (operatorStack.Count > 0)
        {
            var op = operatorStack.Pop();
            if (op is Tokens.LEFT_PARENTHESIS or Tokens.RIGHT_PARENTHESIS)
                throw new InvalidOperationException("Mismatched parentheses in expression");

            outputQueue.Add(op);
        }

        return outputQueue;
    }

    /**
     * @brief Vyhodnotí matematický výraz v postfixovom zápise.
     * @param postfix Zoznam tokenov v postfixovom zápise.
     * @return Výsledok evaluácie postfixového výrazu.
     * @exception InvalidOperationException Vyvolaná, ak sa vyskytne neplatný token alebo operátor.
     */
    private static bool IsOperand(string token, out double number) =>
        double.TryParse(token, NumberStyles.Float, CultureInfo.InvariantCulture, out number);

    private static double EvaluatePostfixTokens(List<string> postfix)
    {
        var stack = new Stack<double>();
        
        foreach (var token in postfix)
        {
            if (IsOperand(token, out var number))
            {
                stack.Push(number);
            }
            else if (Tokens.OperatorData.ContainsKey(token))
            {
                var b = stack.Pop();
                var a = 0.0;
                if (token is not Tokens.FACTORIAL and not Tokens.LOGARITHM)
                {
                    if (stack.TryPeek(out _))
                    {
                        a = stack.Pop();
                    }
                }
                
                var result = token switch
                {
                    Tokens.ADD => Operations.Add(a, b),
                    Tokens.SUBTRACT => Operations.Subtract(a, b),
                    Tokens.MULTIPLY => Operations.Multiply(a, b),
                    Tokens.DIVIDE => Operations.Divide(a, b),
                    Tokens.POWER => Operations.Power(a, b),
                    Tokens.FACTORIAL => Operations.Factorial((int)b),
                    Tokens.LOGARITHM => Operations.Log(b, 10),
                    _ => throw new InvalidOperationException($"Unknown operator: {token}")
                };
                
                stack.Push(result);
            }
            else
            {
                throw new InvalidOperationException($"Unknown token: {token}");
            }
        }

        return stack.Pop();
    }

    /**
     * @brief Vyhodnotí matematický výraz vo forme reťazca.
     * @param expression Matematický výraz vo formáte reťazca (napr. "3 + 5").
     * @return Výsledok evaluácie výrazu.
     * @exception InvalidOperationException Vyvolaná, ak je vo výraze neplatný token alebo zátvorky.
     */
    public static double EvaluateExpression(string expression)
    {
        var infix = Tokenize(expression);
        var postfix = ConvertInfixTokensToPostfix(infix);
        return EvaluatePostfixTokens(postfix);
    }
}