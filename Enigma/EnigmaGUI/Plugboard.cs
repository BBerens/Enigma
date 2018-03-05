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
    public partial class Plugboard : UserControl
    {
        public Plugboard()
        {

            InitializeComponent();
            for (int i = 0; i < 26; i++)
            {
                tableLayoutPanel1.Controls.Add(new CharBox(i));
                CharBox thisCharBox = tableLayoutPanel1.Controls[i] as CharBox;
                thisCharBox.CharTextChanged += new EventHandler(TextBoxValueChanged);
                thisCharBox.CharTextRemoved += new EventHandler(TextBoxValueRemoved);

            }
        }

        public string GetPlugboardString()
        {
            string plugboardStr = "";
            foreach (CharBox box in tableLayoutPanel1.Controls)
            {
                if (box.getChar() != ' ' && plugboardStr.IndexOf(box.getChar()) == -1)
                {
                    plugboardStr += box.getIndex();
                    plugboardStr += box.getChar();
                }
            }
            return plugboardStr;
        }
        protected void TextBoxValueChanged(object sender, EventArgs e)
        {
            CharBox triggeredBox = sender as CharBox;
            char matchingChar = triggeredBox.getChar();
            int matchingIndex = matchingChar - 'A';
            CharBox matchingBox = tableLayoutPanel1.Controls[matchingIndex] as CharBox;
            matchingBox.setChar(triggeredBox.getIndex());
            triggeredBox.setBold();
            matchingBox.setBold();
        }

        protected void TextBoxValueRemoved(object sender, EventArgs e)
        {
            CharBox triggeredBox = sender as CharBox;
            CharBox matchingBox = (CharBox)tableLayoutPanel1.Controls[0];
            char triggeredIndex = triggeredBox.getIndex();
            
            for(int i = 0; i < 26 && matchingBox.getChar() != triggeredIndex; i++)
            {
                 matchingBox = (CharBox)tableLayoutPanel1.Controls[i];
            }

            matchingBox.clearChar();
            triggeredBox.ClearBold();
            matchingBox.ClearBold();

        }
    }
}
