#include "json_builder.h"

namespace json {
    Builder::BaseContext Builder::EndArray() {
        if (!nodes_stack_.back()->IsArray()) {
            throw std::logic_error("Invalid state -> EndArray called without StartArray");
        } else {
            nodes_stack_.pop_back();
        }
        return Builder::BaseContext(*this);
    }

    Builder::BaseContext Builder::EndDict() {
        if (!nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Invalid state -> EndDict called without StartDict");
        } else {
            nodes_stack_.pop_back();
        };
        return Builder::BaseContext(*this);
    }

    Builder::KeyItemContext Builder::Key(const std::string& key) {
        current_key_ = key;
        availability_flag_ = true;
        return Builder::KeyItemContext(*this);
    }

    Builder::DictItemContext Builder::StartDict() {
        if (nodes_stack_.empty()) {
            nodes_stack_.push_back(&route_);
        } else {
            if (nodes_stack_.back()->IsArray()) {
                nodes_stack_.back()->AsArray().push_back(Node());
                nodes_stack_.push_back(&nodes_stack_.back()->AsArray().back());
            } else if (nodes_stack_.back()->IsDict()) {
                nodes_stack_.push_back(&nodes_stack_.back()->AsDict()[current_key_]);
                current_key_.clear();
                availability_flag_ = false;
            } else {
                throw std::logic_error("Invalid state -> other");
            }
        }
        *nodes_stack_.back() = Dict{};
        return Builder::DictItemContext(*this);
    }

    Builder::ArrayItemContext Builder::StartArray() {
        if (nodes_stack_.empty()) {
            nodes_stack_.push_back(&route_);
        } else {
            if (nodes_stack_.back()->IsArray()) {
                nodes_stack_.back()->AsArray().push_back(Node());
                nodes_stack_.push_back(&nodes_stack_.back()->AsArray().back());
            } else if (nodes_stack_.back()->IsDict()) {
                nodes_stack_.push_back(&nodes_stack_.back()->AsDict()[current_key_]);
                current_key_.clear();
                availability_flag_ = false;
            } else {
                throw std::logic_error("Invalid state -> other");
            }
        }
        *nodes_stack_.back() = Array{};
        return Builder::ArrayItemContext(*this);
    }

    Node Builder::Build() {
        if (!nodes_stack_.empty()) {
            throw std::logic_error("Invalid state -> route_");
        }

        return route_;
    }
}
