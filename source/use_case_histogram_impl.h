#pragma once

#include "use_case.h"
#include "histogram.hpp"

namespace histogram {

class HistogramUseCaseImpl : public domain::AppUseCase  {
public:
    void SetHistogram( std::shared_ptr<HistogramModel> model) { histogram_model_ = model; }
    void AppendFrame(const model::WordsFrame & frame) override;
private:
    std::shared_ptr<HistogramModel> histogram_model_;
};

}
