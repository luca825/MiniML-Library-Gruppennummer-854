#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <typeinfo>


// Uses type erasure pattern, to hide the template of the column in the outward facing class (Column), so that DataFrame is free from templates,
// and the whole library has not to be implemented as an header only library

enum class ColumnType {
    Int,
    Float,
    Double
};

class ColumnBase {
public:
    virtual ~ColumnBase() = default;
    virtual ColumnType getType() const = 0;
    virtual std::string typeToString() const = 0;
    virtual void addValueFromDouble(double val) = 0;
    virtual std::vector<double> getDataAsDouble() const = 0;
    virtual double getDataAtAsDouble(int index) const = 0;
    virtual void setAt(double val, int index) = 0;
    virtual void deleteAt(int index) = 0; 
    virtual int size() const = 0;
    virtual void print() const = 0;
    virtual std::unique_ptr<ColumnBase> clone() const = 0; // For clone pattern
};

template<typename T>
class ColumnImpl : public ColumnBase {
private:
    std::vector<T> data;
    ColumnType type;

public:
    ColumnImpl(ColumnType type) : type(type) {}

    ColumnType getType() const override {
        return type;
    }

    void addValue(const T& value) {
        data.push_back(value);
    }

    void addValueFromDouble(double val) override {
        data.push_back(static_cast<T>(val));
    }

    int size() const override {
        return(data.size());
    }

    std::vector<double> getDataAsDouble() const override {
        std::vector<double> result;
        result.reserve(data.size());
        for (const auto& v : data) {
            result.push_back(static_cast<double>(v));
        }
        return result;
    }

    double getDataAtAsDouble(int index) const override {
        return(static_cast<double>(data.at(index)));
    }

    void deleteAt(int index) override {
        if (index < 0 || index >= data.size()) {
            throw std::out_of_range("Index out of range in deleteAt.");
        }
        data.erase(data.begin() + index);
    }

    void setAt(double val, int index) override {
        if (index < 0 || index >= data.size()) {
            throw std::out_of_range("Index out of range in setAt.");
        }
        data[index] = static_cast<T>(data.at(index));
    }

    std::vector<T>& getData() const {
        return data;
    }

    void print() const override {
        std::cout << "Data: ( ";
        for (T x : data) {
            std::cout << x << " ";
        }
        std::cout << "); Type: " + typeToString() << std::endl;

    }

    std::string typeToString() const {
        switch (type) {
            case ColumnType::Int: return "Int";
            case ColumnType::Float: return "Float";
            case ColumnType::Double: return "Double";
            default: return "Unknown";
        }
    }

    std::unique_ptr<ColumnBase> clone() const override {
        auto copy = std::make_unique<ColumnImpl<T>>(type);
        copy->data = data;
        return copy;
    }
};

class Column {
    private:
        std::unique_ptr<ColumnBase> impl;

    public:
        Column(ColumnType type) {
            switch (type) {
                case ColumnType::Int:
                    impl = std::make_unique<ColumnImpl<int>>(type);
                    break;
                case ColumnType::Float:
                    impl = std::make_unique<ColumnImpl<float>>(type);
                    break;
                case ColumnType::Double:
                    impl = std::make_unique<ColumnImpl<double>>(type);
                    break;
                default:
                    throw std::runtime_error("Unknown ColumnType");
            }
        }

        ColumnType getType() const {
            return impl->getType();
        }

        std::string getTypeAsString() const {
            return impl->typeToString();
        }

        void addValueFromDouble(double val) {
            impl->addValueFromDouble(val);
        }

        void fillFromDouble(const std::vector<double>& vals) {
            for(double val : vals) impl->addValueFromDouble(val);
        }

        std::vector<double> getDataAsDouble() const {
            return impl->getDataAsDouble();
        }

        void deleteAt(int index) {
            impl->deleteAt(index);
        }

        void setAt(double val, int index) {
            impl->setAt(val, index);
        }

        int size() const {
            return(impl->size());
        }

        void print() const {
            impl->print();
        }

        // Rule of 5´
        // Copy constructor DIY added, to allow deepcopy of a column, instead of copying the pointer
        // Copy ctor
        Column(const Column& other) {
            impl = other.impl->clone();
        }
        
        // Copy assignement
        Column& operator=(const Column& other) {
            if (this != &other) {
                impl = other.impl->clone();
            }
            return *this;
        }

        // Moving ctor
        Column(Column&&) = default;
        // Moving assignement
        Column& operator=(Column&&) = default;

        // Deconstructor
        // Default is enough, since it the class is based on a unique_ptr which handles the memory life
        ~Column() = default;

        double getDataAtAsDouble(int index) const {
            return(impl->getDataAtAsDouble(index));
        }

        Column() = delete;
};


#endif // COLUMN_H

