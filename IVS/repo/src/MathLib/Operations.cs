/**************************************************************
// Project:     Calculator
// Team:        Seg_fault
// $NoKeywords: $ivs_project_2
// $Date:       2025-03-28
**************************************************************/

using System;
using System.Collections.Generic;

namespace MathLib;

/// @file Operations.cs
/// @mainpage
/// # Projekt Calculator
/// **Popis projektu:**
/// - Tento projekt implementuje matematické operácie vrátane výpočtu smerodajnej odchýlky.
/// - Obsahuje moduly na spracovanie matematických výrazov.
/// - Využíva vlastné knižnice pre výpočty.
///
/// **Moduly projektu:**
/// - **MathLib**: Obsahuje základné matematické operácie.
/// - **Utils**: Pomocné funkcie na evaluáciu výrazov.
/// - **Tokens**: Definície matematických tokenov.
///
/// **Autor tímu:** Seg_fault
/**
 * @class Operations
 * @brief Poskytuje metódy pre základné a pokročilé matematické operácie.
 */
public static class Operations
{
    private static readonly Dictionary<int, double> FactorialCache = new();
    
    /// <summary>
    /// Vypočíta smerodajnú odchýlku zoznamu hodnôt.
    /// </summary>
    /// @brief Vypočíta smerodajnú odchýlku zoznamu hodnôt.
    /// @param values Zoznam čísel, pre ktoré sa vypočíta smerodajná odchýlka.
    /// @return Smerodajná odchýlka vstupného zoznamu.
    /// @exception ArgumentException Vyvolaná, ak je vstupný zoznam prázdny.
    public static double StandardDeviation(List<double> values)
    {
        if (values.Count == 0)
            throw new ArgumentException("List cannot be empty");

        // Spočítame priemer ručne pomocou vlastných funkcií
        double sum = 0;
        foreach (var value in values)
        {
            sum = Add(sum, value);
        }
        double mean = Divide(sum, values.Count);

        // Spočítame súčet štvorcov rozdielov
        double sumOfSquares = 0;
        foreach (var value in values)
        {
            double diff = Subtract(value, mean);
            double square = Power(diff, 2);
            sumOfSquares = Add(sumOfSquares, square);
        }

        double variance = Divide(sumOfSquares, values.Count);
        return Root(variance, 2); // Druhá odmocnina pomocou tvojej Root funkcie
    }

   /// <summary>
    /// Vypočíta faktoriál čísla.
    /// </summary>
    /// @brief Vypočíta faktoriál čísla.
    /// @param number Číslo, ktorého faktoriál sa má vypočítať.
    /// @return Faktoriál čísla.
    /// @exception ArgumentException Vyvolaná, ak je číslo záporné.
    public static double Factorial(int number)
    {
        // Kontrola pre záporné čísla
        if (number < 0)
            throw new ArgumentException("Factorial is not defined for negative numbers.");

        // Ak je už v cache, vrátime uložený výsledok
        if (FactorialCache.ContainsKey(number))
            return FactorialCache[number];

        // Základný prípad: 0! = 1
        if (number == 0)
        {
            FactorialCache[0] = 1; // Uložíme 0! do cache
            return 1;
        }

        // Skontrolujeme, či máme v cache predchádzajúci faktorál (n-1)!
        var prevFact = Factorial(number - 1);

        // Vypočítame aktuálny faktorál
        var fact = number * prevFact;

        // Uložíme aktuálny faktorál do cache
        FactorialCache[number] = fact;

        return fact;
    }

    /// <summary>
    /// Vypočíta logaritmus čísla pri zadanom základe.
    /// </summary>
    /// @brief Vypočíta logaritmus čísla pri zadanom základe.
    /// @param a Číslo, z ktorého sa počíta logaritmus (musí byť kladné).
    /// @param base Základ logaritmu (musí byť kladný a rôzny od 1).
    /// @return Výsledok logaritmu čísla a pri základe base.
    /// @exception IndexOutOfRangeException Vyvolaná, ak a alebo base nie sú platné hodnoty.
    public static double Log(double a, double @base)
    {
        if (a <= 0)
            throw new IndexOutOfRangeException("Logarithm argument must be positive");
        
        if (@base <= 0 || Math.Abs(@base - 1) < double.Epsilon)
            throw new IndexOutOfRangeException("Logarithm base must be positive and not equal to 1");

        return Math.Log(a, @base);
    }

    
    /// <summary>
    /// Vypočíta n-tú odmocninu z čísla.
    /// </summary>
    /// @brief Vypočíta n-tú odmocninu z čísla.
    /// @param a Číslo, z ktorého sa počíta odmocnina.
    /// @param exponent Exponent (stupeň odmocniny, musí byť nenulový).
    /// @return Výsledok n-tej odmocniny čísla a.
    /// @exception IndexOutOfRangeException Vyvolaná, ak exponent je nulový alebo je odmocňované číslo záporné a exponent párny.
    public static double Root(double a, double exponent)
    {
        if (exponent == 0)
            throw new IndexOutOfRangeException("Exponent must be non-zero");

        // Ak exponent je párne číslo, záporný základ nie je povolený
        if (a < 0 && exponent % 2 == 0)
            throw new IndexOutOfRangeException("Cannot calculate even root of negative number");

        // Ak exponent nie je celé číslo a základ je záporný, vyhodíme výnimku (komplexné čísla)
        if (a < 0 && exponent % 1 != 0)
            throw new IndexOutOfRangeException("Cannot calculate root of negative number with non-integer exponent");

        // Ak základ je záporný a exponent je nepárny, môžeme spočítať koreň
        if (a < 0)
            return -Math.Pow(-a, 1.0 / exponent);

        // Pre kladné čísla môžeme použiť klasický výpočet
        return Math.Pow(a, 1.0 / exponent);
    }

    /// <summary>
    /// Vypočíta umocnenie čísla na daný exponent.
    /// </summary>
    /// @brief Vypočíta umocnenie čísla na daný exponent.
    /// @param num Základ mocniny.
    /// @param exp Exponent.
    /// @return Výsledok umocnenia num na exp.
    /// @exception ArgumentException Vyvolaná, ak je základ aj exponent nula alebo ak je základ záporný a exponent nie je celé číslo.
    public static double Power(double num, double exp)
    {
        // Špeciálny prípad, kedy je základ aj exponent nula, čo je matematicky nedefinované.
        if (num == 0 && exp == 0)
            throw new ArgumentException("0^0 is undefined");

        // Kontrola pre záporný základ a neceločíselný exponent. Výsledok by bol komplexné číslo, ktoré táto metóda neobsluhuje.
        if (num < 0 && exp % 1 != 0)
            throw new ArgumentException("Negative base with non-integer exponent is undefined");

        // Použitie vstavané funkcie Math.Pow na výpočet mocniny.
        return Math.Pow(num, exp);
    }

    /// <summary>
    /// Sčíta dve čísla.
    /// </summary>
    /// @brief Sčíta dve čísla.
    /// @param a Prvé číslo.
    /// @param b Druhé číslo.
    /// @return Súčet čísiel a a b.
    /// @exception OverflowException Vyvolaná, ak pretečie rozsah pre kladné čísla.
    public static double Add(double a, double b)
    {
        // Kontrola pretečenia pri sčítaní dvoch kladných čísel. Ak je výsledok záporný, došlo k pretečeniu rozsahu double.
        if (a > 0 && b > 0 && a + b < 0)
            throw new OverflowException("Arithmetic operation resulted in an overflow.");

        // Vrátenie súčtu dvoch čísel.
        return a + b;
    }

    /// <summary>
    /// Odčíta druhé číslo od prvého.
    /// </summary>
    /// @brief Odčíta druhé číslo od prvého.
    /// @param a Prvé číslo.
    /// @param b Druhé číslo.
    /// @return Rozdiel čísiel a a b.
    /// @exception OverflowException Vyvolaná, ak pretečie rozsah pre záporné čísla.
    public static double Subtract(double a, double b)
    {
        // Kontrola pretečenia pri odčítaní dvoch záporných čísel. Ak je výsledok kladný, došlo k pretečeniu rozsahu double.
        if (a < 0 && b < 0 && a - b > 0)
            throw new OverflowException("Arithmetic operation resulted in an overflow.");

        // Vrátenie rozdielu dvoch čísel.
        return a - b;
    }

    /// <summary>
    /// Vynásobí dve čísla.
    /// </summary>
    /// @brief Vynásobí dve čísla.
    /// @param a Prvé číslo.
    /// @param b Druhé číslo.
    /// @return Súčin čísiel a a b.
    /// @exception OverflowException Vyvolaná, ak pretečie rozsah pre výsledok násobenia.
    public static double Multiply(double a, double b)
    {
        // Vypočítanie výsledku násobenia.
        var res = a * b;
        // Kontrola pretečenia. Ak majú pôvodné čísla rovnaké znamienko a výsledok je záporný, alebo ak majú opačné znamienka a výsledok je kladný, došlo k pretečeniu.
        if ((((a > 0 && b > 0) || (a < 0 && b < 0)) && res < 0) ||
            (((a > 0 && b < 0) || (a < 0 && b > 0)) && res > 0))
            throw new OverflowException("Arithmetic operation resulted in an overflow.");

        // Vrátenie výsledku násobenia.
        return res;
    }
    
    /// <summary>
    /// Vykoná delenie dvoch čísel.
    /// </summary>
    /// @brief Vykoná delenie dvoch čísel.
    /// @param a Čitateľ.
    /// @param b Menovateľ.
    /// @return Výsledok delenia a / b.
    /// @exception DivideByZeroException Vyvolaná, ak je menovateľ nulový.
    public static double Divide(double a, double b)
    {
        if (b == 0)
            throw new DivideByZeroException("Arithmetic operation resulted in diving with zero.");
        
        return a / b;
    }
}