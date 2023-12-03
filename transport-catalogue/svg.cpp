#include "svg.h"

namespace svg {
    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        RenderObject(context);

        context.out << std::endl;
    }
    
    Circle& Circle::SetCenter(Point center) {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius) {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\" "sv;
        RenderAttrs(context.out);
        out << "/>"sv;
    }

    Polyline& Polyline::AddPoint(Point point) {
        std::ostringstream oss_x;
        oss_x << point.x;
        std::ostringstream oss_y;
        oss_y << point.y;


        if (polyline_.empty()) {
            polyline_ += oss_x.str() + "," + oss_y.str();
        } else {
            polyline_ += " " + oss_x.str() + "," + oss_y.str();
        }
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<polyline points=\"" << polyline_ << "\" ";
        RenderAttrs(context.out);
        out << "/>"sv;
    }
    
    Text& Text::SetPosition(Point position) {
        position_ = position;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        std::ostringstream oss_x;
        oss_x << size;
        font_size_ = oss_x.str();
        return *this;
    }

    Text& Text::SetFontFamily(std::string name) {
        font_family_ = name;
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

    Text& Text::SetData(std::string data) {
        std::string replacedData;
        replacedData.reserve(data.size());

        for (char c : data) {
            switch (c) {
            case '>':
                replacedData.append("&gt;");
                break;
            case '<':
                replacedData.append("&lt;");
                break;
            case '&':
                replacedData.append("&amp;");
                break;
            case '\'':
                replacedData.append("&aposition;");
                break;
            case '"':
                replacedData.append("&quot;");
                break;
            default:
                replacedData.push_back(c);
                break;
            }
        }

        data_ = std::move(replacedData);
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<text ";
        RenderAttrs(context.out);
        out << " x=\""sv << position_.x << "\" y=\""sv << position_.y << "\" "sv;
        out << "dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\" "sv;
        out << "font-size=\""sv << font_size_;

        if (!font_family_.empty())
        {
            out << "\" font-family=\""sv << font_family_;
        }
        if (!font_weight_.empty())
        {
            out << "\" font-weight=\""sv << font_weight_;
        }
        out << "\">"sv << data_ << "</text>";
    }

    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }
    void Document::Render(std::ostream& out) const {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;

        RenderContext context(out, 2);

        for (const auto& object : objects_) {
            object->Render(context.Indented());
        }
        out << "</svg>"sv;
    }
}
