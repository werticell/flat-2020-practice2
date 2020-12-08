#include "Grammar.h"



class EarleyParser {
 public:
  class EarleySituation;
  struct EarleySituationHash;

  EarleyParser() = default;

  bool Parse(const Grammar& grammar, std::string_view word);


  void Predict(const Grammar& grammar, size_t ind);
  void Scan(int32_t ind, char symbol);
  void Complete(size_t ind);

 private:
  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> early_sets_;

};

class EarleyParser::EarleySituation {
 public:
  GrammarRule rule_;
  uint32_t point_pos_;
  uint32_t start_pos_;


  EarleySituation(std::string rule_lhs, std::string rule_rhs,
                  uint32_t point_pos, uint32_t start_pos);

  EarleySituation(const GrammarRule& other, uint32_t point_pos, uint32_t start_pos);

  char GetSymbolAfterPoint() const;
  bool IsComplete() const;

  EarleySituation MovePointForward() const;

  bool operator==(const EarleySituation& other) const {
    return rule_.rule_lhs_ == other.rule_.rule_lhs_ &&
           rule_.rule_rhs_ == other.rule_.rule_rhs_ &&
           point_pos_ == other.point_pos_ &&
           start_pos_ == other.start_pos_;
  }

};

struct EarleyParser::EarleySituationHash {
  size_t operator()(const EarleySituation& obj) const {
    return std::hash<std::string>{}(obj.rule_.rule_rhs_ + obj.rule_.rule_lhs_ + \
                                    std::to_string(obj.point_pos_) + \
                                    std::to_string(obj.start_pos_));
  }
};

