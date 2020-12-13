#include "../include/GrammarRule.h"


GrammarRule::GrammarRule(const std::string& rule) {
  size_t del_pos = rule.find(rule_delimiter);
  if (del_pos == std::string::npos) {
    throw std::runtime_error("rule doesn't contain two parts");
  } else {
    rule_lhs_ = rule.substr(0, del_pos);
    rule_rhs_ = rule.substr(del_pos + rule_delimiter.size());
    if (rule_lhs_.empty() || rule_rhs_.empty()) {
      throw std::runtime_error("one of states is empty");
    } else if (rule_lhs_.size() >= 2) {
      throw std::runtime_error("Sorry this parser version doesn't"
                               " work with nonterminals of length > 1");
    }
    if (rule_rhs_ == "EPS") {
      rule_rhs_ = std::string();
    }
  }
}

GrammarRule::GrammarRule(std::string rule_lhs, std::string rule_rhs) :
                         rule_lhs_(std::move(rule_lhs)), rule_rhs_(std::move(rule_rhs)) {}
