## CPC

Training with Modern C++ Challenge book.

## Build

### Debug
```shell
mkdir build; cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### make

// verbose 
```shell
make VERBOSE=1
```


## Description

本は C++17 向けだが C++20 の使い方を学ぶ為の個人プロジェクト。

module については g++ もまだ未対応なので、現時点では使わない。

## Contents

| File | Title | Note |
:---|:---|:---
|__数学__
|[chg001.cxx](src/chg001.cxx)|3 または 5 で割り切れる正の整数の和|
|[chg002.cxx](src/chg002.cxx)|最大公約数|gcd() は [challenge.h](include/challenge.h) に実装してる
|[chg003.cxx](src/chg003.cxx)|最小公倍数|lcmr() は [challenge.h](include/challenge.h) に実装してる
|[chg004.cxx](src/chg004.cxx)|与えられた数より小さい最大の素数|is_prime() は [challenge.h](include/challenge.h) に実装してる
|[chg005.cxx](src/chg005.cxx)|Sexy素数|ranges 初登場。ranges 版 iota_view, transform を使っている
|[chg006.cxx](src/chg006.cxx)|過剰数|sum_proper_divisors() は [challenge.h](include/challenge.h) に実装してる
|[chg007.cxx](src/chg007.cxx)|友愛数|〃
|[chg008.cxx](src/chg008.cxx)|アームストロング数|ranges 版 accumulate() が無かったので accum() を作ったやつ。<br> std::array のインスタンスからサイズを求める ArraySizeDetector も作った。[challenge.h](include/challenge.h)
|[chg009.cxx](src/chg009.cxx)|素因数分解|prime_factors() は [challenge.h](include/challenge.h) に実装してる
|[chg010.cxx](src/chg010.cxx)|グレイコード|[sequence_view](include/sequence_view.h) を作った
|[chg011.cxx](src/chg011.cxx)|ローマ数字変換|iota_view を使った
|[chg012.cxx](src/chg012.cxx)|最長コラッツ数列|[sequence_view](include/sequence_view.h) や accum() を組み合わせて使った
|[chg013.cxx](src/chg013.cxx)|πの計算|モンテカルロ法を使ってるので、乱数の使い方のサンプルになってる
|[chg014.cxx](src/chg014.cxx)|ISBN の検証|string_view と ranges, accum() をバシバシ使った
|__言語機能__
|[chg015.cxx](src/chg015.cxx)|IPv4 データ型|[ipv4.h](include/ipv4.h) にクラスを定義してある。union を使ったメンバ定義。宇宙船演算子のシンプルな利用
|[chg016.cxx](src/chg016.cxx)|IPv4 アドレス列挙|同上
|[chg017.cxx](src/chg017.cxx)|基本演算できる配列| [array2d](include/array2d.h) 定義
|[chg018.cxx](src/chg018.cxx)|可変引数 min() 関数|大小比較用のユーザ定義関数を渡せるバージョンを concept を使ってオーバーロードできたのでそのサンプル
|[chg019.cxx](src/chg019.cxx)|可変引数 push_back|concept を使って push_back を持っているかチェックしてみた
|[chg020.cxx](src/chg020.cxx)|コンテナの any, all, none|fold expression のサンプル
|[chg021.cxx](src/chg021.cxx)|ファイルハンドルラッパー|RAII に unique_ptr を使うサンプル
|[chg022.cxx](src/chg022.cxx)|温度の単位変換|[temperature.h](include/temperature.h) で実装。宇宙船演算子、ユーザ定義リテラルの使い方サンプル
|__文字列と正規表現__
|[chg023.cxx](src/chg023.cxx)|バイナリから文字列への変換|ranges 関数実装のサンプル、iomanip で hex 表示
|[chg024.cxx](src/chg024.cxx)|文字列からバイナリへの変換|coroutine を漸く導入。[coro_util.h](include/coro_util.h) で generator を定義した。no_unique_address を使った例外ハンドラの切り替えなども。[str_util.h](include/str_util.h) に coroutine を使った split() を実装
|[chg025.cxx](src/chg025.cxx)|英文タイトル Capitalize|sstream を使わず、coroutine を使った generator で文字を返して string のコンストラクタにイテレータを渡すようにして簡素化した
|[chg026.cxx](src/chg026.cxx)|区切り文字列で文字列を連結する|これも generator で文字を返すようにアレンジ
|[chg027.cxx](src/chg027.cxx)|区切り文字列で文字列を分割する|split() を chg024 で作っていたので、文字集合を受け付けるように仕様変更して流用。
|[chg028.cxx](src/chg028.cxx)|最長回文文字列|線形時間で済むアルゴリズムが有るらしいので、そちらを参考にしてやり直す
|[chg029.cxx](src/chg029.cxx)|
|[chg030.cxx](src/chg030.cxx)|
|[chg031.cxx](src/chg031.cxx)|
|[chg032.cxx](src/chg032.cxx)|
|[chg033.cxx](src/chg033.cxx)|
|[chg034.cxx](src/chg034.cxx)|
|[chg035.cxx](src/chg035.cxx)|
|[chg036.cxx](src/chg036.cxx)|
|[chg037.cxx](src/chg037.cxx)|
|[chg038.cxx](src/chg038.cxx)|
|[chg039.cxx](src/chg039.cxx)|
|[chg040.cxx](src/chg040.cxx)|
|[chg041.cxx](src/chg041.cxx)|
|[chg042.cxx](src/chg042.cxx)|
|[chg043.cxx](src/chg043.cxx)|
|[chg044.cxx](src/chg044.cxx)|
|[chg045.cxx](src/chg045.cxx)|
|[chg046.cxx](src/chg046.cxx)|
|[chg047.cxx](src/chg047.cxx)|
|[chg048.cxx](src/chg048.cxx)|
|[chg049.cxx](src/chg049.cxx)|
|[chg050.cxx](src/chg050.cxx)|
|[chg051.cxx](src/chg051.cxx)|
|[chg052.cxx](src/chg052.cxx)|
|[chg053.cxx](src/chg053.cxx)|
|[chg054.cxx](src/chg054.cxx)|
|[chg055.cxx](src/chg055.cxx)|
|[chg056.cxx](src/chg056.cxx)|
|[chg057.cxx](src/chg057.cxx)|
|[chg058.cxx](src/chg058.cxx)|
|[chg059.cxx](src/chg059.cxx)|
|[chg060.cxx](src/chg060.cxx)|
|[chg061.cxx](src/chg061.cxx)|
|[chg062.cxx](src/chg062.cxx)|
|[chg063.cxx](src/chg063.cxx)|
|[chg064.cxx](src/chg064.cxx)|
|[chg065.cxx](src/chg065.cxx)|
|[chg066.cxx](src/chg066.cxx)|
|[chg067.cxx](src/chg067.cxx)|
|[chg068.cxx](src/chg068.cxx)|
|[chg069.cxx](src/chg069.cxx)|
|[chg070.cxx](src/chg070.cxx)|
|[chg071.cxx](src/chg071.cxx)|
|[chg072.cxx](src/chg072.cxx)|
|[chg073.cxx](src/chg073.cxx)|
|[chg074.cxx](src/chg074.cxx)|
|[chg075.cxx](src/chg075.cxx)|
|[chg076.cxx](src/chg076.cxx)|
|[chg077.cxx](src/chg077.cxx)|
|[chg078.cxx](src/chg078.cxx)|
|[chg079.cxx](src/chg079.cxx)|
|[chg080.cxx](src/chg080.cxx)|
|[chg081.cxx](src/chg081.cxx)|
|[chg082.cxx](src/chg082.cxx)|
|[chg083.cxx](src/chg083.cxx)|
|[chg084.cxx](src/chg084.cxx)|
|[chg085.cxx](src/chg085.cxx)|
|[chg086.cxx](src/chg086.cxx)|
|[chg087.cxx](src/chg087.cxx)|
|[chg088.cxx](src/chg088.cxx)|
|[chg089.cxx](src/chg089.cxx)|
|[chg090.cxx](src/chg090.cxx)|
|[chg091.cxx](src/chg091.cxx)|
|[chg092.cxx](src/chg092.cxx)|
|[chg093.cxx](src/chg093.cxx)|
|[chg094.cxx](src/chg094.cxx)|
|[chg095.cxx](src/chg095.cxx)|
|[chg096.cxx](src/chg096.cxx)|
|[chg097.cxx](src/chg097.cxx)|
|[chg098.cxx](src/chg098.cxx)|
|[chg099.cxx](src/chg099.cxx)|
|[chg100.cxx](src/chg100.cxx)|
