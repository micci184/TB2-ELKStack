= データを集めて可視化しよう（Twitterのログ編）

== Twilogサービスを利用したログ取得

　まずはLogstashに取り込むデータを準備します。今回はTwitterアカウントの発言証跡を
追いかけるため、@<b>{Twilog}サービスを利用します。
@<b>{Twilog}とは、Twitterでのツイートを自動で保存・閲覧できるサービスです。
記録された履歴はCSV（SJIS/UTF-8）やXMLでダウンロードすることができます。
今回はCSV（UTF-8）でダウンロードします。

=== Twilogサービスへの登録
　まず、@<href>{http://twilog.org}へアクセスします。
@<b>{Sign in with Twitter}アイコンをクリックし、Twitterの認証画面で自分のアカウントに
Twilogを登録してください。

//image[Twilog_register][Twilogへの登録]{

//}

　認証後、Twilogの管理画面へアクセスします。
Twilogは過去ツイートを200件分のログを取得するため、@<b>{過去ツイートの取得}から
昔のツイートを全て取得しておきましょう。

//image[Twilog_getlog][過去ツイートの取得]{

//}

=== 過去ログのダウンロード
　いよいよ過去ログのダウンロードを行います。とは言っても@<img>{Twilog_downloadCsv}の画面から
好きな形式を選択してダウンロードするだけです。zip圧縮されているため、適宜解凍してください。ログは
好きなディレクトリに配置して良いですが、読み取り権限がついているか確認してください。

//image[Twilog_downloadCsv][ログのダウンロード]{

//}

　ログを解凍して中身をみると、このように表示されました。これを延々と読むのはなかなか辛いものがあります。
左から、@<tt>{Twitter ID}、@<tt>{つぶやいた日時}、@<tt>{つぶやいた時刻}、@<tt>{本文}となっています。
//emlist[ログの中身]{
# 抜粋
"839273776948731904","170308 093716","でもその前に、トラブルシュートのダルさだけはなんとかしてほしいもんだな"
"839273607192690688","170308 093635","KibanaのURLを共有しとけば簡単にレポートとか作れちゃうんだもんなあ"
"839273190639591425","170308 093456","ElasticonのKeynote見たけど、Kibana Canvasは良さそうだと思いました"
//}

　それではこのCSVファイルをLogstashに取り込んでみましょう。

== logstash.confの全体像
　Logstashのデータ取得方法は@<tt>{logstash.conf}で設定します。
@<tt>{logstash.conf}は@<b>{input}、@<b>{filter}、@<b>{output}の3つに
分かれています。

=== input
　inputは、ログをどこから取得するか決める部分です。
ログの取得間隔や、Logstashサービス再開時の挙動を指定することも可能です。

//image[logstash_config_input][logstash.conf（input）]{
/Users/mallow/review/text/text/images/logstash_config_input.png
//}


=== filter
　filterは、ログをどのように加工・整形するか決める部分です。
「加工」とはいっても、取り込んだログの文言を書き換える、条件に一致するログを消去する等
方法は様々です。

//image[logstash_config_filter][logstash.conf（filter）]{
/Users/mallow/review/text/text/images/logstash_config_filter.png
//}

=== output
　outputは、ログをどこに送るか指定する部分です。
前の章でも述べた通り、ElasticSearhへのデータ送付以外にもCSV形式など
指定した拡張子でデータを出力することも可能です。

//image[logstash_config_output][logstash.conf（output）]{
/Users/mallow/review/text/text/images/logstash_config_output.png
//}


== inputプラグイン

　input部分では情報の取得元ごとにプラグインが提供されています。
今回はファイルを取得するため、@<b>{file}プラグインを使います。
利用できるプラグインの種類は@<href>{https://www.elastic.co/guide/en/logstash/current/input-plugins.html}で
確認することができます。

　@<tt>{logstash.conf}の記載はJSON形式で行います。
inputプラグインであれば、次のように記載します。

//emlist[inputプラグインの記載方法]{
input{
  利用するプラグイン名{
    設定を記載
  }
}
//}

=== pathの記載
　各プラグインには必須項目があります。@<tt>{file}プラグインでは@<b>{path}が
必須項目にあたります。@<b>{path => ファイルパス}のように指定します。
パスの指定には正規表現を利用できますが、フルパスで記載する必要があります。

//emlist[pathの指定]{
input{
  file{
    path => "/フォルダのフルパス/logs/**.csv"
  }
}
//}

=== 取り込んだログを標準出力する
　outputに@<b>{stdout}を指定すると、コンソール上に標準出力することができます。
まずはファイルが取り込みできるかテストしてみましょう。

//emlist[CSVをLogstashに取り込み標準出力するlogstash.conf]{
input{
  file{
    path => "/フォルダのフルパス/logs/**.csv"
  }
}
output{
  stdout{ codec => rubydebug }
}
//}

　Logstashの起動は先ほどと同じように@<tt>{/bin}下にある起動スクリプトから行います。
起動すると、このようにコンソール出力されます。
プロセスの終了は@<tt>{Ctl + C}で行います。

//cmd{
$ logstash-5.2.2/bin/logstash -f logstash-5.2.2/logstash.conf
Sending Logstash's logs to /Users/mallow/ELK_Stack/logstash-5.2.2/logs which is now configured via log4j2.properties
[2017-03-11T20:31:56,188][INFO ][logstash.pipeline        ] Starting pipeline {"id"=>"main", "pipeline.workers"=>4, "pipeline.batch.size"=>125, "pipeline.batch.delay"=>5, "pipeline.max_inflight"=>500}
[2017-03-11T20:31:56,421][INFO ][logstash.pipeline        ] Pipeline main started
[2017-03-11T20:31:56,513][INFO ][logstash.agent           ] Successfully started Logstash API endpoint {:port=>9600}
//}

=== ファイルの読み込み位置を指定する
　実際にやってみるとわかりますが、このままではLogstashの標準出力には何も表示されません。
デフォルトの設定では、@<b>{起動した後に更新された分だけファイルを読み取る}設定になっているためです。

　どこまでファイルを読み取ったかは、Logstashが自動で@<b>{sincedb}というファイルに記録しています。
設定を変更することで@<tt>{sincedb}ファイルに記録されている履歴の次から情報を取得することができます。
ファイルの読み込み位置は@<b>{start_position}を用いて設定します。

//emlist[start_positionの指定]{
# Logsatsh起動後に追加されたログのみ取り込む場合
start_position => "end"
# Logsatshが停止後に追加されたログも取り込む場合
start_position => "end"
//}

　@<tt>{logstash.conf}上で指定する場合、次のように記載します。

//emlist[start_positionを指定したlogsatsh.conf]{
input{
  file{
    path => "/フォルダのフルパス/logs/**.csv"
    start_position => "beginning"
  }
}
//}

=== ファイルにタグをつけて分類する
　読み込んだデータを分類したい場合、自分でタグ（@<b>{tags}）をつけることができます。
@<tt>{tags}を利用すると、if文などを用いて取り込んだデータに対する固定の処理を指定することができます。
また、ログ情報に@<tt>{tags}が付与されるため、データの種類ごとにKibanaのグラフを作成することも可能です。
@<code>{tags => "好きな名前"}で指定します。

=== 指定したファイルの種類は除外する
　zipファイルなど、取り込み対象から除外したいものがある場合、@<b>{exclude}を利用することで
指定したものをLogstashの取り込み対象から外すことができます。こちらはフルパス指定は不要です。
正規表現を用いて指定することができます。

//emlist[excludeの指定]{
# zipファイルを取り込みから除外したい場合
exclude => "*.zip"
//}

　今までの設定を全て追加した場合、@<tt>{logstash.conf}はこのようになります。

//emlist[inputプラグインを追加したlogstash.conf]{
input{
  file{
    exclude => "*.zip"
    path => "/Users/mallow/ELK_Stack/logs/**.csv"
    start_position => "beginning"
    tags => "CSV"
  }
}

output{
  stdout{ codec => rubydebug }
}
//}

　この状態でLogstashを起動すると、CSVのデータが標準出力されます。
//cmd{
  $ logstash-5.2.2/bin/logstash -f logstash-5.2.2/logstash.conf
  Sending Logstash's logs to /Users/mallow/ELK_Stack/logstash-5.2.2/logs which is now configured via log4j2.properties
  [2017-03-11T20:31:56,188][INFO ][logstash.pipeline        ] Starting pipeline {"id"=>"main", "pipeline.workers"=>4, "pipeline.batch.size"=>125, "pipeline.batch.delay"=>5, "pipeline.max_inflight"=>500}
  [2017-03-11T20:31:56,421][INFO ][logstash.pipeline        ] Pipeline main started
  [2017-03-11T20:31:56,513][INFO ][logstash.agent           ] Successfully started Logstash API endpoint {:port=>9600}
  ^C[2017-03-11T20:46:29,527][WARN ][logstash.runner          ] SIGINT received. Shutting down the agent.
  [2017-03-11T20:46:29,537][WARN ][logstash.agent           ] stopping pipeline {:id=>"main"}
  $ logstash-5.2.2/bin/logstash -f logstash-5.2.2/logstash.conf
  Sending Logstash's logs to /Users/mallow/ELK_Stack/logstash-5.2.2/logs which is now configured via log4j2.properties
  [2017-03-11T22:07:14,824][INFO ][logstash.pipeline        ] Starting pipeline {"id"=>"main", "pipeline.workers"=>4, "pipeline.batch.size"=>125, "pipeline.batch.delay"=>5, "pipeline.max_inflight"=>500}
  [2017-03-11T22:07:15,039][INFO ][logstash.pipeline        ] Pipeline main started
  [2017-03-11T22:07:15,194][INFO ][logstash.agent           ] Successfully started Logstash API endpoint {:port=>9600}
  {
            "path" => "/Users/mallow/ELK_Stack/logs/froakie0021170311.csv",
      "@timestamp" => 2017-03-11T13:07:15.099Z,
        "@version" => "1",
            "host" => "XX.local",
         "message" => "",
            "tags" => [
          [0] "CSV"
      ]
  }
  {
            "path" => "/Users/mallow/ELK_Stack/logs/froakie0021170311.csv",
      "@timestamp" => 2017-03-11T13:07:15.113Z,
        "@version" => "1",
            "host" => "XX.local",
         "message" => "\"730751058306162689\",\"160512 222643\",\"いちごメロンパン食べたい #precure\"",
            "tags" => [
          [0] "CSV"
      ]
  }

//}

== outputプラグイン
　Kibana上でグラフを作成するためにはElasticSearhにデータが入っている必要があります。
このままでは標準出力に取り込んだデータが出てくるだけなので、outputプラグインを用いて
データの送り先を指定します。

=== ElasticSearhにデータを送る
　ElasticSearhにログを送付するには@<tt>{elasticsearch}プラグインを利用します。
必須項目はありませんが、ElasticSearhのホスト名を指定しないと@<href>{127.0.0.1}にアクセスします。

　hostsを明示s的に設定するためには@<b>{hosts}を利用します。
@<code>{hosts => "ElasticSearhのアクセス用URL"}で指定します。

//emlist[elasticsearchプラグインの指定]{
output {
    elasticsearch{
    hosts => "http://localhost:9200/"
  }
}
//}

=== 送付先indexの指定
　ElasticSearhはデータの持ち方が@<b>{field}に対する@<b>{text}という構造になっています。
fieldとは、データベースでのカラムに相当します。textはカラムの中に入っている実データです。
カラムの中にデータを入れるのと同じように、field内に実際のデータ（@<tt>{text})を保持します。
このfieldの集まりを@<tt>{index}といいます。

　デフォルトのindex名は@<tt>{"logstash-%{+YYYY.MM.dd\}"}です。
@<tt>{elasticsearch}プラグインの@<tt>{index}を用いることでindex名を変更することができます。

//emlist[index名の変更]{
index => "%{index名}-%{+YYYY.MM.dd}"
//}

　今回はindex名は変更せず、デフォルトの設定のまま利用することにします。
ただElasticSearhに大量のデータを集める場合、indexを明示的に指定することで
Kibanaを用いてデータを取り出す速度を上げることができます。

　これで、outputプラグインも書くことができました。
他のプラグインを利用すると、データをファイル出力することも可能です。
利用できるプラグインは公式サイト@<href>{https://www.elastic.co/guide/en/logstash/current/output-plugins.html}に
一覧があります。

//emlist[outputを記載したlogstash.conf]{
input{
  file{
    exclude => "*.zip"
    path => "/Users/mallow/ELK_Stack/logs/**.csv"
    start_position => "beginning"
    tags => "CSV"
  }
}

output{
  stdout{
    codec => rubydebug
  }
  elasticsearch{
    hosts => "http://localhost:9200/"
  }
}
//}

== filterプラグイン
　今の状態だと、ログはこのように連携されます。@<tt>{=>}より左はfield部分、右は実際のデータです。
//emlist[標準出力で出ているログ]{
{
          "path" => "/Users/mallow/ELK_Stack/logs/froakie0021170311.csv",
    "@timestamp" => 2017-03-11T13:07:15.113Z,
      "@version" => "1",
          "host" => "XX.local",
       "message" => "\"730751058306162689\",\"160512 222643\",\"いちごメロンパン食べたい #precure\"",
          "tags" => [[0] "CSV"]
}
//}

　このままKibanaで可視化するのは抵抗があります。というのも、field名@@<tt>{host}では
自分のPCの名前が丸見えになっていますし、@@<tt>{path}でファイルのフルパスが記載してあるのを
そのまま公開するのも気が引けます。

　さらに、@<tt>{@timestamp}を見る限り、時間が明らかに実際のログの時間とずれています。
もっというと@<tt>{message}の中のログは1つの情報ごとに分割したいですよね。
@<b>{filter}を使ってログの中身を整形しましょう。
プラグインの一覧は@<href>{https://www.elastic.co/guide/en/logstash/current/filter-plugins.html}に
情報がまとまっています。

=== CSV形式のログを分割する
　CSV形式のログを分割するためには@<b>{csv}プラグインを利用します。
必須項目はありません。指定は@<code>{　csv {\} }というように行います。

//emlist[filterにcsvを指定したlogstash.conf]{
input{
  file{
    exclude => "*.zip"
    path => "/Users/mallow/ELK_Stack/logs/**.csv"
    start_position => "beginning"
    tags => "CSV"
  }
}

filter{
  csv{

  }
}

output{
  stdout{
    codec => rubydebug
  }
  elasticsearch{
    hosts => "http://localhost:9200/"
  }
}
//}

　実際にログを出力すると、column1などと、カラムごとにfieldも分割されたことがわかります。
//emlist[csvフィルタを指定してLogstashを起動した場合]{
{
       "column1" => "700941673379987456",
          "path" => "/Users/mallow/ELK_Stack/logs/froakie0021170311.csv",
    "@timestamp" => 2017-03-11T14:20:08.119Z,
       "column3" => "test",
       "column2" => "160220 161452",
      "@version" => "1",
          "host" => "ishiiaoi-no-MacBook-Pro.local",
       "message" => "\"700941673379987456\",\"160220 161452\",\"test\"",
          "tags" => [
        [0] "CSV"
    ]
}
//}

=== カラムのデータ型を変更する
　@@<b>{convert}を使うと、@@<tt>{column1}、@@<tt>{column2}（以下略）内のデータ型を指定することができます。
@<code>{convert => { "column1" => "変更したい型" \} }のように記載します。変更できる型は
@<table>{convertList}を参照してください。変更しない場合、全て文字型として扱われます。

//table[convertList][convertオプションで変更できる型]{
指定できるもの  データ型
----------
integer  数値
float 浮動小数点数
date  日付
date_time 日付と時刻
boolean boolean
//}

　今回はcolumn2の中に日付と時刻が記載されているため、column2のみ型を変更します。
//emlist[convertを記載したlogstash.conf]{
filter{
  csv{
    convert => {
      "column2" => "date_time"
    }
  }
}
//}

=== 不要なfieldを消す
　時には見せたくないカラムの情報もあるかと思います。今回は@@<tt>{column1}、@@<tt>{path}
@@<tt>{host}の情報は丸ごと不要なので、消してしまいたいと思います。

//emlist[不要なfield（今回の場合）]{
{
       "column1" => "700941673379987456",
          "path" => "/Users/mallow/ELK_Stack/logs/froakie0021170311.csv",
          "host" => "ishiiaoi-no-MacBook-Pro.local",
}
//}

　fieldごと情報を削除したい場合、@@<b>{remove_field}オプションを利用します。
@@<code>{remove_field => ["削除したいfield名"]}のように指定します。
複数指定する場合、コロンで繋げます。

//emlist[remove_fieldオプションの指定例]{
filter{
  csv{
    remove_field => ["column1", "host", "path"]
  }
}
//}

　実行すると、指定したfieldは丸ごと削除されます。
//emlist[remove_field指定後の出力結果]{
{
    "@timestamp" => 2017-03-12T01:16:04.983Z,
       "column3" => "いちごメロンパン食べたい #precure",
       "column2" => "160512 222643",
      "@version" => "1",
       "message" => "\"730751058306162689\",\"160512 222643\",\"いちごメロンパン食べたい #precure\"",
          "tags" => [
        [0] "CSV"
    ]
}
//}

=== ログが出力された時刻を編集する
　@@<tt>{@timestamp}の時刻が実際のログとずれる理由ですが、Logstashの仕様に原因があります。
Logstashは@@<b>{データを取り込んだ時間}を@@<tt>{@timestamp}として付与します。
これにより、データの持っている時刻と@@<tt>{@timestamp}の時間がずれているように見えたのです。

　また、Logstashは世界標準時間（UTF）を利用するので、日本時間からマイナス9時間されてしまいます。
これらを解消するためには、@@<tt>{@timestamp}の情報をデータ内の情報で置き換える必要があります@@<fn>{dateAttention}。

//footnote[dateAttention][必須の処理ではありませんが、いつデータが出力されたのか知りたい場合、この方法を取ることをお勧めします]

　このように、時刻を変更したい場合@@<b>{date}フィルタを指定します。
@@<tt>{date}フィルタの@@<b>{match}オプションを使ってどのfieldを時刻として利用するか決定しましょう。
@<code>{match => ["field名", "実データの日付記載方式", "変換方法"]}と指定します。

　field名@@<tt>{column2}内のデータ@<tt>{16年05月12日 22時26分43秒"}を
@@<tt>{ISO8601}形式で置き換える場合、このような指定方法となります。

//emlist[matchオプションの指定例]{
date{
  match => [ "column2", "YYMMdd HHmmss", "ISO8601" ]
}
//}

　実際に出力してみると、@@<tt>{@timestamp}の項目がログの時刻と一致していることがわかります。
//emlist[matchオプション指定後の出力結果（抜粋）]{
{
    "@timestamp" => 2016-05-12T13:26:43.000Z,
       "column2" => "160512 222643",
}
//}

　時刻の指定方法は@<href>{https://www.elastic.co/guide/en/logstash/current/plugins-filters-date.html}に
記載があります。

== logstash.confのテスト方法
　@@<tt>{logstash.conf}を作成するときは、このようにログの取り込み・出力を繰り返しながら
必要なぶんだけデータ連携されるように調整を繰り返すことになります。
@@<tt>{logstash.conf}の編集を繰り返していくと文法ミスが見つけにくくなるため、
適宜コンフィグのテストを行いましょう。

　コンフィグのテストはLogstashの起動時に@@<b>{-t}オプションを指定するだけです。
コンフィグが間違っている場合、行数をエラー出力してくれます。この場合はdateフィルタの
書き方が間違っていました。

//emlist[記載していたコンフィグ（抜粋）]{
filter{
  csv{
    convert => {
      "column2" => "date_time"
    }
    remove_field => [ "column1", "host", "message", "path", "@version" ]
  }
  date{
    match => [ "column2" => "YYMMdd HHmmss", "ISO8601" ]
    remove_field => [ "column2" ]
  }
}
//}

//emlist[コンフィグテストの例]{
ishiiaoi-no-MacBook-Pro:~ mallow$ ELK_Stack/logstash-5.2.2/bin/logstash -f ELK_Stack/logstash-5.2.2/logstash.conf  -t
Sending Logstash's logs to /Users/mallow/ELK_Stack/logstash-5.2.2/logs which is now configured via log4j2.properties
[2017-03-12T10:50:00,519][FATAL][logstash.runner          ] The given configuration is invalid. Reason: Expected one of #, {, ,, ] at line 18, column 25 (byte 323) after filter{
  csv{
    convert => {
      "column2" => "date_time"
    }
    remove_field => ["column1", "host", "message", "path", "version"]
  }
  date{
    match => ["column2"

//}

== 今回作成したlogstash.conf
　章の最後に、今回作成した@@<tt>{logstash.conf}を記載します。
@@<tt>{column2}はdateフィルタで@@<tt>{@timestamp}に変換後は不要なカラムと
なるため、削除しました。そのほかにも余計な情報は極力落とすようにしています。
また、コメントアウトは@<code>{# （半角スペース）}で行います。

//emlist[作成したlogstash.conf]{
input{
  file{
    exclude => "*.zip"
    path => "/Users/mallow/ELK_Stack/logs/**.csv"
    start_position => "beginning"
    tags => "CSV"
  }
}

filter{
  csv{
    convert => {
      "column2" => "date_time"
    }
    remove_field => [ "column1", "host", "message", "path", "@version" ]
  }
  date{
    match => [ "column2", "YYMMdd HHmmss", "ISO8601" ]
    remove_field => [ "column2" ]
  }
}

output{
# stdout{
#   codec => rubydebug
# }
 elasticsearch{
   hosts => "http://localhost:9200/"
 }
}
//}

　実際のデータとLogstashへの取り込み結果を比較してみます。出力結果は標準出力の結果ですが
ElasticSearhにも同じように連携されます。

//emlist[取り込んだデータ]{
"730751058306162689","160512 222643","いちごメロンパン食べたい #precure"
//}

//emlist[出力結果]{
{
    "@timestamp" => 2016-05-12T13:26:43.000Z,
       "column3" => "いちごメロンパン食べたい #precure",
          "tags" => [
        [0] "CSV"
    ]
}
//}

　次は、Logstashで取り込んだデータをグラフで可視化しましょう。
ElasticSearhを起動後、Logstashを連携してログを連携しておきます。
