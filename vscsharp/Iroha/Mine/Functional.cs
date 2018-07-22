using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace com.gmail.yamasdais.mine {
    public static class Functional {
        public static T id<T>(T arg)
            => arg;
    }
}
