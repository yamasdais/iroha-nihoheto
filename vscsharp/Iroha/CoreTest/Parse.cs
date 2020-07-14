using Sprache;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Iroha
{
    public class ParsingLocator<T> : IPositionAware<ParsingLocator<T>>
    {
        public T Value { get; }
        public Position Start { get; private set; }
        public int Length { get; private set; }

        public ParsingLocator<T> SetPos(Position startPos, int length)
        {
            Start = startPos;
            Length = length;

            return this;
        }

        public ParsingLocator(T val)
        {
            Value = val;
        }
    }
    public static class ParsingLocatorExt
    {
        public static Parser<ParsingLocator<T>> Locate<T>(this Parser<T> value)
            => value.Select(x => new ParsingLocator<T>(x)).Positioned();

        public static R Select<T, R>(this ParsingLocator<T> obj, Func<ParsingLocator<T>, R> func)
            => func(obj);
    }
    public class TestParse
    {
        static Parser<int> Num = Parse.Decimal.Select(x => int.Parse(x));

        static Parser<(int, Position, int)> NumWithLocation =
            from num in Num.Locate()
            select (num.Value, num.Start, num.Length);
    }
}
