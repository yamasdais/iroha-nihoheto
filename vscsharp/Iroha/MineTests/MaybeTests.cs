using Microsoft.VisualStudio.TestTools.UnitTesting;
using com.gmail.yamasdais.mine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace com.gmail.yamasdais.mine.Tests {
    [TestClass()]
    public class MaybeTests {
        [TestMethod()]
        public void JustTest() {
            var n = Maybe<string>.Just("foo");
            Assert.IsTrue(n.HasValue);
            Assert.AreEqual(n.Value, "foo");

            Assert.IsTrue(n.Lift(v => v == "foo").Any(Functional.id));
            Assert.IsTrue(n.Lift(v => v == "foo").Any(Functional.id));
            Assert.IsFalse(n.Lift(v => v == "bar").Any(Functional.id));
            Assert.IsTrue(n);
        }
        [TestMethod()]
        public void NothingTest() {
            var n = Maybe<string>.Nothing;
            Assert.IsFalse(n.HasValue);
            Assert.IsFalse(n.Any());
            Assert.IsFalse(n.Any());
            Assert.IsFalse(n);
        }
        [TestMethod()]
        public void JustOrNothingTest0() {
            var s = Maybe<string>.JustOrNothing();
            var rfoo = s(null);
            Assert.IsFalse(rfoo.HasValue);
        }
        [TestMethod()]
        public void JustOrNothingTest1() {
            var s = Maybe<string>.JustOrNothing();
            var rfoo = s("foo");
            Assert.IsTrue(rfoo.HasValue);
        }
        [TestMethod()]
        public void JustOrNothingHOTest() {
            var s = Maybe<string>.JustOrNothing((v) => v != null);
            var rfoo = s("foo");
            Assert.IsTrue(rfoo.HasValue);
        }
        [TestMethod()]
        public void NestedMaybeTest() {
            var r = Maybe<string>.Just("foo");
            var r1 = Maybe<Maybe<string>>.Just(r);
        }

        [TestMethod()]
        public void ToMaybeTest() {
            var nothingIfNG = Maybe<string>.JustOrNothing(s => s != "NG");
            Assert.AreEqual(Maybe<string>.Nothing, (null as string).ToMaybe());
            Assert.AreEqual(Maybe<string>.Just("foo"), "foo".ToMaybe());
            Assert.AreEqual(Maybe<string>.Nothing, "NG".ToMaybe(nothingIfNG));
            Assert.AreEqual(Maybe<string>.Just(null), (null as string).ToMaybe(nothingIfNG));

            var val = Maybe<string>.Just("bar");
        }
        [TestMethod()]
        public void OperatorOrTest() {
            var afoo = "foo".ToMaybe();
            var abar = "bar".ToMaybe();
            var n = Maybe<string>.Nothing;
            Assert.AreEqual(afoo, n | n | afoo | abar);
            Assert.AreEqual(true, (bool)n | n | afoo | abar);
        }
    }
}