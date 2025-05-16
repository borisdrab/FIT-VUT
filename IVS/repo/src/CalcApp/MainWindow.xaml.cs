/**************************************************************
// Project:     Calculator
// Team:        Seg_fault
// $NoKeywords: $ivs_project_2
// $Date:       2025-04-25
**************************************************************/


using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Globalization;
using MathLib;

namespace CalcApp
{
    public partial class MainWindow : Window
    {
        private string currentInput = "";

        /// <summary>
        /// Inicializuje okno a nastavi event handler pre klavesnicu.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            CultureInfo.DefaultThreadCurrentCulture = CultureInfo.InvariantCulture;
            CultureInfo.DefaultThreadCurrentUICulture = CultureInfo.InvariantCulture;
        }

        /// <summary>
        /// Prida cislicu do aktualneho vyrazu.
        /// </summary>
        private void Number_Click(object sender, RoutedEventArgs e)
        {
            string digit = (string)((Button)sender).Content;
            currentInput += digit;
            Display.Text = currentInput;
        }

        /// <summary>
        /// Prida operator do aktualneho vyrazu.
        /// </summary>
        private void Operator_Click(object sender, RoutedEventArgs e)
        {
            string op = (string)((Button)sender).Content;
            currentInput += op;
            Display.Text = currentInput;
        }

        /// <summary>
        /// Vyhodnoti aktualny vyraz.
        /// Ak je zadany vyraz spravny, zobrazi vysledok.
        /// Ak je zadany vyraz nespravny, zobrazi chybove hlasenie vo vyskakovacom okne.
        /// </summary>
        private void Equals_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                double result = Utils.EvaluateExpression(currentInput);
                Display.Text = result.ToString();
                currentInput = result.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error");
            }
        }

        /// <summary>
        /// Vymaze aktualny vyraz a vystup.
        /// </summary>
        private void Clear_Click(object sender, RoutedEventArgs e)
        {
            currentInput = "";
            Display.Text = "";
        }

        /// <summary>
        /// Prida do aktualneho vyrazu desatinnu bodku.
        /// Ak posledny znak nebol cislica, doplni nulu a desatinnu bodku.
        private void Decimal_Click(object sender, RoutedEventArgs e)
        {
            if (!currentInput.EndsWith("."))
            {
                if (string.IsNullOrEmpty(currentInput) || "+-*/^".Contains(currentInput[currentInput.Length - 1]))
                {
                    currentInput += "0.";
                }
                else
                {
                    currentInput += ".";
                }
                Display.Text = currentInput;
            }
        }

        /// <summary>
        /// Prida do aktualneho vyrazu faktorial '!'.
        /// </summary>
        private void Factorial_Click(object sender, RoutedEventArgs e)
        {
            currentInput += "!";
            Display.Text = currentInput;
        }

        /// <summary>
        /// Prida do aktualneho vyrazu odmocninu.
        /// </summary>
        private void Root_Click(object sender, RoutedEventArgs e)
        {
            currentInput += "^(1/";
            Display.Text = currentInput;
        }

        /// <summary>
        /// Prida do aktualneho vyrazu logaritmus 'log(' .
        /// </summary>
        private void Log_Click(object sender, RoutedEventArgs e)
        {
            currentInput += "log(";
            Display.Text = currentInput;
        }

        /// <summary>
        /// Prida zatvorku do aktualneho vyrazu.
        private void Parenthesis_Click(object sender, RoutedEventArgs e)
        {
            currentInput += ((Button)sender).Content.ToString();
            Display.Text = currentInput;
        }

        /// <summary>
        /// Zobrazi napovedu o podporovanych operaciach.
        private void Help_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Calculator supports:\n\n" +
                            "- Basic operations: +, -, *, /\n" +
                            "- Power: a^n\n" +
                            "- Root: a^(1/n), where n is the degree of the root\n" +
                            "- Factorial: n!\n" +
                            "- Logarithm (base 10): log(x)\n" +
                            "- Parentheses: ( )\n" +
                            "- Decimal point: '.'\n\n" +
                            "Press '=' or Enter to calculate.",
                            "Help");
        }

        /// <summary>
        /// Umoznuje zadavat vstup z klavesnice (cisla, operatory, zatvorky, Enter, ...).
        /// </summary>
        private void MainWindow_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key >= Key.D0 && e.Key <= Key.D9)
            {
                currentInput += (e.Key - Key.D0).ToString();
            }
            else if (e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9)
            {
                currentInput += (e.Key - Key.NumPad0).ToString();
            }
            else if (e.Key == Key.OemPeriod || e.Key == Key.Decimal)
            {
                Decimal_Click(this, new RoutedEventArgs());
                return;
            }
            else if (e.Key == Key.Add) 
                currentInput += "+";
            else if (e.Key == Key.Subtract) 
                currentInput += "-";
            else if (e.Key == Key.Multiply) 
                currentInput += "*";
            else if (e.Key == Key.Divide) 
                currentInput += "/";
            else if (e.Key == Key.Enter || e.Key == Key.Return)
            {
                e.Handled = true;
                Equals_Click(this, new RoutedEventArgs());
                return;
            }
            else if (e.Key == Key.D9 && Keyboard.Modifiers == ModifierKeys.Shift)
                currentInput += "(";
            else if (e.Key == Key.D0 && Keyboard.Modifiers == ModifierKeys.Shift)
                currentInput += ")";

            Display.Text = currentInput;
        }
    }
}

