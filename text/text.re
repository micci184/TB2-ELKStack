* はじめに

　ある日、イチゴメロンパンを売っている会社で働くもふもふちゃんは、上司からいきなりこんなことを言われました。

　「もふもふちゃん、最近イチゴメロンパンの売れ行きが悪いんだ。ちょっと通販用のWebサイトにどのくらいの人がアクセスしているか
調べてくれないかな。日付ごとのグラフとかあると嬉しいなあ。」

　「あ、そういえばアクセス元もわかるといいよね。将来は海外展開もしたいしね！」

　イチゴメロンパンの通販サイトはWebサーバのApache上に構築されているため、Apacheログを解析すればユーザのアクセス数は調べることができそうです。

　「ええー、このログ全部調べるの…」
#@# Apacheログを入れる

　みてみたところ、Apacheのログはjson形式のようです。アクセス数を数えるためにはリクエストがGETのものだけ数える必要があります。
悪いことに、Webサーバとして利用されているマシンは他の機器とも通信しているようです。人がアクセスした履歴を追いかけるためには
使用している機器のIPアドレスを全て調べ、当てはまるログは解析対象から除外しなければなりません。なかなか骨が折れる作業です。

　…数日後、なんとか1週間分のアクセス数とアクセス元を出すことができました。ログの読みすぎで頭はくらくら、目はチカチカします。
そんなもふもふちゃんは解析結果を上司に提出しました。OKをもらえたので立ち去ろうとしたとき、今度はこんな頼まれごとをされてしまいました。

　「もふもふちゃん、そーいえば最近はTwitterの拡散力は大きいみたいだね。うちのイチゴメロンパンはどのくらいつぶやかれてるのか知りたいなあ」
#@# 挿絵入れる

　しかも、今度もつぶやきの発信元が知りたいと言われてしまいました。しかも、こんなことまでリクエストされてしまいます。
・情報の発信元は地図にプロットしてほしい
・「イチゴメロンパン」と入った単語のつぶやき数の推移を閲覧したい
・ここ3ヶ月分のつぶやき数がわかるとさらに良い

　「そんなー！Twitterの発信元とかどうやって調べたらいいの？だいたいイチゴメロンパンって単語が入ったつぶやき数なんて数えられないよ！
どーしたらいいのー？？」
　もふもふちゃんはすっかり困り果ててしまいました。
#@# 挿絵入れる

　…ここまで行かないにしろ、最近ではあるサービスの売り上げを伸ばすためであったり、自分の作った作品を宣伝するためには
インターネット上に溢れる情報や、自前のWebサイトへのアクセス履歴を分析する場面が多くなっています。理由としては
アクセスしてくるユーザーに対してダイレクトなマーケティング戦略を取るためであったり、人気の推移を研究することで
より買ってもらえる商品を生み出すためであったりと様々です。

　アクセス履歴はアクセスログから追いかけることがほとんどです。もふもふちゃんはログを手当たり次第に読んでいったので
すぐに疲れてしまいました。

　「もっと楽に解析できないのー？」
#@# 挿絵入れる

　…それができるんです。そう、Elastic Stackであれば！
　Elastic StackはオランダのElastic社が提供しているBIツールです。「BIツール」とは、企業や世の中にある色々な情報を集めて分析することを
支援するツールです。このElastic Stackですが、基本はOSSとして提供されており、コードはGithub上で管理されています。
日本でも少しずつ使われる場面が増えてきているようですが、まだまだ知見が少ないというのが現状です。

　ただ！このElastic Stackを使えば、もふもふちゃんが今依頼されていた仕事は全部簡単にできてしまいます。

・Webサイトへのアクセス履歴（Apacheログ)から特定のIP以外がアクセス元となっているものを抜き出したい
・情報の発信元は地図にプロットしたい
・「イチゴメロンパン」と入った単語のTwitter上でのつぶやき数の推移を閲覧したい（3ヶ月分）

　情報更新はほぼリアルタイムにすることが可能ですし、何よりいちいち生ログを閲覧する必要がありません。なんだかとても便利そうです。

　「ちょっとこのElastic Stackってやつ、自分の環境に入れてみようかな」

　みなさんも、もふもふちゃんと一緒に快適なログ解析を始めてみませんか？今までデバックするときくらいにしか使われていなかった
ゴミ同然のログを、ログ解析の力で宝の山に変えることができるかもしれません。

#@# 挿絵入れる

* おことわり
　この本で取り扱っている各ツールのバージョンはElasticSearch、Logstash、Kibana共に「5.2」を使用しています。
Elastic Stackはバージョンアップがかなり早いツールです。バージョンによって挙動がかなり違うため、別バージョンを使用した場合と
コンフィグの書き方や操作方法が異なる場合があります。あらかじめご了承ください。

　この本はログの分析方法をメインに扱う本です。そのため検索エンジンとしての
ElasticSearchのスキーマ設計など、性能チューニング系のトピックは取り上げません。

　筆者のPC環境はmacOS Serria （Ver：10.12.2）です。また、この本ではbrewを用いたElastic Stackのインストールは行いません。

　筆者のお財布事情により、本文はモノクロ印刷です。この本ではKibanaの画面キャプチャが複数回出てきますが
モノクロなことをお許しください。本当はカラフルな画面なので、公式サイトや画像検索などでKibana5を参照してみていただけると
テンションが上がると思います。

　この本の情報はElastic社の公式ドキュメント（URL：https://www.elastic.co/guide/index.html）を元に作成していますが、
本の情報を用いた開発・制作・運用に対して発生した全ての結果に対して責任は負いません。必ずご自身の環境でよく検証してから導入をお願いします。

* Elastic Stackって何？

　「Elastic StackはElastic社が提供しているBIツール群っていうのはわかったけど、どんなツールがあるのかな？
公式サイトを見るといっぱい種類があるみたいだけど…。」

#@# 挿絵入れる

　おや？もふもふちゃん、なんだかお困りのようです。それもそうですね。ELastic Stackには
なんだかたくさん便利ツールがあるのはわかりますが、ログ解析にはどのツールが必要なのかわかりません。
この章でログ分析に最低限必要なツールが何か、一緒にみてみましょう。

** Logstash
　Logstashは、各環境に散らばっているログを集め、指定した対象に連携できるツールです。ログの連携だけではなく、
ログの加工機能も持ち合わせています。コード自体はRuby言語で記載されています。

　肝心のどんなログが取り込みできるかですが、の出力形式としてよくあるテキストファイルはもちろん、xmlやjsonファイルも対象として指定できます。
ファイルの情報以外にも、Twitter APIと連携してTwitterのつぶやき情報を取り込む事や、
データベース（RSDB）に接続して情報を抜いてくる事も可能です。RSDBと連携する際はSQL文を用いて情報を取得するため
SQLが得意な人は自分の欲しい情報だけSQLで取得し、あとはログを連携するだけといった事も実現できます。

　ログの出力先ですが、このあと出てくるElasticSearchだけでなく、プロジェクトの進捗状況管理ツールである
Redmineに送付する事もできます。取り込みした情報をCSVファイルとして出力したりsyslogとして転送することができるので
利用方法によってはログ解析以上の威力を発揮するツールだと言えます。

** ElasticSearch
　ElasticSearchは、Javaで作られている分散処理型の検索エンジンです。クラスタ構成を組むことができるのが特徴なので、
大規模な環境で検索エンジンとして利用されることが多いです。某新聞のWebサイト内の検索、
Dockerのコンテナ検索、Facebook上での検索などが導入事例として有名です。

　クラスタとは、物理的には複数存在しているにも関わらず、論理的には1つとして見せることができる技術です。
処理の負荷分散ができるため、性能を求められる環境で選択されることが多いです。

/Users/mallow/review/text/text/images/cluster01.png
/Users/mallow/review/text/text/images/cluster02.png

　もちろん、ログ解析を行うときもよく使用されます。理由としては次に出てくるKibanaの情報取得元が
ElasticSearchに限られているからです。

** Kibana
　Kibanaは、ElasticSearchに貯められている情報を整形して可視化する情報分析ツールです。
開発言語はアナウンスは出ていないものの、ソース情報を見る限りJavaScriptがメインだと思われます。
Google Chrome等のブラウザからKibana指定のURLにアクセスすることで、このようなグラフをすぐ表示することができます。

#@# Kibanaのキャプチャを入れる

　Kibanaでは知りたい情報の件数だけでなく、折れ線グラフ・棒グラフ・円グラフを用いてログの詳細な情報を解析し
色分けして表示することが可能です。グラフの大きさを決める際にコンフィグなどを編集する必要はなく、ブラウザ上での操作で
全て完結する仕組みとなっています。

** Beats
　Beatsはサーバにインストールすることで、サーバ内のマシンデータをElasticSearchやLogstashに転送する
簡易的なデータ収集ツールです。サーバのリソース情報以外にもネットワークのパケット情報・Windowsのイベントログを収集することが
できるため、Logstashでカバーしきれない範囲の情報を集めてくることができます。
Logstashにログを転送することで、他のログと同じように加工・転送が可能となるため、痒いところに手がとどくツールという位置づけです。

　BeatsはGo言語で作られており、開発初期から大幅に機能追加がされているツールです。今後どのように拡張されていくのか、楽しみですね。

** 基本的な構成

　今回はBeatsは扱わず、ログを収集するLogstash、ログを貯めておくElasticSearch、ログを閲覧するKibanaを基本構成とします。
この3つのツールで構成されている状態は「ELK」と省略されて呼ばれることが多いです。この本でもこれ以降はLogstash、ElasticSearch、Kibanaの
3つをまとめて扱う際は「ELK」と省略して呼ぶこととします。

#@# 簡単な構成図を差し入れる

* 環境構築（各ツールのインストール）

　「よーし、ELKがどんなものかだいたい理解できたから、インストールしてみよ！
…あれ、なんかインストール方法もいっぱいあるみたい。どれを選べばいいのかなあ？」

#@# 挿絵入れる

　あら？もふもふちゃん、今度はインストールで詰まってしまったようです。
公式サイトからダウンロードでzipファイルを落としてくることができるようですが、他のやり方もあるようです。
状況に合っている1番いいやり方を選択したいですよね。一緒にみてみましょう。

** インストールの順番
　インストールの前に、どのツールからインストールするかを決めておきましょう。ELKのデータの流れを考えると、
ElasticSearch→Logstash→Kibanaの順にインストールすることをお勧めします。
理由は図X.Xにあるように、Logstashで取りこんだデータをElasticSearchに連携するため
先にデータの連携先をセットアップしておかないと正しく動作確認ができない可能性があるためです。

/Users/mallow/review/text/text/images/data.png

　また、Kibanaは起動前にElasticSearchのURLを指定する必要があるため、ElasticSearchがセットアップされていないと
そもそも動作しません。今回は各ツールごとに完結できる動作確認方法を取りますが、万が一に備えるという意味では
図X.Xのようにデータの連携先→連携元→データ閲覧ツールという流れでセットアップした方が良いでしょう。

/Users/mallow/review/text/text/images/data.png

** ElasticSearhのインストール
　先にも述べましたが、インストール方法は複数準備されています。導入の目的とご自身の環境に合わせて
ベストなものを選択すると良いでしょう。

1. とにかく使ってみたい場合（Linux）：zipファイル

　「とにかくどんなものか試してみたい！」そんな方はzipファイルを公式サイトからダウンロードしてきましょう。
インストール方法も適当なフォルダにzipファイルを解凍するだけなので導入は簡単に終わります。
ただし、serviceコマンドは付属しないため、長期的な運用を考えている場合には向かないインストール方法です。

2. ちゃんと運用もしたい場合（rpmパッケージを利用するLinux）：rpmパッケージ
　Elastic公式から提供されているrpmパッケージを利用した場合、serviceコマンドが自動的にダウンロードされます。
また、各種設定ファイルやディレクトリ構造はLinuxのディレクトリ形式に合わせて構築されます。
運用を検討している場合は、初めからこちらのやり方を取っておけば環境の再構築を防ぐことができます。

例）
コンフィグファイルの配置先：/etc/elasticsearch/
ログファイルの出力先：/var/log/elasticsearch
基本的なファイルの配置先：/opt/elasticsearch

3. ちゃんと運用もしたい場合（debパッケージを利用するLinux）：debパッケージ
　こちらも2のやり方と同様です。違いはUbuntu系のLinux用パッケージを使うか、
RedHat、OpenSuSE系のLinux用パッケージを使うかだけです。

4. とにかく使ってみたい場合かつ、Docker実行環境がある場合：Dockerコンテナ
　Elastic StackをDocker社が利用していることもあってか、Elastic公式からDockerイメージが提供されています。
手っ取り早く試してみたい場合、かつDockerコンテナの実行環境がある場合は素直にコンテナを利用した方が良いでしょう。
ただし、インストール方式の2.3に移行を考えている場合、構成がかなり変わるので初めからパッケージ利用での環境構築方法を選択した方が良さそうです。

　また、ElasticSearhは物理的にも、Javaのヒープメモリもかなり消費するツールです。
Dockerコンテナ上ではあまり性能が出ないため、大量のデータを流す予定がある場合はコンテナ利用を避けた方が良いです。

5. Puppet、Chef、Ansibleの実行環境がある場合：Githubのレシピを利用
　Elastic公式から各ツールのレシピが共有されています。Githubにアップロードされているため
そちらを用いて環境を構築することも可能です。

6. Windows/Mac上に構築する場合：zipファイル
　Windowsの場合、インストール方法はzipファイル一択となります。Macはbrewコマンドを用いることで
インストールすることもできますが、Elastic公式ではサポートされていないようなので、
今回はインストール方法から対象外としています。

　他の方が検証されているログを見る限り、brewコマンドでインストールした場合はtar.gzパッケージを解凍して
インストールしているように見えるため、公式の最新ファイルをダウンロードしてインストールするのと特に変わらないと思います。
好みで選択すると良いでしょう。

*** zipファイルを用いたインストール

今回は6のzipファイルを用いたインストール方法を取りたいと思います。
zipファイルを用いたインストールの場合、OSが違っていても基本的な手順は同じです。


1. 公式サイトからzipファイルをダウンロード

　Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。
URL:https://www.elastic.co/jp/downloads/elasticsearch

/Users/mallow/review/text/text/images/elasticsearch_download.png

2. 作業用ディレクトリを作成（好みで良いです）

　今回は検証用の環境構築なので、アンインストールを簡単にするために作業用ディレクトリを作成します。

ishiiaoi-no-MacBook-Pro:~ mallow$ mkdir ELK_Stack

ishiiaoi-no-MacBook-Pro:~ mallow$ ls -al
total 136
drwxr-xr-x+  46 mallow  staff   1564  3  5 15:31 .
drwxr-xr-x    5 root    admin    170  2 11 16:39 ..
-r--------    1 mallow  staff      7  2 11 16:49 .CFUserTextEncoding
-rw-r--r--@   1 mallow  staff  24580  3  5 15:22 .DS_Store
drwx------  183 mallow  staff   6222  3  5 15:30 .Trash
drwxr-xr-x    3 mallow  staff    102  5 25  2015 .android
drwxr-xr-x   16 mallow  staff    544  3 12  2016 .atom
-rw-------    1 mallow  staff   9804  3  4 10:37 .bash_history
-rw-r--r--    1 mallow  staff    112  7 12  2015 .bash_profile
drwx------   61 mallow  staff   2074  3  5 15:18 .bash_sessions
-rw-r--r--    1 mallow  staff     46  2  4 21:28 .bashrc
drwx------    3 mallow  staff    102  5 29  2015 .cups
drwxr-xr-x    3 mallow  staff    102  7 12  2015 .distlib
drwxr-xr-x    3 mallow  staff    102  2  4 13:32 .gem
-rw-r--r--    1 mallow  staff    408  3  4 13:18 .gitconfig
-rw-r--r--    1 mallow  staff     13  2 10 11:38 .gitignore_global
-rw-r--r--    1 mallow  staff     27  2 10 11:38 .hgignore_global
drwxr-xr-x    3 mallow  staff    102  6 19  2016 .jssc
drwxr-xr-x   35 mallow  staff   1190  7 21  2016 .npm
drwxr-xr-x    4 mallow  staff    136  6 19  2016 .oracle_jre_usage
drwxr-xr-x    7 mallow  staff    238  7 12  2015 .plenv
drwxr-xr-x    5 mallow  staff    170  7 12  2015 .pyenv
drwxr-xr-x    5 mallow  staff    170  7 12  2015 .rbenv
drwx------    3 mallow  staff    102 10  3  2015 .ssh
-rw-r--r--@   1 mallow  staff      0  2 13 15:41 .stCommitMsg
drwxr-xr-x    6 mallow  staff    204  7 12  2015 .subversion
drwxr-xr-x    9 mallow  staff    306 10  3  2015 .vagrant.d
-rw-------    1 mallow  staff   1877  7 12  2015 .viminfo
drwx------    4 mallow  staff    136  7 25  2015 Applications
drwxrwxr-x@   5 mallow  staff    170  3  5 15:16 Creative Cloud Files
drwx------@   4 mallow  staff    136  3  4 13:08 Desktop
drwx------@  22 mallow  staff    748  2 11 15:53 Documents
drwx------+  28 mallow  staff    952  3  5 15:30 Downloads
drwxr-xr-x    2 mallow  staff     68  3  5 15:31 ELK_Stack
drwx------@  32 mallow  staff   1088  3  5 15:15 Google ドライブ
drwx------@  67 mallow  staff   2278  2 11 20:26 Library
drwx------+   5 mallow  staff    170  5  8  2016 Movies
drwx------+   4 mallow  staff    136  5 24  2015 Music
drwx------+  44 mallow  staff   1496  2 11 15:53 Pictures
drwxr-xr-x+   5 mallow  staff    170  5 24  2015 Public
drwxr-xr-x    8 mallow  staff    272  3  4 13:16 TB2-ELKStack
drwxr-xr-x    2 mallow  staff     68  6 19  2016 VirtualBox VMs
drwxr-xr-x    2 mallow  staff     68  6  7  2015 eclipse
drwxr-xr-x    5 mallow  staff    170  7 23  2016 jhonny5
drwxr-xr-x   24 mallow  staff    816  2 13 15:42 review
drwxr-xr-x    3 mallow  staff    102  2  4 21:47 scriptlogs

ishiiaoi-no-MacBook-Pro:~ mallow$ mv /Users/mallow/Downloads/elasticsearch-5.2.2/ /Users/mallow/ELK_Stack/

ishiiaoi-no-MacBook-Pro:~ mallow$ cd ELK_Stack/elasticsearch-5.2.2/

3. zipファイルの解凍

　ディレクトリ内にダウンロードしたzipファイルを解凍します。

# 必要であれば
unzip elasticsearch-5.2.2

4. ElasticSearhの起動

　/binディレクトリ下にあるelasticsearchスクリプトを実行し、サービスを起動します。
OSがWindowsの場合、elasticsearch.batを実行します。

ishiiaoi-no-MacBook-Pro:~ mallow$ ls -al /Users/mallow/ELK_Stack/elasticsearch-5.2.2/
total 392
drwxr-xr-x@ 12 mallow  staff     408  3  5 15:49 .
drwxr-xr-x   3 mallow  staff     102  3  5 15:46 ..
-rw-r--r--@  1 mallow  staff   11358  2 25 02:24 LICENSE.txt
-rw-r--r--@  1 mallow  staff  172700  2 25 02:24 NOTICE.txt
-rw-r--r--@  1 mallow  staff    9108  2 25 02:24 README.textile
drwxr-xr-x@ 15 mallow  staff     510  2 25 02:29 bin
drwxr-xr-x@  6 mallow  staff     204  3  5 15:49 config
drwxr-xr-x   3 mallow  staff     102  3  5 15:49 data
drwxr-xr-x@ 35 mallow  staff    1190  2 25 02:29 lib
drwxr-xr-x   6 mallow  staff     204  3  5 15:49 logs
drwxr-xr-x@ 12 mallow  staff     408  2 25 02:29 modules
drwxr-xr-x@  2 mallow  staff      68  2 25 02:29 plugins

ishiiaoi-no-MacBook-Pro:~ mallow$ ls -al /Users/mallow/ELK_Stack/elasticsearch-5.2.2/bin/
total 680
drwxr-xr-x@ 15 mallow  staff     510  2 25 02:29 .
drwxr-xr-x@ 12 mallow  staff     408  3  5 15:49 ..
-rwxr-xr-x@  1 mallow  staff    7852  2 25 02:24 elasticsearch
-rwxr-xr-x@  1 mallow  staff    2540  2 25 02:24 elasticsearch-plugin
-rw-r--r--@  1 mallow  staff     733  2 25 02:24 elasticsearch-plugin.bat
-rw-r--r--@  1 mallow  staff  104448  2 25 02:24 elasticsearch-service-mgr.exe
-rw-r--r--@  1 mallow  staff  103936  2 25 02:24 elasticsearch-service-x64.exe
-rw-r--r--@  1 mallow  staff   80896  2 25 02:24 elasticsearch-service-x86.exe
-rw-r--r--@  1 mallow  staff   11239  2 25 02:24 elasticsearch-service.bat
-rwxr-xr-x@  1 mallow  staff     223  2 25 02:24 elasticsearch-systemd-pre-exec
-rwxr-xr-x@  1 mallow  staff    2514  2 25 02:24 elasticsearch-translog
-rw-r--r--@  1 mallow  staff    1435  2 25 02:24 elasticsearch-translog.bat
-rw-r--r--@  1 mallow  staff    3341  2 25 02:24 elasticsearch.bat
-rw-r--r--@  1 mallow  staff     828  2 25 02:24 elasticsearch.in.bat
-rwxr-xr-x@  1 mallow  staff     404  2 25 02:24 elasticsearch.in.sh
ishiiaoi-no-MacBook-Pro:~ mallow$

ishiiaoi-no-MacBook-Pro:elasticsearch-5.2.2 mallow$ bin/elasticsearch
[2017-03-05T15:49:00,854][INFO ][o.e.n.Node               ] [] initializing ...
[2017-03-05T15:49:01,004][INFO ][o.e.e.NodeEnvironment    ] [asgUjsK] using [1] data paths, mounts [[/ (/dev/disk1)]], net usable_space [123.8gb], net total_space [232.6gb], spins? [unknown], types [hfs]
[2017-03-05T15:49:01,005][INFO ][o.e.e.NodeEnvironment    ] [asgUjsK] heap size [1.9gb], compressed ordinary object pointers [true]
[2017-03-05T15:49:01,006][INFO ][o.e.n.Node               ] node name [asgUjsK] derived from node ID [asgUjsKwQg2H9BLGdc3J5A]; set [node.name] to override
[2017-03-05T15:49:01,011][INFO ][o.e.n.Node               ] version[5.2.2], pid[1521], build[f9d9b74/2017-02-24T17:26:45.835Z], OS[Mac OS X/10.12.2/x86_64], JVM[Oracle Corporation/Java HotSpot(TM) 64-Bit Server VM/1.8.0_45/25.45-b02]
[2017-03-05T15:49:04,176][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [aggs-matrix-stats]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [ingest-common]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [lang-expression]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [lang-groovy]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [lang-mustache]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [lang-painless]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [percolator]
[2017-03-05T15:49:04,177][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [reindex]
[2017-03-05T15:49:04,178][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [transport-netty3]
[2017-03-05T15:49:04,178][INFO ][o.e.p.PluginsService     ] [asgUjsK] loaded module [transport-netty4]
[2017-03-05T15:49:04,178][INFO ][o.e.p.PluginsService     ] [asgUjsK] no plugins loaded
[2017-03-05T15:49:08,320][INFO ][o.e.n.Node               ] initialized
[2017-03-05T15:49:08,320][INFO ][o.e.n.Node               ] [asgUjsK] starting ...
[2017-03-05T15:49:08,528][INFO ][o.e.t.TransportService   ] [asgUjsK] publish_address {127.0.0.1:9300}, bound_addresses {[fe80::1]:9300}, {[::1]:9300}, {127.0.0.1:9300}
[2017-03-05T15:49:11,619][INFO ][o.e.c.s.ClusterService   ] [asgUjsK] new_master {asgUjsK}{asgUjsKwQg2H9BLGdc3J5A}{r2ufo0xMTqSnm-4w0Huq9w}{127.0.0.1}{127.0.0.1:9300}, reason: zen-disco-elected-as-master ([0] nodes joined)
[2017-03-05T15:49:11,643][INFO ][o.e.h.HttpServer         ] [asgUjsK] publish_address {127.0.0.1:9200}, bound_addresses {[fe80::1]:9200}, {[::1]:9200}, {127.0.0.1:9200}
[2017-03-05T15:49:11,643][INFO ][o.e.n.Node               ] [asgUjsK] started
[2017-03-05T15:49:11,653][INFO ][o.e.g.GatewayService     ] [asgUjsK] recovered [0] indices into cluster_state

5. 動作確認

　ElasticSearuへクエリを投げ、返り値があるか確認します。
返り値が帰ってくれば、正常にインストールできています。

# elasticsearchの稼動確認

ishiiaoi-no-MacBook-Pro:~ mallow$ curl http://localhost:9200/
{
  "name" : "asgUjsK",
  "cluster_name" : "elasticsearch",
  "cluster_uuid" : "aK9PZbz4SCSbg3Bi_j_R_g",
  "version" : {
    "number" : "5.2.2",
    "build_hash" : "f9d9b74",
    "build_date" : "2017-02-24T17:26:45.835Z",
    "build_snapshot" : false,
    "lucene_version" : "6.4.1"
  },
  "tagline" : "You Know, for Search"
}

もしくはGoogle Chrome上でURLにアクセスし、同じような返り値があればOKです。

/Usersmallow/review/text/text/images/localhost_9200.png

** Logstashのインストール

LogstashもElasticSearhと同様、インストール方式を選択することが可能です。
ただ、ツールごとにインストール方式を分けるやり方は、ディレクトリ構造が異なってしまうため避けた方が良いです。

1. とにかく使ってみたい場合（Linux）：zipファイル

　こちらもElasticSearhと同様、zipファイルを展開するだけでインストールが終了します。
serviceコマンドは付属しません。

2. ちゃんと運用もしたい場合（rpmパッケージを利用するLinux）：rpmパッケージ
　こちらもserviceコマンドの存在や、ディレクトリ構成が自動で割り当てられる点なども
ElasticSearhと同様です。

例）
コンフィグファイルの配置先：/etc/logstash/
ログファイルの出力先：/var/log/logstash
基本的なファイルの配置先：/opt/logstash

3. ちゃんと運用もしたい場合（debパッケージを利用するLinux）：debパッケージ
　こちらもElasticSearhと同様です。

4. とにかく使ってみたい場合かつ、Docker実行環境がある場合：Dockerコンテナ
　ElasticSearhと同様、Elastic社からDockerコンテナが提供されています。
ただし、ElasticSearhとは別のコンテナのため、同時にコンテナを複数起動する必要があります。

　LogstashはRubyで作成されていますが、起動にJavaを必要とします。
こちらもヒープメモリもかなり消費するツールなので注意が必要です。

5. Puppet、Chef、Ansibleの実行環境がある場合：Githubのレシピを利用
　Elastic公式から各ツールのレシピが提供されているので、Githubからクローンして利用することも可能です。

6. Windows/Mac上に構築する場合：zipファイル
　Windowsの場合、インストール方法はzipファイル一択となります。Macはbrewコマンドを用いることで
インストールすることもできますが、Elastic公式ではサポートされていないようなので、
今回はインストール方法から除外しています。

*** zipファイルを用いたインストール

　ElasticSearhのインストール方法に合わせるため、6のzipファイルを用いたインストール方法を取ります。
こちらも、OSごとに手順に差はありません。

1. 公式サイトからzipファイルをダウンロード

　Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。

/Usersmallow/review/text/text/images/logstash_download.png

2. zipファイルの解凍

　ElasticSearhをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。
ディレクトリ構成は一例です。

unzip logstash-5.2.2

ishiiaoi-no-MacBook-Pro:ELK_Stack mallow$ ls -al
total 0
drwxr-xr-x   4 mallow  staff   136  3  5 16:13 .
drwxr-xr-x+ 46 mallow  staff  1564  3  5 15:31 ..
drwxr-xr-x@ 12 mallow  staff   408  3  5 15:49 elasticsearch-5.2.2
drwxr-xr-x@ 17 mallow  staff   578  3  5 16:12 logstash-5.2.2

ishiiaoi-no-MacBook-Pro:ELK_Stack mallow$ ls -al logstash-5.2.2/
total 360
drwxr-xr-x@ 17 mallow  staff     578  3  5 16:12 .
drwxr-xr-x   4 mallow  staff     136  3  5 16:13 ..
-rw-r--r--@  1 mallow  staff  111569  2 25 02:41 CHANGELOG.md
-rw-r--r--@  1 mallow  staff    2249  2 25 02:41 CONTRIBUTORS
-rw-r--r--@  1 mallow  staff    3864  2 25 02:46 Gemfile
-rw-r--r--@  1 mallow  staff   21251  2 25 02:41 Gemfile.jruby-1.9.lock
-rw-r--r--@  1 mallow  staff     589  2 25 02:41 LICENSE
-rw-r--r--@  1 mallow  staff   29292  2 25 02:46 NOTICE.TXT
drwxr-xr-x@ 10 mallow  staff     340  3  5 16:12 bin
drwxr-xr-x@  6 mallow  staff     204  3  5 16:12 config
drwxr-xr-x@  2 mallow  staff      68  2 25 02:41 data
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 lib
drwxr-xr-x@  6 mallow  staff     204  3  5 16:12 logstash-core
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 logstash-core-event-java
drwxr-xr-x@  4 mallow  staff     136  3  5 16:12 logstash-core-plugin-api
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 logstash-core-queue-jruby
drwxr-xr-x@  4 mallow  staff     136  3  5 16:12 vendor

4. 動作確認用のlogstash.conf作成

　この後の章で詳しく述べますが、Logstashはlogstash.confを読み込むことで
ファイルの取り込み元などを指定します。まずは動作確認用に次のコードをconfファイルへ記述します。

# logstash.confの作成
ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$ touch logstash.conf
# logstash.confの編集
ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$ vi logstash.conf

# 標準入力を受け付ける
input {
  stdin { }
}

# 標準出力を行う
output {
  stdout { codec => rubydebug }
}

ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$ ls -al
total 368
drwxr-xr-x@ 18 mallow  staff     612  3  5 16:19 .
drwxr-xr-x   4 mallow  staff     136  3  5 16:13 ..
-rw-r--r--@  1 mallow  staff  111569  2 25 02:41 CHANGELOG.md
-rw-r--r--@  1 mallow  staff    2249  2 25 02:41 CONTRIBUTORS
-rw-r--r--@  1 mallow  staff    3864  2 25 02:46 Gemfile
-rw-r--r--@  1 mallow  staff   21251  2 25 02:41 Gemfile.jruby-1.9.lock
-rw-r--r--@  1 mallow  staff     589  2 25 02:41 LICENSE
-rw-r--r--@  1 mallow  staff   29292  2 25 02:46 NOTICE.TXT
drwxr-xr-x@ 10 mallow  staff     340  3  5 16:12 bin
drwxr-xr-x@  6 mallow  staff     204  3  5 16:12 config
drwxr-xr-x@  2 mallow  staff      68  2 25 02:41 data
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 lib
drwxr-xr-x@  6 mallow  staff     204  3  5 16:12 logstash-core
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 logstash-core-event-java
drwxr-xr-x@  4 mallow  staff     136  3  5 16:12 logstash-core-plugin-api
drwxr-xr-x@  5 mallow  staff     170  3  5 16:12 logstash-core-queue-jruby
-rw-r--r--   1 mallow  staff      68  3  5 16:19 logstash.conf
drwxr-xr-x@  4 mallow  staff     136  3  5 16:12 vendor
ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$


5. 動作確認

　/bin下にあるlogstashスクリプトから、Logstashを起動します。
OSがWindowsの場合、同階層にlogstash.batが配置されているのでそちらを起動しましょう。
logstash.confにタイプミスがない場合、「Logstash startup completed」と出力されます。
コマンドプロンプトに好きな文字列を打ち込んでみましょう。
文字列がそのまま返り値として出力された場合、正しくセットアップできています。

ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$ /bin/logstash -f logstash.conf
-bash: /bin/logstash: No such file or directory
ishiiaoi-no-MacBook-Pro:logstash-5.2.2 mallow$ bin/logstash -f logstash.conf
Sending Logstash's logs to /Users/mallow/ELK_Stack/logstash-5.2.2/logs which is now configured via log4j2.properties
[2017-03-05T16:22:40,422][INFO ][logstash.setting.writabledirectory] Creating directory {:setting=>"path.queue", :path=>"/Users/mallow/ELK_Stack/logstash-5.2.2/data/queue"}
[2017-03-05T16:22:40,439][INFO ][logstash.agent           ] No persistent UUID file found. Generating new UUID {:uuid=>"ee122f82-747e-4d36-b851-322979edf3b6", :path=>"/Users/mallow/ELK_Stack/logstash-5.2.2/data/uuid"}
[2017-03-05T16:22:40,817][INFO ][logstash.pipeline        ] Starting pipeline {"id"=>"main", "pipeline.workers"=>4, "pipeline.batch.size"=>125, "pipeline.batch.delay"=>5, "pipeline.max_inflight"=>500}
[2017-03-05T16:22:40,858][INFO ][logstash.pipeline        ] Pipeline main started
The stdin plugin is now waiting for input:

# 標準入力で"hello world"を入力
hello world
{
    "@timestamp" => 2017-03-05T07:22:40.899Z,
      "@version" => "1",
          "host" => "ishiiaoi-no-MacBook-Pro.local",
       "message" => "hello world"
}

** Kibanaのインストール

　Kibanaも他ツールと同様、インストール方法が複数準備されています。
種類はElasticSearh・Logstashと変わらないため、そちらの章を参照してください。

*** zipファイルを用いたインストール

　ElasticSearhとLogstashのインストール方法に合わせるため、zipファイルを用いたインストール方法を選択しました。
こちらも、OSごとに手順に差はありません。

1. 公式サイトからzipファイルをダウンロード

　Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。
OSの種類によってzipファイルが異なるため、注意が必要です。

/Usersmallow/review/text/text/images/kibana_download.png

2. zipファイルの解凍

　ElasticSearhをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。

unzip kibana-5.2.2-darwin-x86_64.tar.gz

ishiiaoi-no-MacBook-Pro:~ mallow$ mv /Users/mallow/Downloads/kibana-5.2.2-darwin-x86_64 /Users/mallow/ELK_Stack/
ishiiaoi-no-MacBook-Pro:~ mallow$ cd ELK_Stack/
ishiiaoi-no-MacBook-Pro:ELK_Stack mallow$ ls -al
total 0
drwxr-xr-x   5 mallow  staff   170  3  5 16:35 .
drwxr-xr-x+ 46 mallow  staff  1564  3  5 16:30 ..
drwxr-xr-x@ 12 mallow  staff   408  3  5 15:49 elasticsearch-5.2.2
drwxr-xr-x@ 16 mallow  staff   544  2 25 02:38 kibana-5.2.2-darwin-x86_64
drwxr-xr-x@ 19 mallow  staff   646  3  5 16:22 logstash-5.2.2
ishiiaoi-no-MacBook-Pro:ELK_Stack mallow$


3. kibana.ymlの編集

　前にも述べた通り、KibanaはElasticSearhからデータを取得するためElasticSearhのURLを指定する必要があります。
kibana.yml内にURLを指定する箇所があるのでそちらを記述しましょう。

ishiiaoi-no-MacBook-Pro:kibana-5.2.2-darwin-x86_64 mallow$ cp -ap config/kibana.yml config/kibana.yml.org
ishiiaoi-no-MacBook-Pro:kibana-5.2.2-darwin-x86_64 mallow$ ls -al config/
total 32
drwxr-xr-x@  4 mallow  staff   136  3  5 16:42 .
drwxr-xr-x@ 16 mallow  staff   544  2 25 02:38 ..
-rw-r--r--@  1 mallow  staff  4412  2 25 02:38 kibana.yml
-rw-r--r--@  1 mallow  staff  4412  2 25 02:38 kibana.yml.org
ishiiaoi-no-MacBook-Pro:kibana-5.2.2-darwin-x86_64 mallow$

# kibana.ymlの編集
# The URL of the Elasticsearch instance to use for all your queries.
#elasticsearch.url: "http://localhost:9200"
elasticsearch.url: "http://localhost:9200"

4. 動作確認

　/binフォルダ下にあるkibanaスクリプトから起動します。（Windowsはkibana.batから起動します。）

ishiiaoi-no-MacBook-Pro:kibana-5.2.2-darwin-x86_64 mallow$ bin/kibana
  log   [07:46:54.934] [info][status][plugin:kibana@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.016] [info][status][plugin:elasticsearch@5.2.2] Status changed from uninitialized to yellow - Waiting for Elasticsearch
  log   [07:46:55.046] [info][status][plugin:console@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.900] [info][status][plugin:timelion@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.906] [info][listening] Server running at http://localhost:5601
  log   [07:46:55.907] [info][status][ui settings] Status changed from uninitialized to yellow - Elasticsearch plugin is yellow
  log   [07:47:00.943] [info][status][plugin:elasticsearch@5.2.2] Status changed from yellow to yellow - No existing Kibana index found
  log   [07:47:01.661] [info][status][plugin:elasticsearch@5.2.2] Status changed from yellow to green - Kibana index ready
  log   [07:47:01.662] [info][status][ui settings] Status changed from yellow to green - Ready


起動後、ブラウザにhttp://localhost:5601と入力します。図X.Xのような画面が見えていればインストールは完了です。

http://localhost:5601

/Usersmallow/review/text/text/images/kibana_setup.png

* データを集めて可視化しよう（Twitter編）

　「環境はセットアップできた！でもどうやってログを集めればいいんだっけ？
それにKibanaにアクセスしても何も出てこないよ？」

　おや？もふもふちゃんは大事なことを忘れてしまっているようです。
Kibanaにデータを表示するためには、ElasticSearhにデータが入っていないとダメですよね。

「確かにそうかも。今はElasticSearhの動作確認をしただけだから何もデータが入ってないなあ。
どうやってデータを取り込めばいいのかな？」

　…もふもふちゃんの頭からはLogstashの存在がかき消されているようです。
今回の構成では、ElasticSearhにデータを送るツールとしてLogstashを利用するのでしたね。
まずはLogstashの設定をもふもふちゃんと一緒にやってみましょう。

　目標は「Twitterのつぶやきから"イチゴメロンパン"という文言が含まれているものだけを抽出し
Kibanaで分析しやすい形にしてElasticSearhに送る」ことです。

** Logstashでデータを集めよう
　では、いよいよlogstash.confを編集していきます。
まずは、編集イメージを持つためにlogstash.confの構造をみてみましょう。

*** logstash.confの構造
　logstash.confはinput、filter、outputの3つにセクションが分かれています。
まずは図X.Xをみてください。

/Users/mallow/review/text/text/images/logstash_config.png

**** input
　inputは、ログをどこから取得するか決める部分です。
ログの取得間隔や、Logstashサービス再開時の挙動を指定することも可能です。

**** filter
　filterは、ログをどのように加工・整形するか決める部分です。
「加工」とはいっても、取り込んだログの文言を書き換える、条件に一致するログを消去する等
方法は様々です。

**** output
　outputは、ログをどこに送るか指定する部分です。
前の章でも述べた通り、ElasticSearhへのデータ送付以外にもCSV形式など
指定した拡張子でデータを出力することも可能です。

*** inputプラグインを書いてみよう
　では、早速Twitterのログを取り込んでみましょう。
図X.Xで囲った部分となります。

/Users/mallow/review/text/text/images/logstash_config_input.png

**** Twitter APIの準備
　まずはTwitter APiを準備しましょう。

1. URLにアクセス

　まずはTwitter APIのURLにアクセスします。
あらかじめTwitterアカウントにログインしておく必要があります。

https://dev.twitter.com

　アクセス後、「Manage Your Apps」リンクをクリックします。

/Usersmallow/review/text/text/images/Twitter_Developers01.png

2. 必要事項の入力

欄内の必要事項を記入します。

Name:アプリケーション名
Description:アプリケーションの概要
Website:アプリケーションを動かすためのトップページのURLを記載
（localhostなどのダミーデータは拒否されるため、自分のブログのアドレスなどを書けば良いです。）
Callback URL：ユーザ認証後の戻り先のURLアドレス

/Usersmallow/review/text/text/images/Twitter_Developers02.png
/Usersmallow/review/text/text/images/Twitter_Developers03.png
/Usersmallow/review/text/text/images/Twitter_Developers04.png

　登録に成功すると、次のような画面が表示されます。
/Usersmallow/review/text/text/images/Twitter_Developers05.png


3. 規約に同意して鍵をもらう

　情報登録が完了すると、鍵情報が表示されます。

Consumer Key (API Key):APIの認証に必要です。
Consumer Secret (API Secret):こちらも認証に必要です。他の人には教えてはいけません。


#@# 画面キャプチャを入れる（あと必要事項がわかったら追記する）

**** 情報の取得元を指定する
　いよいよlogstash.confの編集を行います。
/etc/logstash/logstash.confを開いて編集します。
　input部分では情報の取得元ごとにプラグインが提供されています。
今回はtwitter情報を取得するため、twitterプラグインを使う指定を行います。
先ほど取得したtwitterの鍵情報をlogstash.confに記載しましょう。

#@# コンフィグを書く（必要最低限の情報だけ）

この状態でLogstashを起動すると、このようにデータが取得できます。
#@# 返り値をかく
どうやら「イチゴメロンパン」以外のつぶやきも取れてしまっているようですね。

**** Twitterの検索キーワードを指定
　今回は「イチゴメロンパン」という文言が含まれるツイート数を数えたいので、"keyword"のオプション設定を利用します。

input{
  twitter{
    consumer_key => "Twitter APIのconsumer_key"
    consumer_secret => "Twitter APIのconsumer_secret"
    oauth_token => "Twitter APIのAccess Token"
    oauth_token_secret => "Twitter APIのAccess Token Secret"
    keywords => ["イチゴメロンパン"]
    full_tweet => "true"
  }
}

*** outputプラグインを書いてみよう
　欲しい情報を集めた後は、ElasticSearhに送りたいですよね。
図X.Xで囲った部分となります。

/Users/mallow/review/text/text/images/logstash_config_output.png

**** ログの送付先を指定する
　今回はElasticSearhにログを送付するため、ElasticSearhプラグインを利用します。
動作確認のため標準出力を可能にするstdoutプラグインも利用しましょう。
output内に複数のプラグインを同時記述することができます。括弧の閉じ忘れには気をつけてください。

output {
    elasticsearch{
    hosts => "http://localhost:9200/"
  }
}

*** 動作確認

　「filterプラグインなんて書く必要はあるのかなあ？」

#@# 挿絵を入れる

　もふもふちゃん、余裕をかましているようです。本当に大丈夫なのでしょうか。
大丈夫かどうか、動作確認してみましょう。
ElasticSearh→Logstash→Kibanaの順に起動します。

　サービスが開始されたら、Kibanaにアクセスします。

　「さっきと何も変わんないようー、エラーも出てないよ？」

　おっと、Kibanaでログを閲覧するためには、ElasticSearhとKibanaの情報を紐付ける必要があります。
詳細はKibanaの章で解説しますので、今はとにかくKibanaで情報が見えるようにしましょう。
図X.Xの画面キャプチャ通りに操作してください。

/Usersmallow/review/text/text/images/Kibana_create_index.png

　「見えたあ！」

　お、もふもふちゃんはうまく情報の紐付けができたようですね。紐付けができるとKibanaはこのような画面になっているはずです。

#@# キャプチャを差し込む

　「あら？なんかログがただ並んでいるだけのように見えるなあ。なんでえー？？」

　…やっぱり大丈夫じゃなかったみたいですね。filterプラグインはログを加工するだけでなく
データを綺麗に分割する役割も持ち合わせています。今はただ情報を取得して送付しているだけなので
取り込んだものがそのままデータ連携されてしまったのですね。

　「やっぱりサボっちゃダメなのかあー」

　そうですね。でもfilter部分の頑張り度とKibana画面の使いやすさ度は比例しますので
ここはもう一踏ん張りしてみましょう。

*** filterプラグインを書いてみよう
　今から編集する部分は、図X.Xで囲った部分となります。ここがlogstash.confの肝となる部分です。

/Users/mallow/review/text/text/images/logstash_config_filter.png

**** fieldとtextにデータをうまく分ける（ElasticSearhのデータの持ち方を説明）
　先ほどKibanaを閲覧したとき、データがうまく分かれていなかったと思います。
これはElasticSearhのデータの持ち方が「field」に対する「text」という構造になっているためです。
fieldとは、データベースでのカラムに相当します。textはカラムの中に入っている実データです。
カラムの中にデータを入れるのと同じように、field内にtextを保持します。
このfieldの集まりを「index」といいます。

#@# 解説の図を入れる

　つまり、ElasticSearh内のfieldごとにデータが入るようにログを取り込まないと
Kibanaでは1つのログの塊として認識されてしまい、データの区分けができないのです。
これではせっかくのKibanaの便利さも半減されてしまいます。
では、どのようにログを分割すれば良いのでしょう？

**** kvフィルタ：ログを分割する
　kvフィルタを使うことで、ログを分割することができます。
どのように分割されるのか、具体例を用いながらみていきましょう。
kvフィルタを利用する前は、このようにログが出力されます。

#@# kvフィルタ前のログ

　では、kvフィルタを追加してみましょう。

#@# kvフィルタをコンフィグに追加

　この状態でLogstashサービスを起動すると、このように出力されます。

#@# kvフィルタ後のログ

　「ログが分かれてる！」

　そうなのです、kvフィルタを追加するとログの区切りでログの種類：実際の情報といった形で
ログが分割できるのです。例えば「aaa：bbb」というログがあった場合、
「aaa」の部分はfield項目扱いに、「bbb」の部分はtext項目扱いとなります。
実際のログも区切り前がログの種類、区切り跡が実際の情報になりますからね。

**** removeフィルタ：いらない情報は捨てる

　「うーん、ログにはTwitterアカウント名が含まれているけれど、ちょっとこれは個人情報っぽいから
あまりログ情報としては持っておきたくないなあ」

　確かに、いらない情報や取得してはいけない情報は捨ててしまいたいですよね。
そんなときはremoveフィルタを使用することで、指定した条件に一致するログを「なかったこと」にできます。

#@# コードを入れる

　条件指定はif文を用いて行います。if文のあとにfield名を指定し、
イコールの後に削除したいログの具体的な名称を記載します。
条件指定には正規表現を用いることができるので「○○に当てはまるものは全て」といった
指定も可能です。前に記載したlogstash.confを用いて、dropフィルタを使用する前と後を比較してみましょう。

#@# dropフィルタを利用する前

#@# dropフィルタを利用した後

本当になくなってしまいましたね…。

**** timestampフィルタ：ログが出力された時刻を編集する

#@# 忘れたので調べる

*** 最後に動作確認

　「よーし、いい感じにlogstashフィルタがかけたよ！一度テキストに出力して動作確認してみよう！」

　お、もふもふちゃん、logstash.confが一通り書けたようですね。もふもふちゃんのいう通り、
テキストなどに出力して結果を確かめるのは良いことです。

　いきなりElasticSearchに連携してしまうと、意図していないfieldに分割されてしまったのでやり直したいとなったときに
無駄なデータを溜めてしまうことになります。ElasticSearchからデータを削除するためには
削除用のクエリをElasticSearchに直接投げる必要があります。あまり面倒なことはしたくないですよね。
なので、一度コンフィグが書けたらどこかに出力して動作確認するようにしましょう。
面倒であれば、標準出力でも良いと思います。

#@# 全部盛りのlogstash.confを記載

ちなみに、logstash.confのコメントアウトは「#」を利用して行います。
このコンフィグを起動するとこのようにログが取り込まれます。

#@# 実際のログを貼り付け

　「お？結構いい感じかも！」

　確かに、「イチゴメロンパン」というつぶやきのログだけ取り込めました。fieldごとに情報が分かれているのも便利そうです。

　「これ、Kibanaでみたらどんなになるのかな？」

　そうですね、このままよりはKibanaで閲覧できた方が便利です。早速データ連携してみましょう。

* 集めたデータをみてみよう（Twitter編）

　集めたデータを見る前に、ElasticSearch→Logstash→Kibanaの順にサービスを起動しておきましょう。
無事、サービス起動できたらKibanaにアクセスします。

** KibanaのUI
　これまでの章で何回かKibanaの操作が出てきましたが、詳細な説明を省略していました。
これ以降はKibanaの具体的な使い方を解説していきます。

#@# キャプチャを入れる

　「なんかいっぱい押すところがあるよ？」

　…もうちょっといい感想はないのでしょうか？

　とはいえもふもふちゃんのいう通り、Kibanaにはいくつか機能がついています。
一つずつ紹介していきます。

#@# もふちゃんを入れる

*** Discover：データの詳細を閲覧する

#@# Discover画面のキャプチャ

　DiscoverはElasticSearchのindex内に保持されている生データを閲覧できる箇所です。
fieldごとに分割されたログの詳細をはもちろん、fieldごとのデータのサマリ情報や
何時頃そのログが出力されたか棒グラフを用いて確認することもできます。

*** Visualize：データを使ってグラフを作成する

#@# Visualizeのキャプチャを入れる

　取り込まれたログを使って棒グラフや折れ線グラフを作成する場所です。
グラフは保存しておくことができるので、昔作ったログを呼び出して参照することも可能です。
ちなみにですが、グラフの保存先はElasticSearchの中です。

*** Dashboard:グラフを集めて閲覧する

#@# Dashboardのキャプチャを入れる

　Visualizeで作成したグラフを一箇所にまとめて参照することができます。
Googleの画像検索で出てくるKibanaの画面は、Dashboardの画面が使われることが多いです。
やはりグラフが集まっていると見栄えも良いですよね。

*** Settings：各種設定画面
#@# Settingsのキャプチャを入れる

　ElasticSearchのindex情報が更新されたときや、必要なくなったグラフを削除したいときに使用します。
名前の通り設定画面です。

** Discover画面を使ってみよう

#@# キャプチャを貼る

　「なんか棒グラフが並んでるね〜、下がログなのかな？でも、三角のビックリマークが出てるよ？」

　もふもふちゃんの言う通り、Discover画面をみていると三角のマークが出ています。
一体これは何でしょう？

#@# 拡大したキャプチャを貼る

　「それにログはみやすいけどさあ、傾向とかすぐ調べたいよねー。これ全部開けて中身見ないといけないのかな？」

　確かにそうですね。中身を全部見ていかないといけないのであれば、生ログとにらめっこするのと何ら変わりがありません。
ざっくり傾向を見たいときはどうすれば良いのでしょう？
そんな疑問をこの章で解決していきましょう。

#@# 挿絵をはる

*** indexの紐付け

#@# 拡大したキャプチャを貼る

　まず、この三角マークを何とかしましょう。これはKibanaの仕組みに原因があります。
KibanaはElasticSearchのindexからデータを取得していますが、ElasticSearchは複数indexを持つことができます。
Kibanaで利用するindexを指定してあげないと、どの情報を出せばいいのかわかりません。

　なので、Logstashの動作確認時に一度どのindexを使うのか指定したのです。
その後Logstashの設定を変更したことにより、新たにfield部分が追加されてしまいました。
でも、Kibana側は古いindex情報を使っているので、「連携されてきたログは何だか別れているけれど、そんな情報は聞いてないぞ？？」と
なってしまうのです。

　そこで、新しいfieldを追加した後は新しいindexの情報を取得しないとエラーになってしまうのです。
これを行わないとKibanaで正しく検索ができなくなるので忘れないようにしましょう。

#@# 図を書いて入れる

では、実際にindexの紐付けをおこないます。まず「Settings」画面を開きます。

#@# キャプチャを入れる

次に、紐付けるindexを選択します。

#@# キャプチャを入れる

indexの情報を見せるための時間軸を洗濯します。
基本はログの中に時間の情報（@timestamp）を保持しているため、そちらを選択します。

#@# キャプチャを入れる

#@# キャプチャを入れる

これで紐付けは完了です。

*** 生ログをみてみよう
　では、実際にログを閲覧してみましょう。まずはDashboardの使い方から解説します。

#@# キャプチャを入れる

　画面左上では、現在何件のデータが取得されているのか表示されます。
その下にある白い枠の中にデータの検索条件を入力し、Enterキーか虫眼鏡マークをクリックすると
検索を行うことができます。検索条件の具体的な記法は後ほど解説します。

#@# キャプチャを入れる

　普段はこのように、データの詳細は折りたたまれています。これではせっかく取り込みをしたログを
全部表示することができません。そこで、各ログの左上にある三角マークをクリックします。

#@# キャプチャを入れる

　すると、ログの詳細が全部表示されました。いくつか閲覧してみると、どうも言語が日本語のものが多いようですね。
本当に100%日本語なのかみたいですよね。

*** ログの傾向をみてみよう
　では、今度はログの傾向を閲覧してみましょう。Kibanaの左側にあるInsuranceを利用します。
このInsuranceは直近500件分のログを分析し、各fieldに入っているデータがどのくらい同じなのか
割合で示すことができます。

#@# キャプチャを入れる

　例えば、「XX」fieldではログの傾向が〇〇だと一目でわかります。
各データ横にある虫眼鏡をクリックすると、その条件を自動で追加することができます。
プラスの虫眼鏡を選択すると「そのデータに一致する条件で検索」、マイナスは「そのデータに一致しない条件で検索」となります。

#@# キャプチャを入れる

検索条件を外したい場合、枠内のチェックボックスをクリックすると条件指定の解除が可能です。

#@# キャプチャを入れる

虫眼鏡をクリックすることで種類を切り替えることが可能です。

#@# キャプチャを入れる

また、ゴミ箱をクリックすると、検索条件を完全に削除してしまうことができます。

#@# キャプチャを入れる

*** 自分で検索してみよう
　ある程度はKibanaの機能を使って取得したい情報を絞ることは可能です。
ただし、自分で検索条件を指定してデータを絞りたいときはあると思います。
特にグラフを作成する場合はあらかじめ検索条件でデータ量を指定しないと
余計な情報もグラフに表示されてしまいます。
検索条件を新規に入力したい場合、画面右上のNewボタンをクリックします。

#@# キャプチャを入れる

**** 条件に一致するものを取り出す
　基本的にはfield名を指定し、その後field内のデータを指定する方法をとります。
例えばfield名「つぶやき」に、「イチゴメロンパン」と記載されている情報に絞って
ログを取得したい場合、検索条件は下記のようになります。

#@# 検索条件をを入れる

　実際に検索をかけるとこのようになります。

#@# キャプチャを入れる

　実際に内訳をみると、検索条件通りのログが残っていることがわかります。

**** 条件に一致しないものを取り出す
　今度は条件に一致しないものは件数としてカウントされないようにしましょう。
例えばfield名「つぶやき」に、「イチゴメロンパン」と記載されている情報は除外して
ログを取得したい場合、検索条件は下記のようになります。

#@# キャプチャを入れる

　こちらも内訳をみると、検索条件通りのログが取得できているようです。

**** 複数検索条件を指定する
　では、今度は複数の検索条件を指定してみたいと思います。

**** AかつBの場合を検索する
　この場合、検索条件をANDで繋いで指定します。1つのログにAとBの条件に当てはまるものが対象となります。
実際にやってみると次のようになります。

#@# 検索条件を入れる

　内訳はこのような感じになります。

#@# キャプチャを入れる

**** AまたはBの場合を検索する
　この場合、検索条件をORで繋いで指定します。

#@# 検索条件を入れる

　内訳はこのような感じになります。

#@# キャプチャを入れる

*** 検索期間を変えてみよう
　Discover画面の右上を見て下さい。時計マークの隣に「Last 15 minutes」と記載されています。
つまり、今Kibanaに表示されているログは全て15分前のログのみとなります。

#@# キャプチャを入れる

　「それじゃー意味ないじゃん！」

　もふもふちゃんの言う通り、このままだと直近のログしか閲覧することができません。
そこで、「Last 15 minutes」にマウスを合わせてクリックします。

#@# キャプチャを入れる

　すると、検索時間が選べることがわかります。試しに「Last 7 days」をクリックしてみましょう。

#@# キャプチャを入れる

　検索期間が広がったのがわかるでしょうか。Quickタブからはこのように、ざっくりとした検索期間を指定することが可能です。

　「何分前とかは指定できないの？」

　可能ですよ。Absoluteタブを選択して下さい。

#@# キャプチャを入れる

　時間の単位をプルダウンから選択し、遡りたい期間を数字で入力します。Goボタンをクリックすることで
指定した条件で再検索することができます。灰色のボタンをクリックすると、ミリ秒以下を切り捨てることができます。

#@# キャプチャを入れる

　「じゃあさ、12月から3月の間とかは指定できるの？」

　こちらも可能です。**タブを選択して下さい。

#@# キャプチャを入れる

　すると、カレンダーが表示されます。検索を始めたい期間と終わりの期間をを数字で入力するか、
カレンダーからクリックして選択します。Goボタンを押すことで検索を開始することが可能です。
「Set to Now」をクリックすると、現在の時間に検索終了期間を合わせることができます。

*** 検索条件を保存しよう

　「色々検索したけどさ、これ毎回設定するのはめんどくさいね…。使い回しはできないのかな？」

　もふもふちゃんの言う通り、ボタンを使って検索条件を絞るにしろ、自分で検索条件を入力するにしろ
毎回条件指定を行うのは面倒ですね。しかし、Kibanaは検索条件を保存することができます。
画面右上のフロッピーディスクアイコンをクリックして下さい。

#@# キャプチャを入れる

　すると、名前を入れる欄が出てきます。好きな名前を入れてSaveボタンを押しましょう。

#@# キャプチャを入れる

　保存できました。左隣のボタンを押して、一旦検索条件を指定していない状態に戻してみます。

#@# キャプチャを入れる

　では、先ほど保存した検索条件を再度指定してみましょう。フロッピーディスクアイコンの右隣にある
フォルダーアイコンをクリックします。

#@# キャプチャを入れる

　すると、先ほど保存した検索条件の名前が出てくるので選択します。

#@# キャプチャを入れる

　検索条件の呼び出しができました。同じ名前で検索条件の詳細をアップデートしたい場合、
条件を書き変えて検索実行をした後、フロッピーディスクのボタンから上書き保存することができます。

** Visualize画面を使ってみよう

　「ログの詳細を検索で出せるのはわかったけど、Googleの画像とかで出てくるのは
もっとかっこいいグラフだけどな？」

　確かにそうですね。次はグラフを作ってみましょう。
Visualizeタブからグラフを作成します。まずはクリックしてみましょう。

*** グラフの種類を選ぶ

#@# キャプチャを入れる

　クリックすると、グラフの種類を選択する画面が出てきます。
種類は複数選択することができます。

**** エリアチャート
　データ件数の数だけ塗りつぶして表示されます。SFの映画とかによく出てきそうなグラフですね。

#@# キャプチャを入れる

**** 棒グラフ
　データ件数の数だけ棒の長さが長くなるグラフです。

#@# キャプチャを入れる

**** 折れ線グラフ
　データの数に応じて点がプロットされ、それを線でつないだグラフです。
データが存在している期間がばらけていない場合、点しか表示されないのでグラフとしては
見づらいときもたまにあります。

#@# キャプチャを入れる

**** 円グラフ
　データの内訳に応じて円が分かれていきます。円を分ける条件を指定しないと
データの総件数が表示されるだけなので、複雑な設定を一緒に行うことが基本となります。
工夫次第でドーナツ型のグラフを作ることができます。1番見栄えするグラフです。

**** 表
　fieldの中にあるカラムが何件あるか、数を表示することができます。
イメージとしては簡易型のExcelが近いです。
データはCSV形式でダウンロードすることができます。ただし、文字コードはUTF-8なので
Excelで開くと文字化けします。

#@# キャプチャを入れる

**** Markdown
　Markdownを表示することができます。
URLのリンクを記載したり、グラフの閲覧方法をメモ書きで残すことが可能です。

#@# キャプチャを入れる

**** 世界地図
　データの送信元情報がログなどに含まれていた場合、その情報がどこから来たのか
世界地図にプロットすることが可能です。
この世界地図の情報ですが、世界地図のマップ情報はElastic社のサーバから取得しているため
インターネット環境がないと何もすることができません。
ちなみにKibana3,4などは世界地図情報の取得元サイトがサービスを終了してしまったため
地図グラフを使うことはできなくなってしまいました…。

#@# キャプチャを入れる


では、今度こそグラフを作成しましょう。
好きなグラフを選択します。今回は円グラフを選択しました。
画面の下はすでに作成したグラフを呼び出す箇所です。

#@# キャプチャを入れる

*** 件数の数え方を指定する
　すると、選択した種類のグラフが作成されます。
初めは検索条件などを指定していないため、データ総数がグラフにプロットされます。

#@# キャプチャを入れる

　画面左側を操作することで、データの数え方やデータの内訳方法を指定することができます。
まずは数え方を変更してみましょう。

#@# キャプチャを入れる

　画面左上の部分をクリックします。Countはデータ件数をそのまま数える設定です。
設定の内訳は次の表を参考にしてください。ただし、Count以外はデータの中に数値型が含まれていないと
選択することができません。

#@# 表を作って入れる

*** 詳細な情報ごとに色分けする

　「いろいろ数字が取れるようになったのは便利だけど、Google画像検索とかで出てくるみたいに
色分けするにはどーしたらいいのー？」

　確かにもふもふちゃんの言う通り、今はただの緑色の円があるだけです。情報の内訳を表示することはできるのですが、
今は内訳の表示方法を指定できていません。よってデータ数しか表示されないのです。
早速指定してみましょう。

#@# キャプチャを入れる

　まずは、データの内訳方法を決めます。指定できる内訳方法は次の表を参照してください。

#@# 表を作って入れる

　今までの設定で作成したfieldごとにデータを分ける場合、Termsを選択します。
詳細設定欄でfield名が選択できるようになりますので、そちらで好きなfieldを選択します。

#@# キャプチャを入れる

　これで色ごとに情報の内訳を表示することができるようになりました。
画面右上に色に対応したfield名が表示されます。マウスを当てるとそのfieldのみ強調することが可能です。
グラフにマウスを当てるとfield名とデータ件数と全体に占める件数の割合が表示されます。

**** .rawとはなんぞや？（区切られた項目を正しく数える）

　「困ったなー、なんかうまく内訳が表示されないや」

#@# キャプチャを入れる

　確かによく見ると、「XX」という文言が重複しています。field名で分けるように指定しているはずですが、なぜでしょうか？
理由は.rawの存在にあります。

　field名を内訳の対象として指定した場合、field内のデータにハイフン（-）やドット（.）が存在していると
別のデータとしてカウントされます。今回はfield内にドットが含まれていたため、「」と「」としてデータがカウントされてしまい
結果としてデータ数の整合性が取れていなかったのです。

　これを解消するために、field名.rawとなっているTermsを指定します。

#@# キャプチャを入れる

#@# キャプチャを入れる

　これで正しくデータを数えることができました。

*** オプションを指定する
　1つのデータに対して内訳を表示することはできましたが、2種類以上のデータを同じグラフに表示することも可能です。
Add Sub buckets欄をクリックすると、先ほど同じ設定画面が表示されます。あとは同じようにデータのカウント方法・
内訳方法を指定するだけです。

　また、これとは別にグラフの見せ方に関してオプションを指定することができるので、詳細を解説します。
（文字数によって追加するか決める）

*** 保存する
　グラフを作成後、保存せずにブラウザを閉じた場合は再度作成し直しとなります。
必要に応じてグラフを保存しましょう。

　まずは一度緑の三角ボタンをクリックし、設定をグラフに反映します。

#@# キャプチャを入れる

　次に画面右上のフロッピーディスクをクリックし、グラフ名を入力します。
Saveボタンをクリックすると保存完了となります。操作の流れは検索条件を保存するときと同じです。

#@# キャプチャを入れる

　保存したグラフを表示するときは、フォルダーアイコンをクリックします。

#@# キャプチャを入れる

** Dashboard画面を使ってみる

　「グラフがたくさんできたけど、同じ画面に並べることはできないのかな？」

　そうですね。Visualize画面では1度に表示できるグラフは1個だけでした。
作成したグラフを1つのページで全て閲覧できれば便利ですよね。

　Dashboard画面はグラフを1つにまとめる役割を持ちます。
まずは1つ作成してみましょう。

*** グラフを並べる
　始めに画面Dashboardボタンをクリックし、画面を切り替えます。

#@# キャプチャを入れる

　まずは使いたいグラフを表示させます。画面右上のプラスアイコンをクリックし、
Visualizeタブから保存したグラフ名を選択します。

#@# キャプチャを入れる

　このときにSearchタブを選択し、保存した検索条件を選択すると
簡易版のDiscover画面を表示することができます。生データを参照したい場合は便利です。

#@# キャプチャを入れる

*** グラフの大きさを指定する
　使いたいグラフを決めたあとはグラフを配置していきます。基本はドラック&ドロップで
グラフを配置していくだけです。

#@# キャプチャを入れる

　グラフの大きさを変更したい場合はグラフ右下をクリックしたままドラック&ドロップで
大きさを調整します。ある程度は自動で大きさが決まってしまうので、ミリ単位の調節はできません。

#@# キャプチャを入れる

*** 保存する（検索期間を保持する/しないを選択する）

#@# キャプチャを入れる

　最後に、作成したDashboardを保存します。保存画面は他と同様フロッピーディスクアイコンから
表示することができますが、1点他のグラフと違う点があります。それは「現在設定している検索期間も同時に保存するか？」と言う点です。
Stoar with dateにチェックをつけずに保存すると、Dashboardを開き直したときはデフォルトの検索期間が表示されます。

#@# キャプチャを入れる

#@# キャプチャを入れる

　逆にチェックを入れて保存すると、Dashboard保存時の検索期間も適用した状態で
Dashboardを開くことができます。毎回決まった検索期間を指定するような使い方をしたい場合は
チェックを入れておくと良いでしょう。

#@# キャプチャを入れる

#@# キャプチャを入れる

* 終わりに
　頑張ってKibanaのDashboard画面を作ったもふもふちゃん。上司の人にはDashboardリンクを共有して
グラフの参照方法を共有したので面倒な仕事を頼まれることもなくなりました。

　「いやーよかったよかった！ほとんどリアルタイムで最新情報を表示できるから
メンテナンスもちょっとでいいし楽ちんだよ！」

　それはよかったですね。新しい情報をKibanaで表示したくなった場合、Logstashの取り込み元を
新しく指定すれば良いですよね。サーバスペックなどを監視する場合は今回は取り上げなかったBeatsを
利用してデータ収集することもできます。

　今回取り上げたTwitter情報を分析する、という利用方法はただの一例です。今手元にあるデータとElastic Stackを
組み合わせればデータ分析の可能性は無限大に広がります。それでは楽しいElastic Stackライフを！
