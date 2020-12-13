#include "../include/EarleyParser.h"



EarleySituation::EarleySituation(std::string rule_lhs, std::string rule_rhs,
                           uint32_t point_pos, uint32_t start_pos) :
                           GrammarRule(std::move(rule_lhs), std::move(rule_rhs)),
                           point_pos_(point_pos),
                           start_pos_(start_pos) {}

EarleySituation::EarleySituation(const GrammarRule &rule,
                           uint32_t point_pos, uint32_t start_pos) :
                           GrammarRule(rule),
                           point_pos_(point_pos),
                           start_pos_(start_pos) {}

char EarleySituation::GetSymbolAfterPoint() const {
  return rule_rhs_[point_pos_];
}

bool EarleySituation::IsComplete() const {
  return point_pos_ == rule_rhs_.size();
}

EarleySituation EarleySituation::MovePointForward() const {
  return {rule_lhs_, rule_rhs_, point_pos_ + 1, start_pos_};
}

bool EarleySituation::operator==(const EarleySituation& other) const {
  return rule_lhs_ == other.rule_lhs_ && rule_rhs_ == other.rule_rhs_ &&
      point_pos_ == other.point_pos_ && start_pos_ == other.start_pos_;
}


void EarleyParser::ResizeEarleySets(size_t size) {
  early_sets_.resize(size);
}

bool EarleyParser::Parse(const Grammar &grammar, std::string_view word) {
  early_sets_.clear();
  early_sets_.resize(word.size() + 1);

  EarleySituation new_rule {grammar.starting_symbol_ + "'",
                        grammar.starting_symbol_, 0 , 0};

  early_sets_[0].insert(new_rule);

  for (size_t cur_size = -1; cur_size != early_sets_[0].size();) {
    cur_size = early_sets_[0].size();

    Predict(grammar, 0);
    Complete(0);
  }

  for (int32_t i = 1; i <= word.size(); ++i) {
    Scan(i - 1, word[i - 1]);

    for (size_t cur_size = -1; cur_size != early_sets_[i].size();) {
      cur_size = early_sets_[i].size();

      Predict(grammar, i);
      Complete(i);
    }

  }

  new_rule.point_pos_ = 1;
  return early_sets_[word.size()].find(new_rule) !=
         early_sets_[word.size()].end();
}



void EarleyParser::Predict(const Grammar &grammar, size_t ind) {
  std::list<EarleySituation> buffer;
  for (const EarleySituation& situation : early_sets_[ind]) {

    std::string symbol_after_point (1, situation.GetSymbolAfterPoint());
    for (const GrammarRule& possible_cont : grammar.rules_list_) {
      if (possible_cont.rule_lhs_ == symbol_after_point) {
        buffer.emplace_back(possible_cont, 0, ind);
      }
    }
  }

  for (const EarleySituation& new_situation : buffer) {
    early_sets_[ind].insert(new_situation);
  }
}

void EarleyParser::Scan(int32_t ind, char symbol) {
  for (const EarleySituation& situation : early_sets_[ind]) {
    if (situation.GetSymbolAfterPoint() == symbol) {
      early_sets_[ind + 1].insert(situation.MovePointForward());
    }
  }
}

void EarleyParser::Complete(size_t ind) {
  std::list<EarleySituation> buffer;

  for (const EarleySituation& situation : early_sets_[ind]) {
    if (situation.IsComplete()) {
      for (const EarleySituation& situation_to_complete : early_sets_[situation.start_pos_]) {
        std::string symbol_after_point (1, situation_to_complete.GetSymbolAfterPoint());
        if (symbol_after_point == situation.rule_lhs_) {
          buffer.push_back(situation_to_complete.MovePointForward());
        }
      }
    }
  }
  for (const EarleySituation& new_situation : buffer) {
    early_sets_[ind].insert(new_situation);
  }
}
