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

=== pathの記載（input：path）
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

=== 取り込んだログを標準出力する（output：stdout）
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

=== ファイルの読み込み位置を指定する（input：start_position）
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

=== ファイルにタグをつけて分類する（input：tags）
　読み込んだデータを分類したい場合、自分でタグ（@<b>{tags}）をつけることができます。
@<tt>{tags}を利用すると、if文などを用いて取り込んだデータに対する固定の処理を指定することができます。
また、ログ情報に@<tt>{tags}が付与されるため、データの種類ごとにKibanaのグラフを作成することも可能です。
@<code>{tags => "好きな名前"}で指定します。

=== 指定したファイルの種類は除外する（input：exclude）
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
         "message" => "\"730751058306162689\",\"160512 222643\",\"クールビューティーだけどさ #nhk_life\"",
            "tags" => [
          [0] "CSV"
      ]
  }

//}


== ログの送付先を指定する
　今回はElasticSearhにログを送付するため、ElasticSearhプラグインを利用します。
動作確認のため標準出力を可能にするstdoutプラグインも利用しましょう。
output内に複数のプラグインを同時記述することができます。括弧の閉じ忘れには気をつけてください。

//cmd{
output {
    elasticsearch{
    hosts => "http://localhost:9200/"
  }
}
//}

=== 動作確認

//lead{
　「filterプラグインなんて書く必要はあるのかなあ？ログの取り込みができればいいと思うけど…。」
//}

　もふもふちゃん、余裕をかましているようです。本当に大丈夫なのでしょうか。
大丈夫かどうか、動作確認してみましょう。
ElasticSearh→Logstash→Kibanaの順に起動します。

　サービスが開始されたら、Kibanaにアクセスします。

//lead{
　「さっきと何も変わんないよ！」
//}

　おっと、Kibanaでログを閲覧するためには、ElasticSearhとKibanaの情報を紐付ける必要があります。
詳細はKibanaの章で解説しますので、今はとにかくKibanaで情報が見えるようにしましょう。
まずは画面の1番下にある@<tt>{"Create index"}を押してください。

#@#//image[Kibana_create_index01][Indexの紐付け]{
#@#/Usersmallow/review/text/text/images/Kibana_create_index01.png
#@#//}

　これで情報の紐付けができました。紐付けができるとKibanaはこのような画面になっているはずです。

#@#//image[Kibana_create_index02][Indexの紐付け後]{
#@#/Usersmallow/review/text/text/images/Kibana_create_index02.png
#@#//}

//lead{
　「あら？なんかログがただ並んでいるだけのように見えるなあ。なんでー？？」
//}

　…やっぱり大丈夫じゃなかったみたいですね。filterプラグインはログを加工するだけでなく
データを綺麗に分割する役割も持ち合わせています。今はただ情報を取得して送付しているだけなので
取り込んだものがそのままデータ連携されてしまったのですね。

//lead{
　「やっぱりサボっちゃダメなのかー。」
//}

　そうですね。でもfilter部分の頑張り度とKibana画面の使いやすさ度は比例しますので
ここはもう一踏ん張りしてみましょう。

=== filterプラグインを書いてみよう
　今から編集する部分は、@<tt>{logstash.conf}のメインとなる部分です。

== fieldとtextにデータをうまく分ける（ElasticSearhのデータの持ち方を説明）
　先ほどKibanaを閲覧したとき、データがうまく分かれていなかったと思います。
これはElasticSearhのデータの持ち方が@<tt>{field}に対する@<tt>{text}という構造になっているためです。
fieldとは、データベースでのカラムに相当します。textはカラムの中に入っている実データです。
カラムの中にデータを入れるのと同じように、field内にtextを保持します。
このfieldの集まりを@<tt>{index}といいます。

　つまり、ElasticSearh内のfieldごとにデータが入るようにログを取り込まないと
Kibanaでは1つのログの塊として認識されてしまい、データの区分けができないのです。
これではせっかくのKibanaの便利さも半減されてしまいます。
では、どのようにログを分割すれば良いのでしょう？

== kvフィルタ：ログを分割する
　kvフィルタを使うことで、ログを分割することができます。
どのように分割されるのか、具体例を用いながらみていきましょう。
kvフィルタを利用する前は、このようにログが出力されます。

#@# kvフィルタ前のログ

　では、kvフィルタを追加してみましょう。

//cmd{
input{
  twitter{
    consumer_key => "Twitter APIのconsumer_key"
    consumer_secret => "Twitter APIのconsumer_secret"
    oauth_token => "Twitter APIのAccess Token"
    oauth_token_secret => "Twitter APIのAccess Token Secret"
    keywords => ["検索したいもの"]
    full_tweet => "true"
  }
}

filter{
  kv{
  }
}

output {
    elasticsearch{
    hosts => "http://localhost:9200/"
  }
}
//}

　この状態でLogstashサービスを起動すると、このように出力されます。

#@# kvフィルタ後のログ

　このように、kvフィルタを追加するとログの区切りでログの種類：実際の情報といった形で
ログが分割できるのです。例えば「aaa：bbb」というログがあった場合、
「aaa」の部分はfield項目扱いに、「bbb」の部分はtext項目扱いとなります。
実際のログも区切り前がログの種類、区切り跡が実際の情報になりますからね。

== removeフィルタ：いらない情報は捨てる

//lead{
　「うーん、ログにはTwitterアカウント名が含まれているけれど、ちょっとこれは個人情報っぽいから
あまりログ情報としては持っておきたくないなあ」
//}

　確かに、いらない情報や取得してはいけない情報は捨ててしまいたいですよね。
そんなときはremoveフィルタを使用することで、指定した条件に一致するログを「なかったこと」にできます。

#@# コードを入れる

　条件指定はif文を用いて行います。if文のあとにfield名を指定し、
イコールの後に削除したいログの具体的な名称を記載します。
条件指定には正規表現を用いることができるので「○○に当てはまるものは全て」といった
指定も可能です。前に記載した@<tt>{logstash.conf}を用いて、dropフィルタを使用する前と後を比較してみましょう。

#@# dropフィルタを利用する前

#@# dropフィルタを利用した後

本当になくなってしまいましたね…。

== timestampフィルタ：ログが出力された時刻を編集する

#@# 忘れたので調べる

=== 最後に動作確認

　「よーし、いい感じにlogstashフィルタがかけたよ！一度テキストに出力して動作確認してみよう！」

　お、もふもふちゃん、@<tt>{logstash.conf}が一通り書けたようですね。もふもふちゃんのいう通り、
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

　ちゃんと動作しているようですね。ただ、標準出力でデータを追いかけることは避けたいものです。
今度はKibanaを使ってデータを可視化してみましょう。
