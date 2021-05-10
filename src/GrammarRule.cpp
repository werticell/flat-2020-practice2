#include "../include/GrammarRule.h"


GrammarRule::GrammarRule(const std::string& rule) {
  size_t del_pos = rule.find(rule_delimiter);
  if (del_pos == std::string::npos) {
    throw std::runtime_error("rule doesn't contain two parts");
  } else {
    rule_lhs_ = rule.substr(0, del_pos);
    std::string str_rule_rhs = rule.substr(del_pos + rule_delimiter.size());

    if (rule_lhs_.empty() || str_rule_rhs.empty()) {
      throw std::runtime_error("one of states is empty");
    } else if (rule_lhs_.size() >= 2) {
      throw std::runtime_error("Sorry this parser version doesn't"
                               " work with nonterminals of length > 1");
    }

    if (str_rule_rhs == "EPS") {
      rhs_tokens_.emplace_back(std::string());
    } else {
      for (char symbol : str_rule_rhs) {
        rhs_tokens_.emplace_back(1, symbol);
      }
    }
  }
}

GrammarRule::GrammarRule(const std::string& rule_lhs, const std::string& rule_rhs) :
                         rule_lhs_(rule_lhs) {

  if (rule_rhs == "EPS") {
    rhs_tokens_.emplace_back(std::string());
  } else {
    for (char symbol : rule_rhs) {
      rhs_tokens_.emplace_back(1, symbol);
    }
  }

}

GrammarRule::GrammarRule(const std::string& rule_lhs,
                         const std::vector<std::string>& rhs_tokens) :
    rule_lhs_(rule_lhs),
    rhs_tokens_(rhs_tokens) {}
