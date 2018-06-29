# Python についての覚書 #

## pip ##

ubuntu の python にはデフォルトでは pip が入っていなかった。

```
$ sudo apt install python3-pip
```

これで pip3 コマンドが使えるようになる。

Windows では pip がない？ので anaconda の conda を使う。

## python環境 ##
python は anaconda でやっているような、利用環境毎にそれぞれ環境を作っていくのが良い振る舞いとされている様だ。
これを実現するには

  * virtualenv
  * venv
  * buildout

があるようだ。

### virtualenv ###
これは、古くからあるので python2 系でも使える。だが、管理が大変になるらしい。

### venv ###
python3 系が対象。python2 では使えない。今更 python2 は使わない前提でならお勧め。
ubuntu では別パッケージになっているようなので、未インストールなら apt でインストールする。

```
$ sudo apt install python3-venv
$ python3 -m venv [環境名]
```

[環境名] というディレクトリが、色々初期化される。
ここにできる bin/activate というファイルを source すると、この環境にあるファイルやディレクトリが参照される様になるので、この状態で pip を使って環境を作っていくようだ。

環境設定をもとに戻したい場合は、deactivate コマンドを使う。
このコマンドは activate を source したときに定義されるシェル関数として実装されている。

ライブラリ等の環境を構築するためのものなので、ソースディレクトリに作る様なものではなく、ホームディレクトリ以下のアクセス権を有する適当なディレクトリに環境ディレクトリを作ればいいと思う。
ホームディレクトリに、.pylib/[環境名] とか作っていくスタイルを個人的な作法としようかなと取り敢えず決めておこうか。


### buildout ###
デプロイも含めたパワフルなツールらしいので複数人向けプロジェクトに向いているようだ。


## Jupyter notebook ##
Ubuntu 18.04 だとパッケージがあるようなので、これを使って試してみる。


```
$ sudo apt install jupyter-core python3-notebook python-ipykernel
$ jupyter notebook --generate-config
Error executing Jupyter command 'notebook': [Errno 2] No such file or directory
```

エラーが出る。どうもパッケージの整合性が取れていない、という事の様なので、pip でインストールし直す。

```
$ pip3 install --upgrade --force-reinstall jupyter
... update処理が動く
$ jupyter notebook --generate-config
Writing default config to: /home/dais/.jupyter/jupyter_notebook_config.py
$ jupyter notebook # jupyter-notebook でもいいようだ
ブラウザが起動して、notebook 入力可能状態になる
```

### kernel 追加 ###
仮想環境毎に kernel を用意しておけば、notebook 作成時に
所望の kernel を選ぶだけで環境を切り分けられるので便利だろう。
venv で環境作成して、その仮想環境を ipython から参照させる。
例えば scikit-learn を使って何やかんやしたいということであれば

```
$ python3 -m venv scilrn
$ . ~/.pylib/scilrn/bin/activate  # これ以降 venv での作業
(scilrn) $ pip3 install jupyter
(scilrn) $ pip3 install ipykernel
(scilrn) $ pip3 install scikit-learn numpy scipy
(scilrn) $ ipython3 kernel install --user --name scilrn --display-name "scikit-learn"
```

これで、notebook の kernel で scikit-learn という項目が選択できる様になっているはず。

Windows だと anaconda で conda を使った kernel 切り替えのやり方が
（ググれば）色々見つかる。

## matplotlib ##
matplotlib を import しようとすると、tkinter モジュールがない、というエラーになる。


```
ModuleNotFoundError: No module named 'tkinter'
```

これは、ubuntu の python3-tk パッケージをインストールすれば解決した。
