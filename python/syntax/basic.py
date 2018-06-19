#!/usr/bin/python3

# lambda の書き方
f = lambda x : x  # 引数をそのまま返す
f = lambda x, y = 0: x + y

## filter に渡す関数として
filtered = filter(lambda x: x % 2 == 0, range(10))


# 内包表記
evens = [i for i in range(20) if i % 2 == 0]
print(evens)

# enumerate 関数
for i, elem in enumerate(evens):
    print(i, elem)

# unpack
first, second, third = "foo", "bar", 100
print(third)

# unpack（*演算子）
first, second, *third = "foo", "bar", 100, "baz"
print(third)

# 辞書(dictionary)
dict = {
    1: 'one',
    2: 'two',
    3: 'three',
}
print(dict)
print(dict.keys()) # キーのリストビューオブジェクト dict_keys
print(dict.values()) # 値のリストビューオブジェクト dict_values
print(dict.items()) # 要素のリストビューオブエジェクト dict_items

# 集合
s = set([1,2,3])
s = {1,2,3}
empty_set = set()  # 空の集合はこうしてつくる。 {} は空の辞書になる。

# 辞書の内包表記
dict = {number: number**2 for number in range(100)}
print(dict)

# 集合の内包表記
s = {i for i in range(3)}

# ジェネレータ(yield)
def fibonacci():
    a, b = 0, 1
    while True:
        yield b
        a, b = b, a + b
# next() や for ループで値を取得できる。scheme の continuation みたいなもん？

## ジェネレータへの send()
def phychologist():
    print("あなたの悩みを聞かせてください")
    while True:
        answer = (yield)
        if answer is not None:
            if answer.endswith('?'):
                print("自分自身に問いかけをしすぎないように")
            elif '良い' in answer:
                print("それは良いですね。ぜひやりましょう。")
            elif '悪い' in answer:
                print("悲観的にならないようにしましょう。")
free = phychologist()
next(free)
## generator に send() で値を渡すと、yield の戻り値としてその値が使われる
free.send("気分が悪いです")

# genexp（ジェネレータ式）
iter = (x**2 for x in range(10) if x % 2 == 0)
for elem in iter:
    print(elem)
# ジェネレータは一つずつ要素を返すので、リスト内包表記よりもメモリ効率がいい。

# Decorator
class WithoutDecorators:
    def some_static_method():
        print("これは静的なメソッド")
    some_static_method = staticmethod(some_static_method)

    def some_class_method(cls):
        print("これはクラスメソッド")
    some_class_method = classmethod(some_class_method)
# 上と同じ意味のクラスをデコレータを使って実装すると、
class WithDecorators:
    @staticmethod
    def some_static_method():
        print("これは静的なメソッド")
    @classmethod
    def some_class_method(cls):
        print("これはクラスメソッド")

# Decorator関数sample
def mydecorator(function):
    def wrapped(*args, **kwargs):
        # 呼び出し前処理...
        result = function(*args, **kwargs)
        # 呼び出し後処理...
        return result
    return wrapped

# Decoratorクラスsample
class DecoratorAsClass:
    def __init__(self, function):
        self.function = function
    
    def __call__(self, *args, **kwargs):
        # 呼び出し前処理...
        result = self.function(*args, **kwargs)
        # 呼び出し後処理...
        return result

# パラメタ付きDecorator
from functools import wraps
# decorate する関数のメタ情報を decorate 後も維持するため、
# wraps() デコレータを利用すると便利
def repeat(number=3):
    """デコレートされた関数をnumberで指定された回数分繰り返す。
    最後に呼ばれた関数の戻り値を返す。
    :param number 繰り返し回数。無指定なら 3
    """
    def actual_decorator(function):
        @wraps(function)
        def wrapper(*args, **kwargs):
            result = None
            for _ in range(number):
                result = function(*args, **kwargs)
            return result
        return wrapper
    return actual_decorator
# 使用例
# パラメタ付きデコレータの場合、括弧をつけないと関数呼び出し時にエラーが発生する
@repeat()
def foo():
    """文字列 'foo' を表示する"""
    print("foo")

## デコレータは応用例が色々ありそう
## Memoize デコレータっぽいのは、functools.lur_cache というのが良さそうだ。

# コンテキストマネージャ
# File を使うとき
with open("/etc/hosts") as hosts:
    for l in hosts:
        if l.startswith('#'):
            continue
        print(l.strip())

# こんな感じで with 文を使うと、open したファイルを自動的に close する上、
# 例外もそのまま投げてくれる

