#include "data_reader.h"
#include <queue>
#include <cmath>
#include <fstream>
#include <mutex>
#include <filesystem>
#include <QDebug>

namespace fs = std::filesystem;

namespace std {
    bool operator<(const data_reader::word_count_pair_t& p1,const data_reader::word_count_pair_t& p2) {
        if (p1.second < p2.second)
            return true;
        if (p1.second == p2.second && p1.first < p2.first)
            return true;
        return false;
    }
}

namespace data_reader {

namespace {

std::vector<word_count_pair_t> TopNWords(const std::map<std::string, int>& wordsCount, size_t N) {
    std::vector<word_count_pair_t> top_words;
    top_words.reserve(N);
    for (const auto& pair : wordsCount) {
        if(top_words.size() < N) {
            top_words.push_back(pair);
            std::sort(top_words.rbegin(), top_words.rend());
        }
        else {
            if(top_words.back().second < pair.second) {
                top_words.push_back(pair);
                std::sort(top_words.rbegin(), top_words.rend());
                top_words.pop_back();
            }
        }
    }

    return top_words;
}

}

DataReader::DataReader(domain::AppUseCase &app_use_case)
: app_use_case_(app_use_case),
  last_procent_(0),
  state_(std::make_shared<impl::ReadyState>()) {}

model::MeasureStatus DataReader::Start() {
    return GetState()->StartEvent(*this);
}

model::MeasureStatus DataReader::Stop() {
    return GetState()->StopEvent(*this);
}

model::MeasureStatus DataReader::Pause() {
    return GetState()->PauseEvent(*this);
}

void DataReader::SetState(std::shared_ptr<impl::State> state) {
    std::lock_guard<std::mutex> lock(mt_);
    state_ = state;
}

std::shared_ptr<impl::State> DataReader::GetState() {
    mt_.lock();
    auto state = state_;
    mt_.unlock();
    return state;
}

void DataReader::SetFile(const std::string &path) {
    path_ = path;
}

void DataReader::Run() {
    SetState(std::make_shared<impl::MeasureState>());
    std::ifstream in(path_);
    bool is = in.is_open();
    auto full_count_bytes_in_file = fs::file_size(path_);
    std::string buffer;
    size_t word_counts = 0;

    while(!is_stopped_.load()) {
        std::getline(in, buffer);

        if(in.eof())
            break;

        word_counts += ParseWords(buffer);

        auto current_byte_position = static_cast<size_t>(in.tellg());
        last_procent_ = double(current_byte_position) / full_count_bytes_in_file;
        if(word_counts >= WORDS_SIZE_FRAME) {
            SendDataToUI();
            word_counts = 0;
        }

        if(is_paused_.load()) {
            SetState(std::make_shared<impl::PausedState>());
            std::mutex mt;
            std::unique_lock<std::mutex> lock(mt);
            pause_locker_.wait(lock,[&](){ return !is_paused_.load(); } );
            SetState(std::make_shared<impl::MeasureState>());
        }
    }
    last_procent_ = 0;
    SendDataToUI();
    words_count_.clear();
    SetState(std::make_shared<impl::ReadyState>());
}

size_t DataReader::ParseWords(std::string_view str) {
    std::string word;
    size_t count = 0;
    for (char c : str) {
        if (!WordSymbolCheck(c)) {
            if (!word.empty()) {
                ProcessString(std::move(word));
                word.clear();
                count++;
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
       ProcessString(std::move(word));
       count++;
    }
    return count;
}

bool DataReader::WordSymbolCheck(char c) {
    c = static_cast<char>(std::tolower(c));
    return (c >= 'a'&& c <= 'z');
}

void DataReader::ProcessString(std::string && str) {
    auto it = words_count_.find(str);
    if(it == words_count_.end())
        words_count_.insert({str, 1});
    else
        it->second++;
}

void DataReader::SendDataToUI() {
    app_use_case_.AppendFrame({TopNWords(words_count_, MAX_TOP_GENERATE), last_procent_});
}

model::MeasureStatus impl::ReadyState::StartEvent(DataReader & reader) {
    reader.is_stopped_.store(false);
    reader.is_paused_.store(false);
    reader.runner_ = std::thread([&]{
        reader.Run();
    });
    reader.runner_.detach();
    return model::MeasureStatus::Ok;
}

model::MeasureStatus impl::MeasureState::PauseEvent(DataReader & reader) {
    reader.is_paused_.store(true);
    return model::MeasureStatus::Ok;
}

model::MeasureStatus impl::MeasureState::StopEvent(DataReader & reader) {
    reader.is_stopped_.store(true);
    return model::MeasureStatus::Ok;
}

model::MeasureStatus impl::PausedState::PauseEvent(DataReader & reader) {
    reader.is_paused_.store(false);
    reader.pause_locker_.notify_one();
    return model::MeasureStatus::Ok;
}

model::MeasureStatus impl::PausedState::StopEvent(DataReader & reader) {
    reader.is_stopped_.store(true);
    reader.is_paused_.store(false);
    reader.pause_locker_.notify_one();
    return model::MeasureStatus::Ok;
}

}
