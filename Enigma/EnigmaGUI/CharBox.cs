using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EnigmaGUI
{
    public partial class CharBox : UserControl
    {
        public CharBox(int input)
        {
            InitializeComponent();
            char inChar = (char)('A' + input);
            this.label1.Text = inChar.ToString();
        }
        public event EventHandler CharTextChanged;
        public event EventHandler CharTextRemoved;

        public char getChar()
        {
            if (textBox1.Text == "")
                return ' ';
            else
                return textBox1.Text[0];
        }

        public char getIndex()
        {
            return label1.Text[0];
        }

        public void setChar(char inChar)
        {
            if (inChar == ' ')
                this.textBox1.Text = "";
            else
                this.textBox1.Text = inChar.ToString();
        }

        public void setBold()
        {
            label1.Font = new Font(label1.Font, FontStyle.Bold);
            textBox1.Font = new Font(textBox1.Font, FontStyle.Bold);
        }

        public void ClearBold()
        {
            label1.Font = new Font(label1.Font, FontStyle.Regular);
            textBox1.Font = new Font(textBox1.Font, FontStyle.Regular);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            TextBox thisTextBox = sender as TextBox;
            if (thisTextBox.Text != "")
            {
                char triggeredChar = thisTextBox.Text[0];
                if (Char.IsLetter(triggeredChar))
                    this.CharTextChanged(this, e);
                else
                    thisTextBox.Clear();
            }
            else
            {
                this.CharTextRemoved(this, e);
            }
        }

        public void clearChar()
        {
            this.textBox1.Clear();
        }
    }
}
