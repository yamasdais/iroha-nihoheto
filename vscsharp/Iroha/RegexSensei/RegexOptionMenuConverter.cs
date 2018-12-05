using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using static System.Text.RegularExpressions.RegexOptions;
using System.Threading.Tasks;
using System.Windows.Data;

namespace RegexSensei {
    public class RegexOptionMenuConverter : IMultiValueConverter {
        public static readonly RegexOptions[] Options = {
          IgnoreCase,
          Multiline,
          ExplicitCapture,
          Compiled,
          Singleline,
          IgnorePatternWhitespace,
          RightToLeft,
          ECMAScript,
          CultureInvariant
        };
        public object Convert(object[] value, Type targetType, object parameter, CultureInfo culture) {
            var ret = None;
            foreach (var f in value.Zip(Options, (b, flg) => ((b is bool) && (bool)b) ? flg : None)) {
                ret |= f;
            }
            return ret;
        }

        public object[] ConvertBack(object value, Type[] targetType, object parameter, CultureInfo culture) {
            var ret = new object[targetType.Length];
            for (int i = 0; i < ret.Length; i++) {
                ret[i] = (bool)value;
            }
            return ret;
        }
    }
}
