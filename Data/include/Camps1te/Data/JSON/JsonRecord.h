#pragma once

#include "JsonRecordData.h"

namespace Camps1te::Data {

    struct Record {
        virtual ~Record() = default;

        // TODO at some point
        // virtual RecordHeader* GetRecordHeader() const = 0;

        virtual const char* GetFullIdentifier() const     = 0;
        virtual const char* GetOwnerName() const          = 0;
        virtual const char* GetRelativeIdentifier() const = 0;

        virtual const char* GetType() const = 0;

        virtual RecordData* GetData() const = 0;
    };
}
