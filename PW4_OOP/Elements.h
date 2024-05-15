#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
ref class Circuit_Elem abstract {
protected:
    bool is_conn;
public:
    Circuit_Elem() {
        this->is_conn = true;
    }
    virtual bool Check() {
        return is_conn;
    }
    virtual void Connect() {
        this->is_conn = true;
    }
    virtual void Disconnect() {
        this->is_conn = false;
    }
};
ref class Contactor :public Circuit_Elem {
private:
    bool closed;
public:
    Contactor() :Circuit_Elem() {
        this->closed = true;
    }
    Contactor(int a) :Circuit_Elem() {
        if (a == 0) {
            this->closed = false;
        }
        else {
            this->closed = true;
        }
    }
    Contactor(Contactor^ c) {
        this->is_conn = c->is_conn;
        this->closed = c->closed;
    }
    void Connect() override {
        this->is_conn = true;
        this->closed = true;
    }
    void Disconnect() override {
        this->is_conn = false;
        this->closed = false;
    }
    virtual System::String^ ToString() override {
        return "\r\nКлюч з характеристиками: \r\nПідключений: " +
            System::Convert::ToString(is_conn) + ";" + "\r\n" + "Замкнений: " + System::Convert::ToString(closed);
    }
    bool Check() override {
        if (Circuit_Elem::Check() == true && this->closed == true) {
            return true;
        }
        else {
            return false;
        }
    }
};
ref class Resistance_Elem :public Circuit_Elem {
private:
    int resist;
public:
    Resistance_Elem() :Circuit_Elem() {
        this->resist = 1;
    }
    Resistance_Elem(int r) :Circuit_Elem() {
        if (r <= 0) {
            throw gcnew System::Exception("Неприпустиме значення опору елемента");
        }
        else {
            this->resist = r;
        }
    }
    Resistance_Elem(Resistance_Elem^ r) {
        this->is_conn = r->is_conn;
        this->resist = r->resist;
    }
    virtual System::String^ ToString() override {
        return "\r\nЕлемент опору з характеристиками: \r\nПідключений: " +
            System::Convert::ToString(is_conn) + ";\r\n" + "Опір: " + System::Convert::ToString(resist);
    }
    bool Check() override {
        if (Circuit_Elem::Check() == true) {
            return true;
        }
        else {
            return false;
        }
    }
};
ref class Wires :public Circuit_Elem {
private:
    int length;
public:
    Wires() :Circuit_Elem() {
        this->length = 1;
    }
    Wires(int l) :Circuit_Elem() {
        if (l <= 0) {
            throw gcnew System::Exception("Неприпустиме значення довжини елемента");
        }
        this->length = l;
    }
    Wires(Wires^ w) {
        this->is_conn = w->is_conn;
        this->length = w->length;
    }
    virtual System::String^ ToString() override {
        return "\r\nДроти з характеристиками: \r\nПідключені: " +
            System::Convert::ToString(is_conn) + ";\r\n" + "Довжина: " + System::Convert::ToString(length);
    }
    bool Check() override {
        if (Circuit_Elem::Check() == true) {
            return true;
        }
        else {
            return false;
        }
    }
};
ref class Current_Sourse :public Circuit_Elem {
private:
    int power;
    int resist;
    bool activated;
public:
    Current_Sourse() :Circuit_Elem() {
        this->power = 1;
        this->resist = 1;
        this->activated = true;
    }
    Current_Sourse(int p, int r, int b) :Circuit_Elem() {
        if (p <= 0 || r <= 0) {
            throw gcnew System::Exception("Неприпустиме значення опору або потужності елемента");
        }
        else {
            this->power = p;
            this->resist = r;
            this->activated = b;
        }
    }
    Current_Sourse(Current_Sourse^ s) {
        this->is_conn = s->is_conn;
        this->power = s->power;
        this->resist = s->resist;
        this->activated = s->activated;
    }
    void Connect() override {
        this->is_conn = true;
        this->activated = true;
    }
    void Disconnect() override {
        this->is_conn = false;
        this->activated = false;
    }
    virtual System::String^ ToString() override {
        return "\r\nДжерело струму з характеристиками: \r\nПідключене: " +
            System::Convert::ToString(is_conn) + ";\r\n" + "Напруга: " + System::Convert::ToString(power) + ";\r\n" + "Опір: "
            + System::Convert::ToString(resist) + ";\r\n" + "Активоване: " + System::Convert::ToString(activated);
    }
    bool Check() override {
        if (Circuit_Elem::Check() == true && this->activated == true) {
            return true;
        }
        else {
            return false;
        }
    }
};
ref class Electric_Circuit 
{
private:
    ref class Node 
    {
    public:
        Circuit_Elem^ ptr;
        Node^ next;
    };
    Node^ first;
    TextBox^ Box;
public:
    Electric_Circuit(TextBox^ t) {
        Box = t;
        first = nullptr;
    }
    void Add(Circuit_Elem^ e) {
        Node^ temp = gcnew Node;
        temp->ptr = e;
        temp->next = first;
        first = temp;
    }
    bool Check_Circuit() {
        Node^ temp = first;
        while (temp != nullptr) {
            if ((temp->ptr)->Check() != true) {
                return false;
            }
            temp = temp->next;
        }
        return true;
    }
    void Print() {
        Node^ temp = first;
        Box->Text = "Елементи ланцюга: \n";
        while (temp != nullptr) {
            Box->Text += temp->ptr->ToString();
            temp = temp->next;
        }
    }
    void Activate() {
        Node^ temp = first;
        while (temp != nullptr) {
            (temp->ptr)->Connect();
            temp = temp->next;
        }
    }
    void Disactivate() {
        Node^ temp = first;
        while (temp != nullptr) {
            (temp->ptr)->Disconnect();
            temp = temp->next;
        }
    }
};