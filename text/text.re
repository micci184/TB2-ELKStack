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
#@# クラスターを解説した図を入れる

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

#@# データの流れ図を入れる

　また、Kibanaは起動前にElasticSearchのURLを指定する必要があるため、ElasticSearchがセットアップされていないと
そもそも動作しません。今回は各ツールごとに完結できる動作確認方法を取りますが、万が一に備えるという意味では
図X.Xのようにデータの連携先→連携元→データ閲覧ツールという流れでセットアップした方が良いでしょう。

#@# インストールの順番図を入れる

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

#@# サイトのキャプチャを入れる

2. 作業用ディレクトリを作成（好みで良いです）

　今回は検証用の環境構築なので、アンインストールを簡単にするために作業用ディレクトリを作成します。

#@# ディレクトリ作成用コードを入れる

3. zipファイルの解凍

　ディレクトリ内にダウンロードしたzipファイルを解凍します。

#@# コードを入れる

4. ElasticSearhの起動

　/binディレクトリ下にあるelasticsearchスクリプトを実行し、サービスを起動します。
OSがWindowsの場合、elasticsearch.batを実行します。

#@# コードを入れる

5. 動作確認

　ElasticSearuへクエリを投げ、返り値があるか確認します。

#@# コードを入れる

もしくはGoogle Chrome上でURLにアクセスし、同じような返り値があればOKです。

#@# 画面キャプチャを入れる

返り値が帰ってくれば、正常にインストールできています。

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

#@# サイトのキャプチャを入れる

2. zipファイルの解凍

　ElasticSearhをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。
ディレクトリ構成は一例です。

#@# コードを入れる

4. 動作確認用のlogstash.conf作成

　この後の章で詳しく述べますが、Logstashはlogstash.confを読み込むことで
ファイルの取り込み元などを指定します。まずは動作確認用に次のコードをconfファイルへ記述します。

#@# コードを入れる（logstash.confを書くところ全部入れる）

5. 動作確認

　/bin下にあるlogstashスクリプトから、Logstashを起動します。
OSがWindowsの場合、同階層にlogstash.batが配置されているのでそちらを起動しましょう。
logstash.confにタイプミスがない場合、「Logstash startup completed」と出力されます。
コマンドプロンプトに好きな文字列を打ち込んでみましょう。
文字列がそのまま返り値として出力された場合、正しくセットアップできています。

#@# コードを入れる（/bin/logstash -f logstash.conf)

** Kibanaのインストール

　Kibanaも他ツールと同様、インストール方法が複数準備されています。
種類はElasticSearh・Logstashと変わらないため、そちらの章を参照してください。

*** zipファイルを用いたインストール

　ElasticSearhとLogstashのインストール方法に合わせるため、zipファイルを用いたインストール方法を選択しました。
こちらも、OSごとに手順に差はありません。

1. 公式サイトからzipファイルをダウンロード

　Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。
OSの種類によってzipファイルが異なるため、注意が必要です。

#@# サイトのキャプチャを入れる

2. zipファイルの解凍

　ElasticSearhをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。

#@# コードを入れる

3. kibana.ymlの編集

　前にも述べた通り、KibanaはElasticSearhからデータを取得するためElasticSearhのURLを指定する必要があります。
kibana.yml内にURLを指定する箇所があるのでそちらを記述しましょう。

#@# コードを入れる（kibana.ymlの編集）

4. 動作確認

　/binフォルダ下にあるkibanaスクリプトから起動します。（Windowsはkibana.batから起動します。）

#@# コードを入れる（起動）

起動後、ブラウザにhttp://localhost:5601と入力します。図X.Xのような画面が見えていればインストールは完了です。

#@# キャプチャを入れる

* データを集めて可視化しよう（Twitter編）

　「環境はセットアップできた！でもどうやってログを集めればいいんだっけ？
それにKibanaにアクセスしても何も出てこないよ？」

#@# 挿絵入れる

　おや？もふもふちゃんは大事なことを忘れてしまっているようです。
Kibanaにデータを表示するためには、ElasticSearhにデータが入っていないとダメですよね。

「確かにそうかも。今はElasticSearhの動作確認をしただけだから何もデータが入ってないなあ。
どうやってデータを取り込めばいいのかな？」

　…もふもふちゃんの頭からはLogstashの存在がかき消されているようです。
今回の構成では、ElasticSearhにデータを送るツールとしてLogstashを利用するのでしたね。
まずはLogstashの設定をもふもふちゃんと一緒にやってみましょう。

　目標は「Twitterのつぶやきから"イチゴメロンパン"という文言が含まれているものだけを抽出し
Kibanaで分析しやすい形にしてElasticSearhに送る」ことです。

** Logstashのフォルダ階層←蛇足感があるので削ってもいいかも？
　まずはLogstashの階層構造を押さえておきましょう。
基本的にはLinux用のツールと同じような階層構造になっています。

#@# treeコマンドで階層構造をだす

　よく扱うディレクトリのみピックアップして解説します。

*** /etc
　Logstashがどのようにログを取り込むか決定するlogstash.confなど、設定系ファイルが格納されています。
基本的にはこちらのディレクトリを触ることが多いでしょう。

#@# treeコマンドで階層構造をだす

*** /opt
　Logstashの起動用スクリプトが配置されています。起動・停止はこちらの/binディレクトリ下にあるスクリプトを実行します。
動作確認時も何回か扱いましたね。

*** /var
　Logstashのログがこちらのディレクトリに配置されます。コンフィグが間違っているときや
Logstashの動作に問題が発生した場合はこちらのログファイルを参照してみましょう。
ログにはlogstash.logとlogstash.errの2種類があります。logstash.logは動作の停止ログなど
動作情報も出力されます。logstash.errは深刻な問題の場合のみ出力されるので、基本的にはlogstash.logを
参照すれば良いです。

　logstash.logは日付ごとに自動で改廃されます。最大1週間分保持されますが、日付が2日以上経ったものは
tar.gzファイルとして圧縮されます。

** Logstashでデータを集めよう
　では、いよいよlogstash.confを編集していきます。
まずは、編集イメージを持つためにlogstash.confの構造をみてみましょう。

*** logstash.confの構造
　logstash.confはinput、filter、outputの3つにセクションが分かれています。
まずは図X.Xをみてください。

#@# 僕の自慢の図をわかりやすくして入れる

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

#@# 僕自慢の図に枠をつけて差し込む

**** Twitter APIの準備
　まずはTwitter APiを準備しましょう。

1. URLにアクセス

　まずはTwitter APIのURLにアクセスします。

#@# URLを挿入

2. 必要事項の入力

欄内の必要事項を記入します。

#@# 画面キャプチャを入れる（あと必要事項がわかったら追記する）

3. 規約に同意して鍵をもらう

　情報登録が完了すると、鍵情報をダウンロードすることができます。
後ほど必要になるため、控えを取っておきましょう。

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

#@# コンフィグをかく（該当部分だけ）

検索条件の指定方法はTwitter APIの記法を流用しています。詳細は公式サイトを閲覧すると良いでしょう。

この状態でLogstashを起動すると、このようにデータが取得できます。
#@# 返り値をかく
今度は「イチゴメロンパン」の情報だけ取得できました。

#@# 必要に応じて詳細を追記。

*** outputプラグインを書いてみよう
　欲しい情報を集めることができたので、今度はログをElasticSearhに送ってみましょう。
図X.Xで囲った部分となります。

#@# 僕自慢の図に枠をつけて差し込む

**** ログの送付先を指定する
　今回はElasticSearhにログを送付するため、ElasticSearhプラグインを利用します。
動作確認のため標準出力を可能にするstdoutプラグインも利用しましょう。
output内に複数のプラグインを同時記述することができます。括弧の閉じ忘れには気をつけてください。

#@# コードを記載する。

#@# 必要に応じて内容を追記する。

*** 動作確認

　「filterプラグインなんて書く必要はあるのかなあ？」

#@# 挿絵を入れる

　もふもふちゃん、余裕をかましているようです。本当に大丈夫なのでしょうか。
大丈夫かどうか、動作確認してみましょう。
ElasticSearh→Logstash→Kibanaの順に起動します。

#@# コードをみる

　サービスが開始されたら、Kibanaにアクセスします。

#@# 画面キャプチャを入れる

　「さっきと何も変わんないようー、エラーも出てないよ？」

　おっと、Kibanaでログを閲覧するためには、ElasticSearhとKibanaの情報を紐付ける必要があります。
詳細はKibanaの章で解説しますので、今はとにかくKibanaで情報が見えるようにしましょう。
図X.Xの画面キャプチャ通りに操作してください。

#@# 画面キャプチャ（手順つき）を入れる

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

#@# 僕自慢の図に枠をつけて差し込む

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
*** グラフの種類を選ぶ
*** 件数の数え方を指定する
*** 詳細な情報ごとに色分けする
**** .rawとはなんぞや？（区切られた項目を正しく数える）
*** オプションを指定する
*** 保存する
** Dashboard画面を使ってみる
*** グラフを並べる
*** グラフの大きさを指定する
*** 期間はどうする？（検索期間を保持する/しないを選択する）
*** 保存する

* 終わりに



　それでは楽しいElastic Stackライフを！（ラストはこれでしめる）
