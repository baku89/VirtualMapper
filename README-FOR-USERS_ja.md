# Virtual Mapperの使い方 (ユーザー向け)


**重要: 最新バージョンはmacOSのみの対応です。**


## どういうソフトか

ライブやプロジェクションマッピング、インスタレーション向けの映像など、立体的な形状のスクリーンに投影する映像を作る場合、展開図のガイドの上にグラフィックを配置していくケースが多いかと思います。

そういった際に、制作ソフトのプレビュー画面や映像ファイルをステージの3Dモデル上に貼り付け、立体的にリアルタイムプレビューできるツールがVirtual Mapperです。

テンプレート制作者はあらかじめ3D空間のカメラ位置、スクリーン形状などのシーンデータを**FBX形式**でエクスポートしておきます。各アーティストは配布されたシーンデータを読み込み、制作ソフトの外部出力や映像ファイルなどのソースを指定することでプレビューが可能になります。

macOSの場合はSyphon、Windowsの場合はSpoutという方式でソフト間で映像を転送するため、映像ソフトによっては対応プラグインが必要になる場合があります。

[このビデオ](https://t.co/rwHJ5RuFHn)が分かりやすい例です。

##  手順

### 1. Virtual Mapperを[ダウンロード](https://github.com/baku89/VirtualMapper/releases) (最新バージョンmacOSのみ対応)

### 2. 制作ソフト側に外部出力用プラグインが必要な場合、インストールする

macOSの場合は[こちら](http://syphon.v002.info/)、Windowsの場合は[こちら](http://spout.zeal.co/)のサイトを参考に、各ソフト向けのプラグインを入手してください。以下は例です。

- Adobe After Effects
  + macOS: [AESyphon](http://tobiasebsen.dk/experiments/aesyphon/)) (フリー版は1024×768までの出力に制限されます)
  + Windows: [SpoutAE](https://github.com/benkuper/Spout-AfterEffects/releases)

### 3. Virtual Mapperを起動する

### 4. FBXファイルをロードする

![](./doc/thumbnail.png)

起動画面のサイドバー右上 `Scene > Open Scene` より、FBXファイルを選択。

### 5. 入力ソースを選択

サイドバー `Source` より、入力ソースを選択。

 - **Syphon** : (macOS) After Effects等、外部ソフトからのプレビュー入力
 - **Image** : 静止画 (JPG, PNG等)
 - **Video** : 動画。macOSの場合、Quick Time Xでそのまま再生出来るファイル形式に対応

## 操作

カメラの基本操作は以下になります。

 - **左クリック + ドラッグ** : 視点回転
 - **中央クリック + ドラッグ | [H] + ドラッグ | [スペース] + ドラッグ** : 視点平行移動
 - **右クリック + ドラッグ** : 視点ズーム
 - **[1-9]** : カメラ切り替え

 その他のショートカットは、サイドバー `Misc > Help` から確認できます。
 
 
