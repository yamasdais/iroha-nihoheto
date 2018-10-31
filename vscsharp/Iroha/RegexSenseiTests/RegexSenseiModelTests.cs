using Microsoft.VisualStudio.TestTools.UnitTesting;
using RegexSensei;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Text.RegularExpressions.RegexOptions;

namespace RegexSensei.Tests {
    [TestClass()]
    public class RegexSenseiModelTests {
        [TestMethod()]
        public void setOptionTest() {
            var model = new RegexSenseiModel();
            model.SetOption(IgnoreCase, true);
            Assert.AreEqual(IgnoreCase, model.ReOp);
            model.SetOption(IgnoreCase, false);
            Assert.AreEqual(None, model.ReOp);
            model.SetOption(IgnoreCase, true);
            model.SetOption(Multiline, true);
            Assert.AreEqual(IgnoreCase | Multiline, model.ReOp);
            model.SetOption(IgnoreCase, false);
            Assert.AreEqual(Multiline, model.ReOp);
        }
    }
}