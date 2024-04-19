#pragma once

#include "use_case.h"
#include "data_reader.h"
#include <memory>

namespace data_reader {

class ReaderUseCaseImpl : public domain::ReaderUseCase  {
public:
    void SetReader(std::shared_ptr<data_reader::DataReader> reader) {
        reader_ = reader;
    }

    void SetFile(const std::string & path) override { reader_->SetFile(path); }
    model::MeasureStatus Start() override { return reader_->Start(); }
    model::MeasureStatus Stop() override { return reader_->Stop(); }
    model::MeasureStatus Pause() override { return reader_->Pause(); }
private:
    std::shared_ptr<data_reader::DataReader>  reader_;
};

}
