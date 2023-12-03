#pragma once

#include "json.h"

namespace json {
    class Builder {
    private:
        class BaseContext;
        class DictItemContext;
        class ArrayItemContext;
        class KeyItemContext;
    public:
        BaseContext EndArray();
        BaseContext EndDict();
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        KeyItemContext Key(const std::string& key);
        Node Build();

        template<typename T>
        BaseContext Value(T value);
    private:

        class BaseContext {
        public:
            BaseContext(Builder& base) : builder_(base) {}

            template<typename T>
            BaseContext Value(T value) {
                return builder_.Value(value);
            }

            BaseContext StartArray() {
                return builder_.StartArray();
            }

            BaseContext EndArray() {
                return builder_.EndArray();
            }

            BaseContext StartDict() {
                return builder_.StartDict();
            }

            KeyItemContext Key(std::string key) {
                return builder_.Key(key);
            }

            BaseContext EndDict() {
                return builder_.EndDict();
            }

            Node Build() {
                return builder_.Build();
            }

        protected:
            Builder& builder_;
        };

        class DictItemContext : public BaseContext {
        public:
            DictItemContext(BaseContext base) : BaseContext(base) {}

            template<typename T>
            BaseContext Value(T value) = delete;

            BaseContext StartArray() = delete;
            BaseContext EndArray() = delete;
            BaseContext StartDict() = delete;
            Node Build() = delete;
        };

        class ArrayItemContext : public BaseContext {
        public:
            ArrayItemContext(BaseContext base) : BaseContext(base) {}

            template<typename T>
            ArrayItemContext Value(T value) {
                return builder_.Value(value);
            }

            BaseContext Key(std::string key) = delete;
            BaseContext EndDict() = delete;
            Node Build() = delete;
        };

        class KeyItemContext : public BaseContext {
        public:
            KeyItemContext(BaseContext base) : BaseContext(base) {}

            template<typename T>
            DictItemContext Value(T value) {
                return builder_.Value(value);
            }

            BaseContext EndArray() = delete;
            BaseContext Key(std::string key) = delete;
            BaseContext EndDict() = delete;
            Node Build() = delete;
        };

    private:
        Node route_;
        std::vector<Node*> nodes_stack_;
        std::string current_key_;
        bool availability_flag_ = false;
    };

    template<typename T>
    Builder::BaseContext Builder::Value(T value) {
        if (route_ == nullptr) {
            route_ = Node(value);
            return BaseContext(*this);
        } else if (nodes_stack_.back()->IsDict() && availability_flag_) {

            json::Dict& dict = std::get<json::Dict>(nodes_stack_.back()->GetValue());
            dict.emplace(current_key_, std::move(value));
            current_key_.clear();
            availability_flag_ = false;
            return Builder::DictItemContext(*this);
        } else if (nodes_stack_.back()->IsArray()) {
            json::Array& array = std::get<json::Array>(nodes_stack_.back()->GetValue());
            array.emplace_back(std::move(value));
            return Builder::ArrayItemContext(*this);
        } else {
            throw std::logic_error("Invalid state -> other");
        }
    }
}
