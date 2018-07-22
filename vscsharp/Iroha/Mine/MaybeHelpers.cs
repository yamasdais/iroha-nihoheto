using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace com.gmail.yamasdais.mine {
    public static class MaybeHelpers {
        #region conversion
        public static T? ToNullable<T>(this Maybe<T> a)
            where T : struct
            => a ? a.Value : new T?();

        public static Maybe<T> ToMaybe<T>(
            this T? a,
            Func<T, Maybe<T>> justNothing = null
        )
            where T : struct => a.HasValue
                ? a.Value.ToMaybe(justNothing)
                : Maybe<T>.Nothing;

        public static Maybe<T> ToMaybe<T>(
            this T a,
            Func<T, Maybe<T>> justNothing = null
        ) => justNothing == null
                ? Maybe<T>.JustOrNothing()(a)
                : justNothing(a);
        #endregion

#region conditional
#if false
        public static bool IsJust<T>(this Maybe<T> a)
            => a.HasValue;

        public static bool IsNothing<T>(this Maybe<T> a)
            => !IsJust<T>(a);
#endif
#endregion

#region mapper
#endregion

#region functional
        public static Maybe<TR> Lift<TA, TR>(
            this Maybe<TA> a,
            Func<TA, TR> func,
            Func<TR, Maybe<TR>> justNothing = null
        ) => !a
            ? Maybe<TR>.Nothing
            : func(a.Value).ToMaybe(justNothing);

        public static Func<TA, Maybe<TR>> Catcher<TA, TR, TEx>(
            Func<TA, TR> func,
            Func<TR, Maybe<TR>> justNothing = null
        ) where TEx : Exception {
            return a => {
                try {
                    return func(a).ToMaybe(justNothing);
                } catch (TEx) {
                    return Maybe<TR>.Nothing;
                }
            };
        }

#endregion
    }
}
