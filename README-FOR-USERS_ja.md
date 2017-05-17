# Virtual Mapperの使い方 (ユーザー向け)


**重要: 最新バージョンはmacOSのみの対応です。**


## どういうソフトか

ライブやインスタレーション向けの映像制作の際、立体な配置・形状のスクリーンの展開図の上にグラフィックを配置していくケースが多いかと思います。

そういった時に、制作画面や映像ファイルを3D空間状に配置し、実際の見え方に近い形でリアルタイムプレビューするツールが、Virtual Mapperです。

テンプレート制作者は、あらかじめ3D空間のカメラ位置、スクリーン形状などを**FBX**形式でエクスポートしておきます。

macOSの場合はSyphon、Windowsの場合はSpoutという方式でソフト間で映像を転送するため、映像ソフトによっては対応プラグインが必要になる場合があります。

[このビデオ](https://t.co/rwHJ5RuFHn)が分かりやすい例です。



##  手順

### 1. Virtual Mapperを[ダウンロード](https://github.com/baku89/VirtualMapper/releases) (macOSのみ対応)

### 2. 映像ソフト側にプラグインが必要な場合、インストールする

 - Adobe After Effects ([AESyphon](http://tobiasebsen.dk/experiments/aesyphon/))

### 3. Virtual Mapperを起動する

### 4. FDXファイルをロードする

![](./doc/thumbnail.png)

起動画面のサイドバー右上 `Scene > Open Scene` より、FBXファイルを選択。

### 5. 入力ソースを選択

サイドバー `Source` より、入力ソースを選択。

 - **Syphon** : After Effects等、外部ソフトからのプレビュー入力
 - **Image** : 静止画 (JPG, PNG等)
 - **Video** : 動画。macOSの場合、Quick Time Xでそのまま再生出来るファイル形式に対応

## 操作

カメラの基本操作は以下

 - **左クリック + ドラッグ** : 視点回転
 - **中央クリック + ドラッグ または [H] + ドラッグ** : 視点平行移動
 - **右クリック + ドラッグ** : 視点ズーム
 - **[1-9]** : カメラ切り替え

 その他のショートカットは、サイドバー `Misc > Help` から確認できます。
 
 