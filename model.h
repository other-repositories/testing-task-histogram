#pragma once

#include <vector>
#include <string>

namespace model {

class WordsFrame {
public:

    using list_words_t = std::vector<std::pair<std::string, int>>;

    WordsFrame() = delete;
    WordsFrame(WordsFrame &) = default;
    WordsFrame(WordsFrame &&) = default;
    WordsFrame & operator=(WordsFrame &&) = default;
    WordsFrame & operator=(WordsFrame &) = default;

    WordsFrame(list_words_t vec, float procent_progress) : words_(std::move(vec)), procent_progress_(procent_progress) {}

    const list_words_t & GetWords() const { return words_; }
    float GetProcentProgress() const { return procent_progress_; }

private:
    list_words_t words_;
    float procent_progress_;
};

enum class MeasureStatus {
    Ok = 0x0,
    AlreadyStoped = 0x1,
    AlreadyStarted = 0x2,
    NotStarted = 0x3,
    NotStoped = 0x4
};

}
