using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EnigmaGUI
{
    class EnigmaCfg
    {
        private string name;
        private string validRotors;
        private bool fourRotors;
        private bool plugboard;
        private string entryDisc;
        private string validReflector;

        public string Name
        {
            get { return name; }
        }

        public string ValidRotors
        {
            get { return validRotors; }
        }
        public bool FourRotors
        {
            get { return fourRotors; }
        }
        public bool Plugboard
        {
            get { return plugboard; }
        }
        public string EntryDisc
        {
            get { return entryDisc; }
        }
        public string ValidReflector
        {
            get { return validReflector; }
        }

        public EnigmaCfg(string name, string validRotors, bool fourRotors, bool plugboard, string entryDisc, string validReflector)
        {
            this.name = name;
            this.validRotors = validRotors;
            this.fourRotors = fourRotors;
            this.plugboard = plugboard;
            this.entryDisc = entryDisc;
            this.validReflector = validReflector;
        }


    }

    class Rotor
    {
        private string indexValue;
        private string name;
        public string Name
        {
            get { return name; }
        }
        public string IndexValue
        {
            get { return indexValue; }
        }

        public Rotor(string name, string indexValue)
        {
            this.name = name;
            this.indexValue = indexValue;
        }
    }
  
}
