#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <functional>

using namespace std;

class Domain {
public:
    Domain() = default;
    Domain(std::string_view domain);
    bool operator==(const Domain& rhs);
    bool IsSubdomain(const Domain& domain) const;
    std::string GetDomainName() const;
private:
    string domain_;
};

class DomainChecker {
public:
    template<typename Iter>
    DomainChecker(Iter begin, Iter end);
    bool IsForbidden(const Domain& domain);
private:
    std::vector<Domain> forbidden_domains_;
};

template<typename Number>
std::vector<Domain> ReadDomains(std::istream& input, const Number& row_count) {
    std::vector<Domain> read_result;
    std::string input_string;

    read_result.reserve(row_count);
    for (Number i = 0; i < row_count; ++i){
        getline(input, input_string);
        read_result.emplace_back(Domain(input_string));
    }

    return read_result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

//------Methods definitions----------

//---class Domain

Domain::Domain(std::string_view domain) {
    domain_ = domain.data();
    domain_.insert(domain_.begin(), '.');
    std::reverse(domain_.begin(), domain_.end());
}

bool Domain::operator==(const Domain& rhs){
    return domain_.compare(rhs.domain_);
}

bool Domain::IsSubdomain(const Domain& domain) const {
    if (this->domain_.size() >= domain.domain_.size()){
        return std::equal(domain.domain_.begin(), domain.domain_.end(), this->domain_.begin());
    }
    return false;
}

std::string Domain::GetDomainName() const { 
    return this->domain_; 
}

//--class DomainChecker

template<typename Iter>
DomainChecker::DomainChecker(Iter begin, Iter end)
    :forbidden_domains_(begin, end)
{
  
    std::sort(forbidden_domains_.begin(), forbidden_domains_.end(),
        [](const Domain& lhs, const Domain& rhs) { 
                return lhs.GetDomainName() < rhs.GetDomainName(); 
        }
    );
        
    auto iterator = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), 
        [](const Domain& lhs, const Domain& rhs){
            return rhs.IsSubdomain(lhs);
        }
    );

    forbidden_domains_.erase(iterator, forbidden_domains_.end());

}

bool DomainChecker::IsForbidden(const Domain& domain){
    if (forbidden_domains_.size() == 0)
        return false;
    auto iter = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, 
        [](const Domain& lhs, const Domain& rhs) { 
            return lhs.GetDomainName() < rhs.GetDomainName(); 
        }
    );
        if (iter != forbidden_domains_.begin()) 
            return domain.IsSubdomain(*std::prev(iter));
        else
            return false;
}
