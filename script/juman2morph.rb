#!/bin/ruby

#JUMAN の出力から morph の出力形式に変換するスクリプト
#usage: juman | juman2morph.rb 

# JUMANの出力形式
#隣 となり 隣 名詞 6 普通名詞 1 * 0 * 0 "代表表記:隣り/となり カテゴリ:場所-その他"
#の の の 助詞 9 接続助詞 3 * 0 * 0 NIL
#客 きゃく 客 名詞 6 普通名詞 1 * 0 * 0 "代表表記:客/きゃく 漢字読み:音 カテゴリ:人 ドメイン:家庭・暮らし;ビジネス"
#は は は 助詞 9 副助詞 2 * 0 * 0 NIL
#よく よく よい 形容詞 3 * 0 イ形容詞アウオ段 18 基本連用形 7 "代表表記:良い/よい 反義:形容詞:悪い/わるい"
#柿 かき 柿 名詞 6 普通名詞 1 * 0 * 0 "代表表記:柿/かき カテゴリ:植物 ドメイン:料理・食事"
#食う くう 食う 動詞 2 * 0 子音動詞ワ行 12 基本形 2 "代表表記:食う/くう ドメイン:料理・食事"
#客 きゃく 客 名詞 6 普通名詞 1 * 0 * 0 "代表表記:客/きゃく 漢字読み:音 カテゴリ:人 ドメイン:家庭・暮らし;ビジネス"
#だ だ だ 判定詞 4 * 0 判定詞 25 基本形 2 NIL
#EOS

#morphの出力形式
#隣_名詞:時相名詞 
#の_名詞:形式名詞 
#客_名詞:時相名詞 
#は_助詞:副助詞 
#よく_形容詞:基本連用形 
#柿_名詞:時相名詞 
#食う_動詞:基本形 
#客_名詞:時相名詞 
#だ_判定詞:基本形

morph_str = []
while line=gets
    if(line=~/EOS/)
        puts morph_str.join(' ')
        morph_str = []
    elsif(~/^@/) #別候補
        #skip
    else
        sp = line.split(' ')
        morph_str << "#{sp[0]}_#{sp[3]}:#{sp[5]}"#:#{spl[9]}"
    end
end
    
