#pragma once

#include "model.h"

namespace domain {
   class ReaderUseCase {
   public:
       virtual void SetFile(const std::string &) = 0;
       virtual model::MeasureStatus Start() = 0;
       virtual model::MeasureStatus Stop() = 0;
       virtual model::MeasureStatus Pause() = 0;
   };

   class AppUseCase {
   public:
        virtual void AppendFrame(const model::WordsFrame & frame) = 0;
   };
}
