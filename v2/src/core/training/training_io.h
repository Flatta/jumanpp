//
// Created by Arseny Tolmachev on 2017/03/22.
//

#ifndef JUMANPP_TRAINING_IO_H
#define JUMANPP_TRAINING_IO_H

#include "core/analysis/extra_nodes.h"
#include "core/core.h"
#include "core/spec/spec_types.h"
#include "util/characters.h"
#include "util/csv_reader.h"

namespace jumanpp {
namespace core {
namespace training {

struct ExampleNode {
  StringPiece surface;
  util::ArraySlice<i32> data;
  i32 position;
  i32 length;
};

class FullyAnnotatedExample {
  std::string comment_;
  std::string surface_;
  std::vector<StringPiece> strings_;
  std::vector<i32> data_;
  std::vector<i32> lengths_;

  friend class TrainingDataReader;

 public:
  StringPiece surface() const { return surface_; }

  util::Sliceable<i32> data() {
    return {&data_, data_.size() / lengths_.size(), lengths_.size()};
  }

  ExampleNode nodeAt(i32 idx) const {
    JPP_DCHECK_IN(idx, 0, lengths_.size());
    i32 codeptPos = 0;
    for (int i = 0; i < idx; ++i) {
      codeptPos += lengths_[i];
    }
    i32 negIdx = 0;
    auto exampleData = const_cast<FullyAnnotatedExample*>(this)->data();
    auto examRow = exampleData.row(idx);
    for (i32 exEntry : examRow) {
      if (exEntry < 0) {
        negIdx = exEntry;
      }
    }

    StringPiece sp{};
    if (negIdx < 0) {
      sp = strings_[~negIdx];
    }

    return ExampleNode{sp, examRow, codeptPos, lengths_[idx]};
  }

  i32 numNodes() const { return static_cast<i32>(lengths_.size()); }

  StringPiece comment() const { return comment_; }

  void reset() {
    comment_.clear();
    surface_.clear();
    strings_.clear();
    data_.clear();
    lengths_.clear();
  }
};

enum class DataReaderMode { SimpleCsv, DoubleCsv };

struct TrainingExampleField {
  util::FlatMap<StringPiece, i32>* str2int;
  i32 dicFieldIdx;
  i32 exampleFieldIdx;
};

class TrainingDataReader {
  DataReaderMode mode_;
  util::CsvReader csv_;
  util::CsvReader csv2_;
  bool finished_;
  std::vector<util::FlatMap<StringPiece, i32>> storages_;
  std::vector<TrainingExampleField> fields_;
  std::vector<chars::InputCodepoint> codepts_;
  i32 surfaceFieldIdx_;
  char doubleFldSep_;

  Status readSingleExampleFragment(const util::CsvReader& csv,
                                   analysis::ExtraNodesContext* xtra,
                                   FullyAnnotatedExample* result);

 public:
  Status initialize(const spec::TrainingSpec& spec, const CoreHolder& core);

  Status initDoubleCsv(StringPiece data, char tokenSep = ' ',
                       char fieldSep = '_');

  Status initCsv(StringPiece data);
  bool finished() const { return finished_; }
  Status readFullExampleDblCsv(analysis::ExtraNodesContext* xtra,
                               FullyAnnotatedExample* result);
  Status readFullExampleCsv(analysis::ExtraNodesContext* xtra,
                            FullyAnnotatedExample* result);
  Status readFullExample(analysis::ExtraNodesContext* xtra,
                         FullyAnnotatedExample* result);

  i64 lineNumber() const { return csv_.lineNumber(); }

  void resetInput(StringPiece data) {
    csv_.initFromMemory(data);
    finished_ = false;
  }
};

}  // namespace training
}  // namespace core
}  // namespace jumanpp

#endif  // JUMANPP_TRAINING_IO_H
