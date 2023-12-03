#pragma once

#include <map>
#include <vector>
#include <string>
#include <variant>
#include <iostream>

namespace json {
    class Node;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node final : private std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string> {
    public:
        using variant::variant;
        using Value = variant;

        bool IsNull() const {
            return std::holds_alternative<std::nullptr_t>(*this);
        }
        bool IsInt() const {
            return std::holds_alternative<int>(*this);
        }
        bool IsDouble() const {
            return IsInt() || IsPureDouble();
        }
        bool IsPureDouble() const {
            return std::holds_alternative<double>(*this);
        }
        bool IsString() const {
            return std::holds_alternative<std::string>(*this);
        }
        bool IsArray() const {
            return std::holds_alternative<Array>(*this);
        }
        bool IsDict() const {
            return std::holds_alternative<Dict>(*this);
        }
        bool IsBool() const {
            return std::holds_alternative<bool>(*this);
        }
             
        int AsInt() const {
            using namespace std::literals;
            if (!IsInt()) {
                throw std::logic_error("Not an integer"s);
            }
            return std::get<int>(*this);
        }
        
        bool AsBool() const {
            using namespace std::literals;
            if (!IsBool()) {
                throw std::logic_error("Not a boolean"s);
            }

            return std::get<bool>(*this);
        }

        double AsDouble() const {
            using namespace std::literals;
            if (!IsDouble()) {
                throw std::logic_error("Not a double"s);
            }
            return IsPureDouble() ? std::get<double>(*this) : AsInt();
        }
        
        const std::string& AsString() const {
            using namespace std::literals;
            if (!IsString()) {
                throw std::logic_error("Not a string"s);
            }

            return std::get<std::string>(*this);
        }
        
        const Array& AsArray() const {
            using namespace std::literals;
            if (!IsArray()) {
                throw std::logic_error("Not an array"s);
            }

            return std::get<Array>(*this);
        }
        
        Array& AsArray() {
            using namespace std::literals;
            if (!IsArray()) {
                throw std::logic_error("Not an array"s);
            }

            return std::get<Array>(*this);
        }
  
        const Dict& AsDict() const {
            using namespace std::literals;
            if (!IsDict()) {
                throw std::logic_error("Not a dict"s);
            }

            return std::get<Dict>(*this);
        }
        Dict& AsDict() {
            using namespace std::literals;
            if (!IsDict())
            {
                throw std::logic_error("Not a dict"s);
            }

            return std::get<Dict>(*this);
        }


        bool operator==(const Node& rhs) const {
            return GetValue() == rhs.GetValue();
        }

        const Value& GetValue() const {
            return *this;
        }

        Value& GetValue() {
            return *this;
        }
    };

    inline bool operator!=(const Node& lhs, const Node& rhs) {
        return !(lhs == rhs);
    }

    class Document {
    public:
        explicit Document(Node route) : route_(std::move(route)) {}

        const Node& GetRoute() const {
            return route_;
        }

    private:
        Node route_;
    };

    inline bool operator==(const Document& lhs, const Document& rhs) {
        return lhs.GetRoute() == rhs.GetRoute();
    }

    inline bool operator!=(const Document& lhs, const Document& rhs) {
        return !(lhs == rhs);
    }

    Document Load(std::istream& input);
    
    void Print(const Document& doc, std::ostream& output);

}
