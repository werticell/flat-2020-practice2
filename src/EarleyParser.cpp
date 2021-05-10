#include "../include/EarleyParser.h"


EarleySituation::EarleySituation(const std::string& rule_lhs, const std::string& rule_rhs,
                           uint32_t point_pos, uint32_t start_pos) :
                           GrammarRule(rule_lhs, rule_rhs),
                           point_pos_(point_pos),
                           start_pos_(start_pos) {}

EarleySituation::EarleySituation(const GrammarRule &rule,
                           uint32_t point_pos, uint32_t start_pos) :
                           GrammarRule(rule),
                           point_pos_(point_pos),
                           start_pos_(start_pos) {}

EarleySituation::EarleySituation(const std::string& rule_lhs,
                                 const std::vector<std::string>& rule_rhs,
                                 uint32_t point_pos, uint32_t start_pos) :
                                 GrammarRule(rule_lhs, rule_rhs),
                                 point_pos_(point_pos),
                                 start_pos_(start_pos) {}


std::string EarleySituation::GetSymbolAfterPoint() const {
  return rhs_tokens_[point_pos_];
}

bool EarleySituation::IsComplete() const {
  if (rhs_tokens_.front().empty()) {
    return true;
  } else {
    return point_pos_ == rhs_tokens_.size();
  }
}

EarleySituation EarleySituation::MovePointForward() const {
  return {rule_lhs_, rhs_tokens_, point_pos_ + 1, start_pos_};
}

bool EarleySituation::operator==(const EarleySituation& other) const {
  return rule_lhs_ == other.rule_lhs_ && rhs_tokens_ == other.rhs_tokens_ &&
      point_pos_ == other.point_pos_ && start_pos_ == other.start_pos_;
}



bool EarleyParser::Parse(const std::string& word) {
  earley_sets_.clear();
  earley_sets_.resize(word.size() + 1);

  EarleySituation new_rule {grammar_.starting_symbol_ + "'",
                        grammar_.starting_symbol_, 0 , 0};

  earley_sets_[0].insert(new_rule);

  for (size_t cur_size = -1; cur_size != earley_sets_[0].size();) {
    cur_size = earley_sets_[0].size();

    EarleyFunctions::Predict(earley_sets_, grammar_, 0);
    EarleyFunctions::Complete(earley_sets_, 0);
  }

  for (int32_t i = 1; i <= word.size(); ++i) {
    EarleyFunctions::Scan(earley_sets_, i - 1,
                          std::string(1, word[i - 1]));

    for (size_t cur_size = -1; cur_size != earley_sets_[i].size();) {
      cur_size = earley_sets_[i].size();
      EarleyFunctions::Predict(earley_sets_,grammar_, i);
      EarleyFunctions::Complete(earley_sets_, i);
    }

  }

  new_rule.point_pos_ = 1;
  return earley_sets_[word.size()].find(new_rule) !=
         earley_sets_[word.size()].end();
}


template<typename T, typename HashT>
void EarleyFunctions::Predict(std::vector<std::unordered_set<T, HashT>>& earley_sets,
                              const Grammar& grammar, size_t ind) {
  std::vector<EarleySituation> buffer;
  for (const EarleySituation& situation : earley_sets[ind]) {

    if (!situation.IsComplete()) {
      std::string symbol_after_point = situation.GetSymbolAfterPoint();
      for (const GrammarRule& possible_cont : grammar.rules_list_) {
        if (possible_cont.rule_lhs_ == symbol_after_point) {
          buffer.emplace_back(possible_cont, 0, ind);
        }
      }
    }

  }

  for (const EarleySituation& new_situation : buffer) {
    earley_sets[ind].insert(new_situation);
  }
}

template<typename T, typename HashT>
void EarleyFunctions::Scan(std::vector<std::unordered_set<T, HashT>>& earley_sets,
                           int32_t ind, const std::string& symbol) {

  for (const EarleySituation& situation : earley_sets[ind]) {
    if (!situation.IsComplete() && situation.GetSymbolAfterPoint() == symbol) {
      earley_sets[ind + 1].insert(situation.MovePointForward());
    }
  }
}

template<typename T, typename HashT>
void
EarleyFunctions::Complete(std::vector<std::unordered_set<T, HashT>>& earley_sets,
                               size_t ind) {
  std::vector<EarleySituation> buffer;

  for (const EarleySituation& situation : earley_sets[ind]) {
    if (situation.IsComplete()) {
      for (const EarleySituation& situation_to_complete : earley_sets[situation.start_pos_]) {
        if (!situation_to_complete.IsComplete()) {
          std::string symbol_after_point = situation_to_complete.GetSymbolAfterPoint();
          if (symbol_after_point == situation.rule_lhs_) {
            buffer.push_back(situation_to_complete.MovePointForward());
          }
        }
      }
    }
  }
  for (const EarleySituation& new_situation : buffer) {
    earley_sets[ind].insert(new_situation);
  }
}
