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
|<td colspan="3">数学</td>
|[chg001.cxx](chg001.cxx)|3 または 5 で割り切れる正の整数の和|
|[chg002.cxx](chg002.cxx)|最大公約数|gcd() は [challenge.h](challenge.h) に実装してる
|[chg003.cxx](chg003.cxx)|最小公倍数|lcmr() は [challenge.h](challenge.h) に実装してる
|[chg004.cxx](chg004.cxx)|与えられた数より小さい最大の素数|is_prime() は [challenge.h](challenge.h) に実装してる
|[chg005.cxx](chg005.cxx)|Sexy素数|ranges 初登場。ranges 版 iota_view, transform を使っている
|[chg006.cxx](chg006.cxx)|過剰数|sum_proper_divisors() は [challenge.h](challenge.h) に実装してる
|[chg007.cxx](chg007.cxx)|友愛数|〃
|[chg008.cxx](chg008.cxx)|アームストロング数|ranges 版 accumulate() が無かったので accum() を作ったやつ。<br> std::array のインスタンスからサイズを求める ArraySizeDetector も作った。[challenge.h](challenge.h)
|[chg009.cxx](chg009.cxx)|素因数分解|prime_factors() は [challenge.h](challenge.h) に実装してる
|[chg010.cxx](chg010.cxx)|グレイコード|[sequence_view](sequence_view.h) を作った
|[chg011.cxx](chg011.cxx)|ローマ数字変換|iota_view を使った
|[chg012.cxx](chg012.cxx)|最長コラッツ数列|[sequence_view](sequence_view.h) や accum() を組み合わせて使った
|[chg013.cxx](chg013.cxx)|πの計算|モンテカルロ法を使ってるので、乱数の使い方のサンプルになってる
|[chg014.cxx](chg014.cxx)|ISBN の検証|string_view と ranges, accum() をバシバシ使った
|[chg015.cxx](chg015.cxx)|IPV4 データ型
|[chg016.cxx](chg016.cxx)|
|[chg017.cxx](chg017.cxx)|
|[chg018.cxx](chg018.cxx)|
|[chg019.cxx](chg019.cxx)|
|[chg020.cxx](chg020.cxx)|
|[chg021.cxx](chg021.cxx)|
|[chg022.cxx](chg022.cxx)|
|[chg023.cxx](chg023.cxx)|
|[chg024.cxx](chg024.cxx)|
|[chg025.cxx](chg025.cxx)|
|[chg026.cxx](chg026.cxx)|
|[chg027.cxx](chg027.cxx)|
|[chg028.cxx](chg028.cxx)|
|[chg029.cxx](chg029.cxx)|
|[chg030.cxx](chg030.cxx)|
|[chg031.cxx](chg031.cxx)|
|[chg032.cxx](chg032.cxx)|
|[chg033.cxx](chg033.cxx)|
|[chg034.cxx](chg034.cxx)|
|[chg035.cxx](chg035.cxx)|
|[chg036.cxx](chg036.cxx)|
|[chg037.cxx](chg037.cxx)|
|[chg038.cxx](chg038.cxx)|
|[chg039.cxx](chg039.cxx)|
|[chg040.cxx](chg040.cxx)|
|[chg041.cxx](chg041.cxx)|
|[chg042.cxx](chg042.cxx)|
|[chg043.cxx](chg043.cxx)|
|[chg044.cxx](chg044.cxx)|
|[chg045.cxx](chg045.cxx)|
|[chg046.cxx](chg046.cxx)|
|[chg047.cxx](chg047.cxx)|
|[chg048.cxx](chg048.cxx)|
|[chg049.cxx](chg049.cxx)|
|[chg050.cxx](chg050.cxx)|
|[chg051.cxx](chg051.cxx)|
|[chg052.cxx](chg052.cxx)|
|[chg053.cxx](chg053.cxx)|
|[chg054.cxx](chg054.cxx)|
|[chg055.cxx](chg055.cxx)|
|[chg056.cxx](chg056.cxx)|
|[chg057.cxx](chg057.cxx)|
|[chg058.cxx](chg058.cxx)|
|[chg059.cxx](chg059.cxx)|
|[chg060.cxx](chg060.cxx)|
|[chg061.cxx](chg061.cxx)|
|[chg062.cxx](chg062.cxx)|
|[chg063.cxx](chg063.cxx)|
|[chg064.cxx](chg064.cxx)|
|[chg065.cxx](chg065.cxx)|
|[chg066.cxx](chg066.cxx)|
|[chg067.cxx](chg067.cxx)|
|[chg068.cxx](chg068.cxx)|
|[chg069.cxx](chg069.cxx)|
|[chg070.cxx](chg070.cxx)|
|[chg071.cxx](chg071.cxx)|
|[chg072.cxx](chg072.cxx)|
|[chg073.cxx](chg073.cxx)|
|[chg074.cxx](chg074.cxx)|
|[chg075.cxx](chg075.cxx)|
|[chg076.cxx](chg076.cxx)|
|[chg077.cxx](chg077.cxx)|
|[chg078.cxx](chg078.cxx)|
|[chg079.cxx](chg079.cxx)|
|[chg080.cxx](chg080.cxx)|
|[chg081.cxx](chg081.cxx)|
|[chg082.cxx](chg082.cxx)|
|[chg083.cxx](chg083.cxx)|
|[chg084.cxx](chg084.cxx)|
|[chg085.cxx](chg085.cxx)|
|[chg086.cxx](chg086.cxx)|
|[chg087.cxx](chg087.cxx)|
|[chg088.cxx](chg088.cxx)|
|[chg089.cxx](chg089.cxx)|
|[chg090.cxx](chg090.cxx)|
|[chg091.cxx](chg091.cxx)|
|[chg092.cxx](chg092.cxx)|
|[chg093.cxx](chg093.cxx)|
|[chg094.cxx](chg094.cxx)|
|[chg095.cxx](chg095.cxx)|
|[chg096.cxx](chg096.cxx)|
|[chg097.cxx](chg097.cxx)|
|[chg098.cxx](chg098.cxx)|
|[chg099.cxx](chg099.cxx)|
|[chg100.cxx](chg100.cxx)|
