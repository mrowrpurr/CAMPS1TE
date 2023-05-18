#pragma once

#include "RecordData.h"

namespace Camps1te::Data {

    struct Record {
        virtual ~Record() = default;

        // TODO at some point
        // virtual RecordHeader* GetRecordHeader() const = 0;

        virtual const char* GetFullIdentifier()     = 0;
        virtual const char* GetOwnerName()          = 0;
        virtual const char* GetRelativeIdentifier() = 0;

        virtual const char* GetType() = 0;

        virtual RecordData* GetData() = 0;
    };
}
