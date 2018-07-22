using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace com.gmail.yamasdais.mine
{
    public struct Maybe<T> : IEquatable<Maybe<T>>, IEnumerable<T>
    {
        public T Value {
            get {
                if (!HasValue) throw new InvalidOperationException("no value");
                return _value;
            }
        }
        private readonly T _value;

        public bool HasValue { get; }

        internal Maybe(T value) {
            _value = value;
            HasValue = true;
        }

#region conditional
        public bool IsJust {
            get { return HasValue; }
        }

        public bool IsNothing {
            get { return !IsJust;  }
        }
#endregion

        public static Maybe<T> Just(T value)
            => new Maybe<T>(value);

        public static readonly Maybe<T> Nothing = new Maybe<T>();

        public static implicit operator Maybe<T>(Maybe<Maybe<T>> flattenMaybe)
            => flattenMaybe.HasValue ? flattenMaybe.Value : Nothing;

        public override int GetHashCode()
            => unchecked(
                _value == null ? 0 : EqualityComparer<T>.Default.GetHashCode(_value) * 439
                ^ HasValue.GetHashCode()
            );

        public bool Equals(Maybe<T> other)
            => EqualityComparer<T>.Default.Equals(_value, other._value)
                && HasValue.Equals(other.HasValue);

        public override bool Equals(object obj)
            => ReferenceEquals(null, obj) ? false
                : obj is Maybe<T> && Equals((Maybe<T>)obj);

        #region operator overloading
        public static bool operator ==(Maybe<T> lhs, Maybe<T> rhs)
            => lhs.Equals(rhs);

        public static bool operator !=(Maybe<T> lhs, Maybe<T> rhs)
            => !lhs.Equals(rhs);

        public static implicit operator bool(Maybe<T> a)
            => a.IsJust;

        public static Maybe<T> operator |(Maybe<T> lhs, Maybe<T> rhs)
            => lhs ? lhs : rhs;
        #endregion

        public static Func<T, Maybe<T>> JustOrNothing()
            => (v) => v == null ? Nothing : Just(v);

        public static Func<T, Maybe<T>> JustOrNothing(
            Func<T, bool> pred
        ) => (v) => pred(v) ? Just(v) : Nothing;

        IEnumerator<T> IEnumerable<T>.GetEnumerator() {
            if (HasValue) {
                yield return Value;
            }
        }

        IEnumerator IEnumerable.GetEnumerator() {
            if (HasValue) {
                yield return Value;
            }
        }
    }
}
