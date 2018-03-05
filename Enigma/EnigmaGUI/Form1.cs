using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace EnigmaGUI
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            List<String> Machines = new List<string>()
            {
                "Enigma A",
                "Enigma B",
                "Enigma I",
                "M3 Army",
                "M3 Navy",
                "M4 Navy"
            };
            this.comboBox1.DataSource = Machines;

            List<String> Rotors = new List<string>()
            {
                "I", "II", "III", "IV", "V", "VI", "VII", "VII"
            };

            List<String> Reflectors = new List<string>()
            {
                "A",
                "B",
                "C",
                "Beta",
                "Gamma"
            };

            comboBox2.DataSource = new BindingSource(Rotors, "");
            comboBox3.DataSource = new BindingSource(Rotors, "");
            comboBox4.DataSource = new BindingSource(Rotors, "");
            comboBox5.DataSource = new BindingSource(Rotors, "");
            comboBox6.DataSource = new BindingSource(Reflectors, "");
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string enigmaString = "";

            //rotors
            enigmaString += Convert.ToString(comboBox2.SelectedIndex + 1);
            enigmaString += Convert.ToString(comboBox3.SelectedIndex + 1);
            enigmaString += Convert.ToString(comboBox4.SelectedIndex + 1);
            if (comboBox5.Enabled)
                enigmaString += Convert.ToString(comboBox5.SelectedIndex + 1);
            enigmaString += Convert.ToString(comboBox6.SelectedIndex + 1);
            enigmaString += " ";
            //enigmaString += "2151 ";

            //ring settings
            enigmaString += comboBox10.SelectedItem;
            enigmaString += comboBox7.SelectedItem;
            enigmaString += comboBox8.SelectedItem;
            if (comboBox9.Enabled)
                enigmaString += comboBox9.SelectedItem;
            enigmaString += " ";

            //message key
            enigmaString += comboBox11.SelectedItem;
            enigmaString += comboBox12.SelectedItem;
            enigmaString += comboBox13.SelectedItem;
            if (comboBox14.Enabled)
                enigmaString += comboBox14.SelectedItem;
            enigmaString += " ";

            //plugboard
            enigmaString += plugboard1.GetPlugboardString();

            enigmaString += " \"" + textBox1.Text + "\"";

            Process cmd = new Process();

            cmd.StartInfo.FileName = @"..\..\..\Debug\Enigma.exe";
            cmd.StartInfo.Arguments = enigmaString;
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.WaitForExit(1000);
            if (cmd.ExitCode == 0)
                textBox2.Text = cmd.StandardOutput.ReadToEnd();
            else
            {
                textBox2.Text = "Error in Engima.exe. Please check parameters.";
            }

        }

    }
}
