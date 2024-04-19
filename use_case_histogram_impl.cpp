#include "use_case_histogram_impl.h"

void histogram::HistogramUseCaseImpl::AppendFrame(const model::WordsFrame &frame) {
    if(!histogram_model_)
        return;

    histogram::top_list_t list;

    std::transform(frame.GetWords().begin(),frame.GetWords().end(),std::back_inserter(list),
    [](const std::pair<std::string,int> & str) -> histogram::top_list_t::value_type {
        return {QString::fromStdString(str.first),str.second};
    });

    emit histogram_model_->PushData(list, frame.GetProcentProgress());
}
