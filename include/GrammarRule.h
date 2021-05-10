#include <string>
#include <utility>
#include <vector>
#include <list>
#include <unordered_set>
#include <string_view>

const std::string rule_delimiter = {"->"};

class GrammarRule {
public:
  std::string rule_lhs_;
  std::vector<std::string> rhs_tokens_;

  explicit GrammarRule(const std::string& rule);
  GrammarRule(const std::string& rule_lhs, const std::string& rule_rhs);
  GrammarRule(const std::string& rule_lhs,
              const std::vector<std::string>& rhs_tokens);
};
