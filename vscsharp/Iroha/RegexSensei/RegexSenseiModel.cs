using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace RegexSensei
{
    public class RegexSenseiModel: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        private Regex _re;

        public Regex Re {
            get {
                return _re;
            }
            private set {
                if (_re != value) {
                    if (_re == null || !_re.Equals(value)) {
                        OnPropertyChanged("Re");
                    }
                    _re = value;
                }
            }
        }

        private void refreshRe() {
            try {
                Re = new Regex(_pattern, ReOp);
            } catch (Exception e) {
                Re = null;
                throw e;
            }
        }
        protected void OnPropertyChanged(string name)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));

        public RegexOptions ReOp { get; set; } = RegexOptions.None;

        public string Text { get; set; } = "";

        private string _pattern = "";
        public string Pattern {
            set {
                if (_pattern != value) {
                    _pattern = value;
                    refreshRe();
                }
            }
            get {
                return _pattern;
            }
        }

        public void SetOption(RegexOptions op, bool v) {
            if (v) {
                ReOp |= op;
            } else {
                ReOp &= ~op;
            }
            refreshRe();
        }

        public MatchCollection Match()
            => Re?.Matches(Text);

    }
}
