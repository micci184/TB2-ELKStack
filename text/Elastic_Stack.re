= Elastic Stackって何？

//lead{
「Elastic StackはElastic社が提供しているBIツール群っていうのはわかったけど、どんなツールがあるのかな？
公式サイトを見るといっぱい種類があるみたいだけど…。」
//}

　おや？もふもふちゃん、なんだかお困りのようです。それもそうですね。ELastic Stackには
なんだかたくさん便利ツールがあるのはわかりますが、ログ解析にはどのツールが必要なのかわかりません。
この章でログ分析に最低限必要なツールが何か、一緒にみてみましょう。

== Logstash
　Logstashは、各環境に散らばっているログを集め、指定した対象に連携できるツールです。ログの連携だけではなく、
ログの加工機能も持ち合わせています。コード自体はRuby言語で記載されています。

　肝心のどんなログが取り込みできるかですが、の出力形式としてよくあるテキストファイルはもちろん、xmlやjsonファイルも対象として指定できます。
ファイルの情報以外にも、Twitter APIと連携してTwitterのつぶやき情報を取り込む事や、
データベース（RSDB）に接続して情報を抜いてくる事も可能です。RSDBと連携する際はSQL文を用いて情報を取得するため
SQLが得意な人は自分の欲しい情報だけSQLで取得し、あとはログを連携するだけといった事も実現できます。

　ログの出力先ですが、このあと出てくるElasticsearchだけでなく、プロジェクトの進捗状況管理ツールである
Redmineに送付する事もできます。取り込みした情報をCSVファイルとして出力したりsyslogとして転送することができるので
利用方法によってはログ解析以上の威力を発揮するツールだと言えます。

== Elasticsearch
　Elasticsearchは、Javaで作られている分散処理型の検索エンジンです。クラスタ構成を組むことができるのが特徴なので、
大規模な環境で検索エンジンとして利用されることが多いです。某新聞のWebサイト内の検索、
Dockerのコンテナ検索、Facebook上での検索などが導入事例として有名です。

　クラスタとは、物理的には複数存在しているにも関わらず、論理的には1つとして見せることができる技術です。
処理の負荷分散ができるため、性能を求められる環境で選択されることが多いです。

//image[cluster01][クラスターを利用しない場合]{
配置パス
/Users/mallow/review/text/text/images/cluster01.png
//}

//image[cluster02][クラスターを利用する場合]{
配置パス
/Users/mallow/review/text/text/images/cluster02.png
//}

　もちろん、ログ解析を行うときもよく使用されます。理由としては次に出てくるKibanaの情報取得元が
Elasticsearchに限られているからです。

== Kibana
　Kibanaは、Elasticsearchに貯められている情報を整形して可視化する情報分析ツールです。
開発言語はアナウンスは出ていないものの、ソース情報を見る限りJavaScriptがメインだと思われます。
Google Chrome等のブラウザからKibana指定のURLにアクセスすることで、データ情報を表示することができます。

　Kibanaでは知りたい情報の件数だけでなく、折れ線グラフ・棒グラフ・円グラフを用いてログの詳細な情報を解析し
色分けして表示することが可能です。グラフの大きさを決める際にコンフィグなどを編集する必要はなく、ブラウザ上での操作で
全て完結する仕組みとなっています。

== Beats
　Beatsはサーバにインストールすることで、サーバ内のマシンデータをElasticsearchやLogstashに転送する
簡易的なデータ収集ツールです。サーバのリソース情報以外にもネットワークのパケット情報・Windowsのイベントログを収集することが
できるため、Logstashでカバーしきれない範囲の情報を集めてくることができます。
Logstashにログを転送することで、他のログと同じように加工・転送が可能となるため、痒いところに手がとどくツールという位置づけです。

　BeatsはGo言語で作られており、開発初期から大幅に機能追加がされているツールです。今後どのように拡張されていくのか、楽しみですね。

== 基本的な構成

　今回はBeatsは扱わず、ログを収集するLogstash、ログを貯めておくElasticsearch、ログを閲覧するKibanaを基本構成とします。
この3つのツールで構成されている状態は「ELK」と省略されて呼ばれることが多いです。この本でもこれ以降はLogstash、Elasticsearch、Kibanaの
3つをまとめて扱う際は「ELK」と省略して呼ぶこととします。
