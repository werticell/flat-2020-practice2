#include "Grammar.h"

class EarleySituation;

class EarleyParser {
 public:
  struct EarleySituationHash;

  bool Parse(const Grammar& grammar, std::string_view word);


  void Predict(const Grammar& grammar, size_t ind);
  void Scan(int32_t ind, char symbol);
  void Complete(size_t ind);

  std::unordered_set<EarleySituation, EarleySituationHash>&
      operator[](size_t ind) { return early_sets_[ind]; }
  void ResizeEarleySets(size_t size);

 private:
  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> early_sets_;

};

class EarleySituation : public GrammarRule {
 public:
  uint32_t point_pos_;
  uint32_t start_pos_;


  EarleySituation(std::string rule_lhs, std::string rule_rhs,
                  uint32_t point_pos, uint32_t start_pos);

  EarleySituation(const GrammarRule& other, uint32_t point_pos, uint32_t start_pos);

  [[nodiscard]] char GetSymbolAfterPoint() const;
  [[nodiscard]] bool IsComplete() const;

  [[nodiscard]] EarleySituation MovePointForward() const;

  bool operator==(const EarleySituation& other) const;

};

struct EarleyParser::EarleySituationHash {
  size_t operator()(const EarleySituation& obj) const {
    return std::hash<std::string>{}(obj.rule_rhs_ + obj.rule_lhs_ + \
                                    std::to_string(obj.point_pos_) + \
                                    std::to_string(obj.start_pos_));
  }
};
