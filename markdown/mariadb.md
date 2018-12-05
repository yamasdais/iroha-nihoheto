# mariadb

## Ubuntu 18.04
インストール記事をググると、character-set の設定を utf8 に変更したというような記事をよく目にしたが、少なくとも ubuntu 18.04 のデフォルトは、utf8mb4 になっており、内部的には 32bit 文字で扱うよ、という事かなと推測したので、とりあえずそのままにしておく。

## install 直後
``` bash
# sudo mysql_secure_installation  # sample db を消したりしたければ実行する
sudo mysql -u root
> use mysql
...
> show tables from mysql;
...
> select user,host, authentication_string from user;
... root のみだった
$ sudo mysql -u root
> CREATE USER 'oreore'@'localhost' IDENTIFIED BY 'xxxxx';
```
