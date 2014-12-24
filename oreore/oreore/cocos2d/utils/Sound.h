#ifndef __OREORE_COCOS2D_SOUND_H__
#define __OREORE_COCOS2D_SOUND_H__

#include <memory>
#include <unordered_map>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

namespace oreore
{
    /**
     * @brief サウンドに関する処理を行うクラスです
     */
    class SoundManager final : public cocos2d::Node
    {
    public:
        /**
         * @brief サウンドファイルのリストです
         * プリロードするファイルの指定等に利用します
         */
        using FileList = std::vector<std::string>;

    private:
        bool enabled, bgmMute, seMute;
        float bgmVolume, seVolume, storedBGMVolume, storedSEVolume;
        std::string reservedMusicFile, currentlyPlaying;
        std::function<std::string(const std::string &, const bool)> resolver;

        static std::unique_ptr<SoundManager> manager;
        CREATE_FUNC(SoundManager);
        bool init() override;

        void completeFading();
        void completeFadeOut();

        template<typename T>
        void _preload(const T &list);

        template<typename T>
        void _unload(const T &list);
    public:
        /**
         * @brief インスタンスを取得します
         */
        static SoundManager &getInstance();

        /**
         * @brief BGMを再生します
         * @param filename 再生するBGMのファイル名
         * @param duration フェードインする時間。デフォルト値は0
         * @param loop BGMをループさせるかどうか。デフォルト値はtrue
         */
        void playBGM(const std::string &filename, const float duration = 0.0f, const bool loop = true);

        /**
         * @brief 再生中のBGMを停止します
         * @param duration フェードアウトする時間。デフォルト値は0
         */
        void stopBGM(const float duration = 0.0f);

        /**
         * @brief 効果音を再生します
         * @param filename 効果音のファイル名
         * @param loop 効果音をループさせるかどうか。デフォルト値はfalse
         */
        unsigned int playSE(const std::string &filename, const bool loop = false);

        /**
         * @brief 再生中の効果音を停止します
         * @param id 再生中の効果音のID
         */
        void stopSE(const unsigned int id);

        /**
         * @brief BGMの音量を取得します
         */
        float getBGMVolume() const;

        /**
         * @brief BGMの音量を設定します
         * @param volume 音量(0.0〜1.0)
         */
        void setBGMVolume(const float volume);

        /**
         * @brief 効果音の音量を取得します
         */
        float getSEVolume() const;

        /**
         * @brief 効果音の音量を設定します
         * @param volume 音量(0.0〜1.0)
         */
        void setSEVolume(const float volume);

        /**
         * @brief BGMの最大音量を取得します
         */
        inline float getMaxBGMVolume() const { return bgmVolume; }

        /**
         * @brief BGMの最大音量を設定します
         * @param volume 最大音量(0.0〜1.0)
         */
        void setMaxBGMVolume(const float volume);

        /**
         * @brief 効果音の最大音量を取得します
         */
        inline float getMaxSEVolume() const { return seVolume; }

        /**
         * @brief 効果音の最大音量を設定します
         * @param volume 最大音量(0.0〜1.0)
         */
        void setMaxSEVolume(const float volume);

        /**
         * @brief BGMをフェードインさせます
         * @param duration フェード時間
         */
        void fadeIn(const float duration);

        /**
         * @brief BGMをフェードアウトさせます
         * @param duration フェード時間
         * @param pauseOnComplete ボリュームが0になった際に、BGMを停止するかポーズするかの指定。デフォルト値はfalse
         */
        void fadeOut(const float duration, const bool pauseOnComplete = false);

        /**
         * @brief BGMを指定したボリュームまでフェードさせます
         * @param volume フェード先の音量
         * @param duration フェード時間
         */
        void fadeTo(const float volume, const float duration);

        /**
         * @brief BGMを擬似的にクロスフェードさせます
         * cocosDenshionではBGMのクロスフェードがサポートされていないので、
         * 再生中のBGMをフェードアウト、BGM切り替え、BGMフェードインを指定された時間で
         * 行い、擬似的なクロスフェードを行います
         * @param filename 再生するBGMファイル名
         * @param duration クロスフェードする時間
         */
        bool playWithFading(const std::string &filename, const float duration);


        /**
         * @brief 音声の有効無効を切り替えます
         * @param enabled 有効にするかどうか
         */
        void setEnabled(const bool enabled);

        /**
         * @brief 音声が有効かどうかを取得します
         */
        inline bool isEnabled() const { return enabled; }

        /**
         * @brief BGMをミュートするかどうかの設定です
         * @param mute ミュートするかどうか
         */
        void setBGMMute(const bool mute);

        /**
         * @brief BGMがミュート状態かどうかを取得します
         */
        inline bool isBGMMute() const { return bgmMute; }

        /**
         * @brief SEをミュートするかどうかの設定です
         * @param mute ミュートするかどうか
         */
        void setSEMute(const bool mute);

        /**
         * @brief SEがミュート状態かどうかを取得します
         */
        inline bool isSEMute() const { return seMute; }


        /**
         * @brief 効果音をプリロードします
         * BGMのプリロードは出来ません
         * @param list 効果音ファイルのリスト
         */
        void preload(const std::initializer_list<std::string> &list);

        /**
         * @brief 効果音をプリロードします
         * BGMのプリロードは出来ません
         * @param list 効果音ファイルのリスト
         */
        void preload(const FileList &fileList);

        /**
         * @brief プリロードされた効果音をアンロードします
         * @param list アンロードしたい効果音ファイルのリスト
         */
        void unload(const std::initializer_list<std::string> &list);

        /**
         * @brief プリロードされた効果音をアンロードします
         * @param list アンロードしたい効果音ファイルのリスト
         */
        void unload(const FileList &fileList);

        /**
         * @brief 音声ファイル名解決用の関数を登録します
         * AndroidとiOSで使用できるファイルの拡張子が異なるため、
         * SoundManagerで指定された名前は一度ここで指定された関数を通して
         * パス解決が行われます
         * @param resolver 解決に使用する関数。第一引数は指定されたファイル名。第二引数はBGMならtrue、効果音ならfalse
         */
        void registerExtResolver(const std::function<std::string(const std::string &, const bool)> &resolver)
        {
            this->resolver = resolver;
        }

        /**
         * @brief 現在再生中のBGM名を取得します
         * 返却されるBGM名には、リゾルバによる加工は施されません
         * また、再生中でない場合は空文字列が返ります
         */
        std::string getCurrentBGMName() const { return currentlyPlaying; }
    };
}

#endif

