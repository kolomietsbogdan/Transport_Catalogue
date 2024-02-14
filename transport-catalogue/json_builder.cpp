#include "json_builder.h"

namespace json {
    using namespace std::literals;

    DictItemContext Builder::StartDict() {
        if (operation_stack_.empty()) throw std::logic_error("Called without builder"s);
        if (root_) throw std::logic_error("Build is complete"s);
        ++dicts_open_;
        keys_.resize(dicts_open_);
        all_dicts_.resize(dicts_open_);
        operation_stack_.push_back(Operation::DICTIONARY);
        return DictItemContext(*this);
    }

    Builder& Builder::EndDict() {
        if (operation_stack_.back() == Operation::DICTIONARY) {
            if (dicts_open_ > 0) {
                Dict dict;
                for (auto& [key, value] : all_dicts_[dicts_open_ - 1]) {
                    dict[key] = std::move(value);
                }
                operation_stack_.pop_back();
                --dicts_open_;
                keys_.resize(dicts_open_);
                all_dicts_.resize(dicts_open_);
                Node node_dict(std::move(dict));
                AddNode(std::move(node_dict));
            }
            else throw std::logic_error("Zero opened dicts"s);
        }
        else throw std::logic_error("Cannot close dict"s);
        return *this;
    }

    ArrayItemContext Builder::StartArray() {
        if (operation_stack_.empty()) throw std::logic_error("Called without builder"s);
        if (root_) throw std::logic_error("Build is complete"s);
        ++arrays_open_;
        all_arrays_.resize(arrays_open_);
        operation_stack_.push_back(Operation::ARRAY);
        return *this;
    }

    Builder& Builder::EndArray() {
        if (operation_stack_.back() == Operation::ARRAY) {
            if (arrays_open_ > 0) {
                Array arr;
                for (auto& elem : all_arrays_[arrays_open_ - 1]) {
                    arr.push_back(std::move(elem));
                }
                operation_stack_.pop_back();
                --arrays_open_;
                all_arrays_.resize(arrays_open_);
                Node node_arr(std::move(arr));
                AddNode(std::move(node_arr));
            }
            else throw std::logic_error("Zero opened arrays"s);
        }
        else throw std::logic_error("Cannot close array"s);
        return *this;
    }

    DictValueContext Builder::Key(const std::string& key) {
        if (operation_stack_.back() == Operation::DICTIONARY) {
            if (dicts_open_ > 0) {
                if (!keys_[dicts_open_ - 1]) {
                    keys_[dicts_open_ - 1] = key;
                }
                else throw std::logic_error("Key has been set already"s);
            }
            else throw std::logic_error("Zero opened dicts"s);
        }
        else throw std::logic_error("Need to create a dict"s);
        return *this;
    }

    Builder& Builder::Value(const Node::Value& value) {
        if (operation_stack_.empty()) throw std::logic_error("Called without builder"s);
        if (root_) throw std::logic_error("Build is complete"s);
        Node node;
        if (std::holds_alternative<std::string>(value)) {
            node = Node(std::get<std::string>(value));
        }
        else if (std::holds_alternative<int>(value)) {
            node = Node(std::get<int>(value));
        }
        else if (std::holds_alternative<bool>(value)) {
            node = Node(std::get<bool>(value));
        }
        else if (std::holds_alternative<double>(value)) {
            node = Node(std::get<double>(value));
        }
        else if (std::holds_alternative<Array>(value)) {
            node = Node(std::get<Array>(value));
        }
        else if (std::holds_alternative<Dict>(value)) {
            node = Node(std::get<Dict>(value));
        }
        else {
            node = Node(nullptr);
        }
        AddNode(std::move(node));
        return *this;
    }

    void Builder::AddNode(Node&& node) {
        if (operation_stack_.back() == Operation::ARRAY) {
            if (arrays_open_ > 0) {
                all_arrays_[arrays_open_ - 1].push_back(std::move(node));
            }
            else throw std::logic_error("No arrays open"s);
        }
        else if (operation_stack_.back() == Operation::DICTIONARY) {
            if (dicts_open_ > 0) {
                if (keys_[dicts_open_ - 1]) {
                    all_dicts_[dicts_open_ - 1].push_back({ keys_[dicts_open_ - 1].value(), std::move(node) });
                    keys_[dicts_open_ - 1].reset();
                }
                else throw std::logic_error("No key"s);
            }
            else throw std::logic_error("Zero dicts opened"s);
        }
        else {
            root_ = std::move(node);
        }
    }

    Node Builder::Build() const {
        if (root_) {
            return root_.value();
        }
        else throw std::logic_error("Need to complete build"s);
    }

    DictValueContext DictItemContext::Key(const std::string& key) {
        return builder_.Key(key);
    }

    Builder& DictItemContext::EndDict() {
        return builder_.EndDict();
    }

    DictItemContext DictValueContext::Value(const Node::Value& value) {
        return builder_.Value(value);
    }

    DictItemContext DictValueContext::StartDict() {
        return builder_.StartDict();
    }

    ArrayItemContext DictValueContext::StartArray() {
        return builder_.StartArray();
    }

    ArrayItemContext ArrayItemContext::Value(const Node::Value& value) {
        return builder_.Value(value);
    }

    DictItemContext ArrayItemContext::StartDict() {
        return builder_.StartDict();
    }

    ArrayItemContext ArrayItemContext::StartArray() {
        return builder_.StartArray();
    }

    Builder& ArrayItemContext::EndArray() {
        return builder_.EndArray();
    }
}