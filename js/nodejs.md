# Node.js について
node.js をインストールする。

## Ubuntu 18.04

``` bash
~$ sudo apt update
~$ sudo apt install nodejs
~$ sudo apt install npm
~$ cd tmp; curl -sL http://deb.nodesource.com/setup_8.x -o tmp/nodesrc_setup.sh
~$ nano tmp/nodesrc_setup.sh
~$ # 一応中身を確認する
~$ sudo bash tmp/nodesrc_setup.sh
~$ sudo apt-get install -y nodejs
~$ nodejs -v
v8.11.3
~$ npm -v
5.6.0
~$ # build-essential が入ってなければこのあたりで apt install しておく
~$ #
~$ sudo npm cache verify
~$ sudo npm install -g n
~$ sudo npm update -g npm
```

## vue.js

``` bash
$~ npm install vue
```
### emacs mode
melpa にはあって、ubuntu package 化されてない様なので、package-list-package で探して install する。
* vue-mode
* ssass-mode
* vue-html-mode
Ubuntu パッケージにもあるもの
* mmm-mode

## vue cli
vue プロジェクトのテンプレート

``` bash
npm run dev
```
で開発ビルド前の状態の web サーバにアクセスして動作チェックできる。

``` bash
npm run build
```
でリリース物がビルドされる

### eslint
lint に引っかかったらエラー扱いされるので、無視したい項目は .eslintrc.js で設定する事。
* 'space-before-function-paren': 'off'

