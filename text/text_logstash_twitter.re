= データを集めて可視化しよう（Twitter編）

//lead{
　「環境はセットアップできた！でもどうやってログを集めればいいんだっけ？
それにKibanaにアクセスしても何も出てこないよ？」
//}

　おや？もふもふちゃんは大事なことを忘れてしまっているようです。
Kibanaにデータを表示するためには、ElasticSearhにデータが入っていないとダメですよね。

//lead{
「確かにそうかも。今はElasticSearhの動作確認をしただけだから何もデータが入ってないなあ。
どうやってデータを取り込めばいいのかな？」
//}

　…もふもふちゃんの頭からはLogstashの存在がかき消されているようです。
今回の構成では、ElasticSearhにデータを送るツールとしてLogstashを利用するのでしたね。
まずはLogstashの設定をもふもふちゃんと一緒にやってみましょう。

　目標は「Twitterのつぶやきから"イチゴメロンパン"という文言が含まれているものだけを抽出し
Kibanaで分析しやすい形にしてElasticSearhに送る」ことです。

== Twitter APIの準備
　まずはTwitter APiを準備しましょう。

===== URLにアクセス

　まずはTwitter APIのURLにアクセスします。
あらかじめTwitterアカウントにログインしておく必要があります。

@<href>{https://dev.twitter.com}

　アクセス後、@<tt>{「Manage Your Apps」}リンクをクリックします。

//image[Twitter_Developers01][Twitterアプリ作成]{
/Usersmallow/review/text/text/images/Twitter_Developers01.png
//}

===== 必要事項の入力

欄内の必要事項を記入します。

//table[twitterApp][必要事項]{
名前  記載内容
-------------------
Name  アプリケーション名
Description アプリケーションの概要
Website アプリケーションを動かすためのトップページのURLを記載（ダミーデータ可）
Callback  ユーザ認証後の戻り先のURLアドレス
//}

//image[Twitter_Developers02][入力画面1]{
/Usersmallow/review/text/text/images/Twitter_Developers02.png
//}

//image[Twitter_Developers03][入力画面2]{
/Usersmallow/review/text/text/images/Twitter_Developers03.png
//}

//image[Twitter_Developers04][入力画面3]{
/Usersmallow/review/text/text/images/Twitter_Developers04.png
//}

　登録に成功すると、次のような画面が表示されます。

//image[Twitter_Developers05][入力画面5]{
  /Usersmallow/review/text/text/images/Twitter_Developers05.png
//}

===== 規約に同意して鍵をもらう

　情報登録が完了すると、鍵情報が表示されます。
@<tt>{Consumer Key (API Key)}はAPIの認証に必要です。
@<tt>{Consumer Secret (API Secret)}は認証に必要な鍵情報です。パスワードと同等の扱いのため、他の人には教えてはいけません。

== Logstashでデータを集めよう
　では、いよいよlogstash.confを編集していきます。
まずは、編集イメージを持つためにlogstash.confの構造をみてみましょう。

=== logstash.confの構造
　logstash.confはinput、filter、outputの3つにセクションが分かれています。
まずはこちらをみてください。

//image[logstash_config][logstash.confの構造]{
/Users/mallow/review/text/text/images/logstash_config.png
//}

==== input
　inputは、ログをどこから取得するか決める部分です。
ログの取得間隔や、Logstashサービス再開時の挙動を指定することも可能です。

==== filter
　filterは、ログをどのように加工・整形するか決める部分です。
「加工」とはいっても、取り込んだログの文言を書き換える、条件に一致するログを消去する等
方法は様々です。

==== output
　outputは、ログをどこに送るか指定する部分です。
前の章でも述べた通り、ElasticSearhへのデータ送付以外にもCSV形式など
指定した拡張子でデータを出力することも可能です。

=== inputプラグインを書いてみよう
　では、早速Twitterのログを取り込んでみましょう。1番始めの部分に当たります。

//image[logstash_config_input][logstash.conf（input）]{
/Users/mallow/review/text/text/images/logstash_config_input.png
//}

　input部分では情報の取得元ごとにプラグインが提供されています。
今回はTwitter情報を取得するため、twitterプラグインを使う指定を行います。
先ほど取得したTwitterの鍵情報を@<tt>{logstash.conf}に記載しましょう。

==== Twitterの検索キーワードを指定
　keywordsオプションを利用して検索したい単語、条件を指定できます。"[]"で囲わないと
構文エラーとなってしまいます。

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
//}

　複数検索したいものを指定する場合、コロンで繋げて記述します。

//cmd{
# イチゴメロンパン、logstash、elasticsearch、kibana、Elasticonを検索したい場合
keywords => ["イチゴメロンパン","logstash","elasticsearch","kibana","Elasticon"]
//}

=== outputプラグインを書いてみよう
　欲しい情報を集めた後は、ElasticSearhに送りたいですよね。
@<tt>{logstash.conf}では1番最後に指定する部分です。

//image[logstash_config_output][logstash.conf（output）]{
/Users/mallow/review/text/text/images/logstash_config_output.png
//}

==== ログの送付先を指定する
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

//image[Kibana_create_index01][Indexの紐付け]{
/Usersmallow/review/text/text/images/Kibana_create_index01.png
//}
　「見えた！」

　お、もふもふちゃんはうまく情報の紐付けができたようですね。紐付けができるとKibanaはこのような画面になっているはずです。

//image[Kibana_create_index02][Indexの紐付け後]{
/Usersmallow/review/text/text/images/Kibana_create_index02.png
//}

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
　今から編集する部分は、@<tt>{logstash.conf}の真ん中となる部分です。

//image[logstash_config_filter][logstash.conf（filter）]{
/Users/mallow/review/text/text/images/logstash_config_filter.png
//}

==== fieldとtextにデータをうまく分ける（ElasticSearhのデータの持ち方を説明）
　先ほどKibanaを閲覧したとき、データがうまく分かれていなかったと思います。
これはElasticSearhのデータの持ち方が@<tt>{field}に対する@<tt>{text}という構造になっているためです。
fieldとは、データベースでのカラムに相当します。textはカラムの中に入っている実データです。
カラムの中にデータを入れるのと同じように、field内にtextを保持します。
このfieldの集まりを@<tt>{index}といいます。

　つまり、ElasticSearh内のfieldごとにデータが入るようにログを取り込まないと
Kibanaでは1つのログの塊として認識されてしまい、データの区分けができないのです。
これではせっかくのKibanaの便利さも半減されてしまいます。
では、どのようにログを分割すれば良いのでしょう？

==== kvフィルタ：ログを分割する
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

==== removeフィルタ：いらない情報は捨てる

//lead{
　「うーん、ログにはTwitterアカウント名が含まれているけれど、ちょっとこれは個人情報っぽいから
あまりログ情報としては持っておきたくないなあ」
}

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

==== timestampフィルタ：ログが出力された時刻を編集する

#@# 忘れたので調べる

=== 最後に動作確認

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
