#pragma once
#include <vector>

class Label{
public:
    Label(std::string name, int num = -1): name_(name), label_poliz_num(num) {}
    int operator[](int index) const { return goto_label[index]; }
    void put_goto(int label){ goto_label.push_back(label); }
    std::string get_name() const { return name_; }
    void set_label_num(int num) { label_poliz_num = num; }
    int get_label_num() const { return label_poliz_num; }
    int size() const { return goto_label.size(); }
private:
    std::string name_;
    int label_poliz_num;
    std::vector<int> goto_label;
};