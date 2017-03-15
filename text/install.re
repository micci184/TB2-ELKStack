= 環境構築（各ツールのインストール）

//lead{
「よーし、ELKがどんなものかだいたい理解できたから、インストールしてみよ！
…あれ、なんかインストール方法もいっぱいあるみたい。どれを選べばいいのかなあ？」
//}

あら？もふもふちゃん、今度はインストールで詰まってしまったようです。
公式サイトからダウンロードでzipファイルを落としてくることができるようですが、他のやり方もあるようです。
状況に合っている1番いいやり方を選択したいですよね。一緒にみてみましょう。

== インストールの順番
インストールの前に、どのツールからインストールするかを決めておきましょう。ELKのデータの流れを考えると、
Elasticsearch→Logstash→Kibanaの順にインストールすることをお勧めします。
理由は図X.Xにあるように、Logstashで取りこんだデータをElasticsearchに連携するため
先にデータの連携先をセットアップしておかないと正しく動作確認ができない可能性があるためです。

//image[data][データ連携の流れ]{
  配置場所
  /Users/mallow/review/text/text/images/data.png
//}

また、Kibanaは起動前にElasticsearchのURLを指定する必要があるため、Elasticsearchがセットアップされていないと
そもそも動作しません。今回は各ツールごとに完結できる動作確認方法を取りますが、万が一に備えるという意味では
図X.Xのようにデータの連携先→連携元→データ閲覧ツールという流れでセットアップした方が良いでしょう。

//image[install][セットアップの順番]{
  配置場所
  /Users/mallow/review/text/text/images/data.png
//}

== Elasticsearchのインストール
先にも述べましたが、インストール方法は複数準備されています。導入の目的とご自身の環境に合わせて
ベストなものを選択すると良いでしょう。

=== とにかく使ってみたい場合（Linux）：zipファイル

「とにかくどんなものか試してみたい！」そんな方はzipファイルを公式サイトからダウンロードしてきましょう。
インストール方法も適当なフォルダにzipファイルを解凍するだけなので導入は簡単に終わります。
ただし、serviceコマンドは付属しないため、長期的な運用を考えている場合には向かないインストール方法です。

=== ちゃんと運用もしたい場合（rpmパッケージを利用するLinux）：rpmパッケージ
Elastic公式から提供されているrpmパッケージを利用した場合、serviceコマンドが自動的にダウンロードされます。
また、各種設定ファイルやディレクトリ構造はLinuxのディレクトリ形式に合わせて構築されます。
運用を検討している場合は、初めからこちらのやり方を取っておけば環境の再構築を防ぐことができます。

例）
* コンフィグファイルの配置先：/etc/Elasticsearch/
* ログファイルの出力先：/var/log/Elasticsearch
* 基本的なファイルの配置先：/opt/Elasticsearch

=== ちゃんと運用もしたい場合（debパッケージを利用するLinux）：debパッケージ
こちらも2のやり方と同様です。違いはUbuntu系のLinux用パッケージを使うか、
RedHat、OpenSuSE系のLinux用パッケージを使うかだけです。

=== とにかく使ってみたい場合かつ、Docker実行環境がある場合：Dockerコンテナ
Elastic StackをDocker社が利用していることもあってか、Elastic公式からDockerイメージが提供されています。
手っ取り早く試してみたい場合、かつDockerコンテナの実行環境がある場合は素直にコンテナを利用した方が良いでしょう。
ただし、インストール方式の2.3に移行を考えている場合、構成がかなり変わるので初めからパッケージ利用での環境構築方法を選択した方が良さそうです。

また、Elasticsearchは物理的にも、Javaのヒープメモリもかなり消費するツールです。
Dockerコンテナ上ではあまり性能が出ないため、大量のデータを流す予定がある場合はコンテナ利用を避けた方が良いです。

=== Puppet、Chef、Ansibleの実行環境がある場合：Githubのレシピを利用
Elastic公式から各ツールのレシピが共有されています。Githubにアップロードされているため
そちらを用いて環境を構築することも可能です。

=== Windows/Mac上に構築する場合：zipファイル
Windowsの場合、インストール方法はzipファイル一択となります。Macはbrewコマンドを用いることで
インストールすることもできますが、Elastic公式ではサポートされていないようなので、
今回はインストール方法から対象外としています。

他の方が検証されているログを見る限り、brewコマンドでインストールした場合はtar.gzパッケージを解凍して
インストールしているように見えるため、公式の最新ファイルをダウンロードしてインストールするのと特に変わらないと思います。
好みで選択すると良いでしょう。

== zipファイルを用いたインストール（Elasticsearch）

今回は6のzipファイルを用いたインストール方法を取りたいと思います。
zipファイルを用いたインストールの場合、OSが違っていても基本的な手順は同じです。


=== 公式サイトからzipファイルをダウンロード

Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。

@<href>{https://www.elastic.co/jp/downloads/elasticsearch}

//image[Elasticsearch_download][Elasticsearchのダウンロード]{
/Users/mallow/review/text/text/images/Elasticsearch_download.png
//}

=== 作業用ディレクトリを作成（好みで良いです）

今回は検証用の環境構築なので、アンインストールを簡単にするために作業用ディレクトリを作成します。

//cmd{
$ mkdir ELK_Stack
//}

=== zipファイルの解凍

ディレクトリ内にダウンロードしたzipファイルを解凍します。

//cmd{
# 必要であれば
$ unzip Elasticsearch-5.2.2
//}


=== Elasticsearchの起動

@<tt>{/bin}ディレクトリ下にある@<tt>{Elasticsearch}スクリプトを実行し、サービスを起動します。
OSがWindowsの場合、@<tt>{Elasticsearch.bat}を実行します。

//cmd{
$ Elasticsearch-5.2.2 mofumofu$ bin/Elasticsearch
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
//}

=== 動作確認

Elasticsearchへクエリを投げ、返り値があるか確認します。
返り値が帰ってくれば、正常にインストールできています。

//cmd{
# Elasticsearchの稼動確認
$ curl http://localhost:9200/
{
  "name" : "asgUjsK",
  "cluster_name" : "Elasticsearch",
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
//}

もしくはGoogle Chrome上でURLにアクセスし、同じような返り値があればOKです。

//image[localhost_9200][Elasticsearchの起動確認]{
/Users/mallow/review/text/text/images/localhost_9200.png
//}

== Logstashのインストール

LogstashもElasticsearchと同様、インストール方式を選択することが可能です。
ただ、ツールごとにインストール方式を分けるやり方は、ディレクトリ構造が異なってしまうため避けた方が良いです。

==={logstash-zip} とにかく使ってみたい場合（Linux）：zipファイル

こちらもElasticsearchと同様、zipファイルを展開するだけでインストールが終了します。
serviceコマンドは付属しません。

==={logstash-rpm}  ちゃんと運用もしたい場合（rpmパッケージを利用するLinux）：rpmパッケージ
こちらもserviceコマンドの存在や、ディレクトリ構成が自動で割り当てられる点なども
Elasticsearchと同様です。

例）
* コンフィグファイルの配置先：/etc/logstash/
* ログファイルの出力先：/var/log/logstash
* 基本的なファイルの配置先：/opt/logstash/

==={logstash-deb} ちゃんと運用もしたい場合（debパッケージを利用するLinux）：debパッケージ
こちらもElasticsearchと同様です。

==={logstash-docker} とにかく使ってみたい場合かつ、Docker実行環境がある場合：Dockerコンテナ
Elasticsearchと同様、Elastic社からDockerコンテナが提供されています。
ただし、Elasticsearchとは別のコンテナのため、同時にコンテナを複数起動する必要があります。

LogstashはRubyで作成されていますが、起動にJavaを必要とします。
こちらもヒープメモリもかなり消費するツールなので注意が必要です。

==={logstash-com} Puppet、Chef、Ansibleの実行環境がある場合：Githubのレシピを利用
Elastic公式から各ツールのレシピが提供されているので、Githubからクローンして利用することも可能です。

==={logstash-win} Windows/Mac上に構築する場合：zipファイル
Windowsの場合、インストール方法はzipファイル一択となります。Macはbrewコマンドを用いることで
インストールすることもできますが、Elastic公式ではサポートされていないようなので、
今回はインストール方法から除外しています。

== zipファイルを用いたインストール(Logstash)

Elasticsearchのインストール方法に合わせるため、6のzipファイルを用いたインストール方法を取ります。
こちらも、OSごとに手順に差はありません。

==={logstash-dl} 公式サイトからzipファイルをダウンロード

Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。

@<href>{https://www.elastic.co/jp/downloads/logstash}

//image[logstash_download][Logstashのダウンロード]{
  /Users/mallow/review/text/text/images/logstash_download.png
//}

==={logstash-unzip} zipファイルの解凍

Elasticsearchをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。
ディレクトリ構成は一例です。

//cmd{
unzip logstash-5.2.2
//}

==={logstash-conf} 動作確認用のlogstash.conf作成

この後の章で詳しく述べますが、Logstashはlogstash.confを読み込むことで
ファイルの取り込み元などを指定します。まずは動作確認用に次のコードをconfファイルへ記述します。

//cmd{
# logstash.confの作成
logstash-5.2.2 mofumofu$ touch logstash.conf
//}

次に、logstash.confを編集します。
//cmd{
# logstash.confの編集
logstash-5.2.2 mofumofu$ vi logstash.conf

# 下記の通り編集
-----------------------------------------------------
# 標準入力を受け付ける
input {
  stdin { }
}
# 標準出力を行う
output {
  stdout { codec => rubydebug }
}
//}

==={logstash-test} 動作確認

@<tt>{/bin}下にある@<tt>{logstash}スクリプトから、Logstashを起動します。
OSがWindowsの場合、同階層に@<tt>{logstash.bat}が配置されているのでそちらを起動しましょう。
@<tt>{logstash.conf}にタイプミスがない場合、<@<tt>{"Logstash startup completed"}と出力されます。
コマンドプロンプトに好きな文字列を打ち込んでみましょう。
文字列がそのまま返り値として出力された場合、正しくセットアップできています。

//cmd{
logstash-5.2.2 mofumofu$ /bin/logstash -f logstash.conf
-bash: /bin/logstash: No such file or directory
logstash-5.2.2 mofumofu$ bin/logstash -f logstash.conf
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
          "host" => "mofumofu-no-MacBook-Pro.local",
       "message" => "hello world"
}
//}

== Kibanaのインストール

Kibanaも他ツールと同様、インストール方法が複数準備されています。
種類はElasticsearch・Logstashと変わらないため、そちらの章を参照してください。

==={kibana-install} zipファイルを用いたインストール

ElasticsearchとLogstashのインストール方法に合わせるため、zipファイルを用いたインストール方法を選択しました。
こちらも、OSごとに手順に差はありません。

==={kibana-zip} 公式サイトからzipファイルをダウンロード

Elasticの公式サイトにアクセスし、zipファイルをダウンロードします。
OSの種類によってzipファイルが異なるため、注意が必要です。

@<href>{https://www.elastic.co/jp/downloads/kibana}

//image[kibana_download][Kibanaのダウンロード]{
/Usersmofumofu/review/text/text/images/kibana_download.png
//}

==={kibana-unzip} zipファイルの解凍

Elasticsearchをインストールする際に作成したディレクトリに、ダウンロードしたzipファイルを解凍します。

//cmd{
unzip kibana-5.2.2-darwin-x86_64.tar.gz
//}

==={kibana-yml} kibana.ymlの編集

前にも述べた通り、KibanaはElasticsearchからデータを取得するためElasticsearchのURLを指定する必要があります。
kibana.yml内にURLを指定する箇所があるのでそちらを記述しましょう。下に記載している場所を編集すれば良いです。

//cmd{
# 前後は省略
--------------------------------------------------------------------
# The URL of the Elasticsearch instance to use for all your queries.
#Elasticsearch.url: "http://localhost:9200"
Elasticsearch.url: "http://localhost:9200"
--------------------------------------------------------------------
//}

==={kibana-test} 動作確認

/binフォルダ下にあるkibanaスクリプトから起動します。（Windowsはkibana.batから起動します。）

//cmd{
kibana-5.2.2-darwin-x86_64 mofumofu$ bin/kibana
  log   [07:46:54.934] [info][status][plugin:kibana@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.016] [info][status][plugin:Elasticsearch@5.2.2] Status changed from uninitialized to yellow - Waiting for Elasticsearch
  log   [07:46:55.046] [info][status][plugin:console@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.900] [info][status][plugin:timelion@5.2.2] Status changed from uninitialized to green - Ready
  log   [07:46:55.906] [info][listening] Server running at http://localhost:5601
  log   [07:46:55.907] [info][status][ui settings] Status changed from uninitialized to yellow - Elasticsearch plugin is yellow
  log   [07:47:00.943] [info][status][plugin:Elasticsearch@5.2.2] Status changed from yellow to yellow - No existing Kibana index found
  log   [07:47:01.661] [info][status][plugin:Elasticsearch@5.2.2] Status changed from yellow to green - Kibana index ready
  log   [07:47:01.662] [info][status][ui settings] Status changed from yellow to green - Ready
//}

起動後、ブラウザに@<href>{http://localhost:5601}と入力します。図X.Xのような画面が見えていればインストールは完了です。

//image[kibana_setup][Kibanaの画面]{
/Usersmofumofu/review/text/text/images/kibana_setup.png
//}
