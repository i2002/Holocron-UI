#include "Widget.h"

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, SizingPolicy policy) :
    size{size}, sizing_policy{policy}
{}

void swap(Widget &first, Widget &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(first.parent, second.parent);
    swap(first.size, second.size);
    swap(first.sizing_policy, second.sizing_policy);
}


// ----------------- Operator overloading -----------------
void Widget::display_attributes(std::ostream& os) const
{
    os << "size: " << size << ", "
       << "sizing policy: " << static_cast<int>(sizing_policy);
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    os << w.display_name() << " (";
    w.display_attributes(os); //FIXME: a way to chain these?
    os << ")\n";
    return os;
}


// ------------------ Getters and setters -----------------
cen::iarea Widget::get_size() const
{
    return size;
}

void Widget::set_size(cen::iarea size_)
{
    size = size_;
}

cen::iarea Widget::get_allocated_size() const
{
    return allocated_size;
}

void Widget::set_allocated_size(cen::iarea size_)
{
    allocated_size = size_;

    if (sizing_policy == SizingPolicy::FIT_PARENT) {
        size = allocated_size;
    }
}
