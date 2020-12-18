#include "Grammar.h"

class EarleyParser;
class EarleySituation;
struct EarleySituationHash;

namespace EarleyFunctions {

  template<typename T, typename HashT>
  void Predict(std::vector<std::unordered_set<T, HashT>>& earley_sets,
               const Grammar& grammar, size_t ind);

  template<typename T, typename HashT>
  void Scan(std::vector<std::unordered_set<T, HashT>>& earley_sets,
            int32_t ind, const std::string& symbol);

  template<typename T, typename HashT>
  void Complete(std::vector<std::unordered_set<T, HashT>>& earley_sets,
                size_t ind);
}

class AbstractParser {
 protected:
  Grammar grammar_;
 public:
  explicit AbstractParser(Grammar grammar) : grammar_(std::move(grammar)) {}

  virtual bool Parse(const std::string& word) = 0;

};

class EarleyParser : public AbstractParser {

 public:
  EarleyParser(const Grammar& grammar) : AbstractParser(grammar) {}

  bool Parse(const std::string& word) override;

 private:
  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> earley_sets_;

};

class EarleySituation : public GrammarRule {
 public:
  uint32_t point_pos_;
  uint32_t start_pos_;


  EarleySituation(const std::string& rule_lhs, const std::string& rule_rhs,
                  uint32_t point_pos, uint32_t start_pos);

  EarleySituation(const std::string& rule_lhs, const std::vector<std::string>& rule_rhs,
                  uint32_t point_pos, uint32_t start_pos);

  EarleySituation(const GrammarRule& other, uint32_t point_pos, uint32_t start_pos);

  [[nodiscard]] std::string GetSymbolAfterPoint() const;
  [[nodiscard]] bool IsComplete() const;

  [[nodiscard]] EarleySituation MovePointForward() const;

  bool operator==(const EarleySituation& other) const;

};

struct EarleySituationHash {

  size_t operator()(const EarleySituation& obj) const {
    std::hash<std::string> str_hash {};
    std::hash<int32_t> int_hash {};
    size_t result = str_hash(obj.rhs_tokens_.front());
    for (size_t i = 1; i < obj.rhs_tokens_.size(); ++i) {
      result ^= str_hash(obj.rhs_tokens_[i]);
    }
    return result ^ str_hash( obj.rule_lhs_) ^ int_hash(obj.point_pos_) ^ \
           int_hash(obj.start_pos_);
  }
};
