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
        BindingList<Rotor> allRotors;
        BindingList<Rotor> curRotors;
        BindingList<Rotor> allReflectors;
        BindingList<Rotor> curReflectors;
        public Form1()
        {
            allRotors = getAllRotors();
            allReflectors = getAllReflectors();
            curRotors = new BindingList<Rotor>();
            curReflectors = new BindingList<Rotor>();
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            List<EnigmaCfg> Machines = getEnigmas();
            this.comboBox1.DisplayMember = "name";
            this.comboBox1.ValueMember = "name";
            this.comboBox1.DataSource = Machines;
            this.comboBox1.SelectedValue = "Enigma M4";

            comboBox2.DisplayMember = "name";
            comboBox3.DisplayMember = "name";
            comboBox4.DisplayMember = "name";
            comboBox5.DisplayMember = "name";
            comboBox6.DisplayMember = "name";
            comboBox2.ValueMember = "indexValue";
            comboBox3.ValueMember = "indexValue";
            comboBox4.ValueMember = "indexValue";
            comboBox5.ValueMember = "indexValue";
            comboBox6.ValueMember = "indexValue";
            comboBox2.DataSource = new BindingSource(curRotors, "");
            comboBox3.DataSource = new BindingSource(curRotors, "");
            comboBox4.DataSource = new BindingSource(curRotors, "");
            comboBox5.DataSource = new BindingSource(curRotors, "");
            comboBox6.DataSource = new BindingSource(curReflectors, "");
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string enigmaString = "";
            //entry disc
            EnigmaCfg temp = (EnigmaCfg)comboBox1.SelectedItem;
            enigmaString += Convert.ToString(temp.EntryDisc);
            enigmaString += " ";
            //rotors
            enigmaString += Convert.ToString(comboBox2.SelectedValue);
            enigmaString += Convert.ToString(comboBox3.SelectedValue);
            enigmaString += Convert.ToString(comboBox4.SelectedValue);
            if (comboBox5.Enabled)
                enigmaString += Convert.ToString(comboBox5.SelectedValue);
            enigmaString += Convert.ToString(comboBox6.SelectedValue);
            enigmaString += " ";
            

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

        List<EnigmaCfg> getEnigmas()
        {
            List<EnigmaCfg> enigmaList = new List<EnigmaCfg>();
            EnigmaCfg newCfg = new EnigmaCfg("Enigma A", "ABC", false, false, "A", "A");
            enigmaList.Add(newCfg);
            newCfg = new EnigmaCfg("Enigma B", "ABC", false, false, "A", "A");
            enigmaList.Add(newCfg);
            newCfg = new EnigmaCfg("Enigma D", "GHI", false, false, "A", "A");
            enigmaList.Add(newCfg);
            newCfg = new EnigmaCfg("Enigma M3", "RSTUVWXY", false, true, "A", "FGH");
            enigmaList.Add(newCfg);
            newCfg = new EnigmaCfg("Enigma M4", "RSTUVWXY", true, true, "A", "DEFGH");
            enigmaList.Add(newCfg);
            newCfg = new EnigmaCfg("German Railway", "DEF", false, false, "A", "B");

            enigmaList.Add(newCfg);
            return enigmaList;
        }

        BindingList<Rotor> getAllRotors()
        {
            BindingList<Rotor> rotorList = new BindingList<Rotor>();
            Rotor newRotor = new Rotor("I", "R");
            rotorList.Add(newRotor);
            newRotor = new Rotor("II", "S");
            rotorList.Add(newRotor);
            newRotor = new Rotor("III", "T");
            rotorList.Add(newRotor);
            newRotor = new Rotor("IV", "U");
            rotorList.Add(newRotor);
            newRotor = new Rotor("V", "V");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VI", "W");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VII", "X");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VIII", "Y");
            rotorList.Add(newRotor);

            newRotor = new Rotor("IC", "A");
            rotorList.Add(newRotor);
            newRotor = new Rotor("IIC", "B");
            rotorList.Add(newRotor);
            newRotor = new Rotor("IIIC", "C");
            rotorList.Add(newRotor);

            newRotor = new Rotor("I", "D");
            rotorList.Add(newRotor);
            newRotor = new Rotor("II", "E");
            rotorList.Add(newRotor);
            newRotor = new Rotor("III", "F");
            rotorList.Add(newRotor);

            newRotor = new Rotor("I", "G");
            rotorList.Add(newRotor);
            newRotor = new Rotor("II", "H");
            rotorList.Add(newRotor);
            newRotor = new Rotor("III", "I");
            rotorList.Add(newRotor);

            newRotor = new Rotor("I", "J");
            rotorList.Add(newRotor);
            newRotor = new Rotor("II", "K");
            rotorList.Add(newRotor);
            newRotor = new Rotor("III", "L");
            rotorList.Add(newRotor);
            newRotor = new Rotor("IV", "M");
            rotorList.Add(newRotor);
            newRotor = new Rotor("V", "N");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VI", "O");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VII", "P");
            rotorList.Add(newRotor);
            newRotor = new Rotor("VIII", "Q");
            rotorList.Add(newRotor);

            return rotorList;
        }

        BindingList<Rotor> getAllReflectors()
        {
            BindingList<Rotor> reflectorList = new BindingList<Rotor>();
            Rotor newRotor = new Rotor("None", "A");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("ETW", "B");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("ETW", "C");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("Beta", "D");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("Gamma", "E");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("Reflector A", "F");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("Reflector B", "G");
            reflectorList.Add(newRotor);
            newRotor = new Rotor("Reflector C", "H");
            reflectorList.Add(newRotor);

            return reflectorList;
        }


            private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox temp = sender as ComboBox;
            EnigmaCfg enigmaCfg = (EnigmaCfg)temp.SelectedItem;
            if (enigmaCfg != null)
            {
                string validRotors = enigmaCfg.ValidRotors;
                this.curRotors.Clear();
                foreach (Rotor rotor in this.allRotors)
                {
                    if (validRotors.Contains(rotor.IndexValue))
                        this.curRotors.Add(rotor);
                }
                string validReflectors = enigmaCfg.ValidReflector;
                this.curReflectors.Clear();
                foreach (Rotor reflector in this.allReflectors)
                {
                    if (validReflectors.Contains(reflector.IndexValue))
                        this.curReflectors.Add(reflector);
                }
                if (enigmaCfg.Plugboard == true)
                    plugboard1.Enabled = true;
                else
                    plugboard1.Enabled = false;
                if (enigmaCfg.FourRotors == true)
                {
                    label5.Enabled = true;
                    comboBox5.Enabled = true;
                    comboBox9.Enabled = true;
                    comboBox14.Enabled = true;
                }
                else
                {
                    label5.Enabled = false;
                    comboBox5.Enabled = false;
                    comboBox9.Enabled = false;
                    comboBox14.Enabled = false;
                }
                
            }
        }
    }
}
