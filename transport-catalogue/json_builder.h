#pragma once

#include <optional>
 
#include "json.h"

namespace json {

    enum class Operation {
        BUILDER,
        ARRAY,
        DICTIONARY
    };

    class DictItemContext;
    class DictValueContext;
    class ArrayItemContext;

    class Builder {
    public:
        Builder() {
            operation_stack_.push_back(Operation::BUILDER);
        }

        DictItemContext StartDict();
        Builder& EndDict();
        ArrayItemContext StartArray();
        Builder& EndArray();
        DictValueContext Key(const std::string& key);
        Builder& Value(const Node::Value& value);
        Node Build() const;
        void AddNode(Node&& node);

    private:
        std::optional<Node> root_;
        std::vector<Operation> operation_stack_;
        std::vector< std::optional<std::string> > keys_;
        int dicts_open_ = 0;
        int arrays_open_ = 0;
        std::vector<std::vector<Node>> all_arrays_;
        std::vector< std::vector<std::pair<std::string, Node>> > all_dicts_;
    };

    class DictItemContext {
    public:
        DictItemContext(Builder& builder)
            : builder_(builder) {}

        DictValueContext Key(const std::string& key);
        Builder& EndDict();

    private:
        Builder& builder_;
    };

    class DictValueContext {
    public:
        DictValueContext(Builder& builder)
            : builder_(builder) {}

        DictItemContext Value(const Node::Value& value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();

    private:
        Builder& builder_;
    };

    class ArrayItemContext {
    public:
        ArrayItemContext(Builder& builder)
            : builder_(builder) {}

        ArrayItemContext Value(const Node::Value& value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        Builder& EndArray();

    private:
        Builder& builder_;
    };
}