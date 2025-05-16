using System.Collections.Generic;

namespace MathLib;

/// @file Tokens.cs
/// @brief Obsahuje definície matematických operátorov a ich vlastností.
/**
 * @class Tokens
 * @brief Obsahuje konštanty a ich metadáta na spracovanie operátorov.
 */
public static class Tokens
{
    /// <summary>Symbol pre sčítanie.</summary>
    public const string ADD = "+";

    /// <summary>Symbol pre odčítanie.</summary>
    public const string SUBTRACT = "-";

    /// <summary>Symbol pre násobenie.</summary>
    public const string MULTIPLY = "*";

    /// <summary>Symbol pre delenie.</summary>
    public const string DIVIDE = "/";

    /// <summary>Symbol pre umocnenie.</summary>
    public const string POWER = "^";

    /// <summary>Symbol pre faktoriál.</summary>
    public const string FACTORIAL = "!";

    /// <summary>Symbol pre logaritmus.</summary>
    public const string LOGARITHM = "log";

    /// <summary>Ľavá zátvorka.</summary>
    public const string LEFT_PARENTHESIS = "(";
    /// <summary>Pravá zátvorka.</summary>
    public const string RIGHT_PARENTHESIS = ")";
    
    /// <summary>
    /// Obsahuje prioritu a asociativitu pre jednotlivé operátory.
    /// </summary>
    public static readonly Dictionary<string, (int precedence, bool rightAssociative)> OperatorData = new()
    {
        { ADD, (2, false) },
        { SUBTRACT, (2, false) },
        { MULTIPLY, (3, false) },
        { DIVIDE, (3, false) },
        { POWER, (4, true) },
        { FACTORIAL, (5, true) },
        { LOGARITHM, (6, true) }
    };
}