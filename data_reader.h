#pragma once

#include "use_case.h"
#include <map>
#include <memory>

#include <thread>
#include <condition_variable>
#include <atomic>
#include <string_view>

//#include <thread>
//#include <mutex>

namespace data_reader {

using word_count_pair_t = std::pair<std::string, int>;
class DataReader;

namespace impl {

class State {
public:
    virtual model::MeasureStatus PauseEvent(DataReader &) = 0;
    virtual model::MeasureStatus StopEvent(DataReader &) = 0;
    virtual model::MeasureStatus StartEvent(DataReader &) = 0;
};

class ReadyState : public State {
public:
    model::MeasureStatus PauseEvent(DataReader &) override { return model::MeasureStatus::NotStarted; }
    model::MeasureStatus StopEvent(DataReader &) override { return model::MeasureStatus::NotStarted; }
    model::MeasureStatus StartEvent(DataReader &) override ;
};

class MeasureState : public State  {
    model::MeasureStatus PauseEvent(DataReader &) override ;
    model::MeasureStatus StopEvent(DataReader &) override ;
    model::MeasureStatus StartEvent(DataReader &) override  { return model::MeasureStatus::AlreadyStarted; }
};

class PausedState : public State  {
    model::MeasureStatus PauseEvent(DataReader &) override ;
    model::MeasureStatus StopEvent(DataReader &) override;
    model::MeasureStatus StartEvent(DataReader &) override {return model::MeasureStatus::AlreadyStarted;}
};

}


class DataReader {
public:
    static constexpr int WORDS_SIZE_FRAME = 1'000;
    static constexpr int MAX_TOP_GENERATE = 15;

    friend class impl::ReadyState;
    friend class impl::MeasureState;
    friend class impl::PausedState;

    DataReader(domain::AppUseCase & app_use_case);
    model::MeasureStatus Start();
    model::MeasureStatus Stop();
    model::MeasureStatus Pause();

    void SetFile(const std::string &);

protected:
    void SetState(std::shared_ptr<impl::State> state);
    std::shared_ptr<impl::State> GetState();
private:
    void Run();

    size_t ParseWords(std::string_view str);
    bool WordSymbolCheck(char c);

    void ProcessString(std::string && str);

    void SendDataToUI();

    std::map<std::string, int> words_count_;
    domain::AppUseCase & app_use_case_;

    std::shared_ptr<impl::State> state_;

    std::atomic_bool is_stopped_;
    std::atomic_bool is_paused_;
    float last_procent_;

    std::string path_;

    std::mutex mt_;
    std::condition_variable pause_locker_;
    std::thread runner_;
};

}

