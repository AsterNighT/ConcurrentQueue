#pragma once
#include <condition_variable>
#include <mutex>
#include <iostream>
using DataType = long long;
const DataType NullValue = -1;

class ConQueue {
   public:
    virtual auto get() -> DataType = 0;
    virtual auto put(DataType element) -> int = 0;
    virtual auto print() -> void = 0;
};

class ListQueue : public ConQueue {
   public:
    struct Node {
        DataType value;
        Node* next;
        Node(DataType _value) : value(_value), next(nullptr) {}
    };
    ListQueue() {
        head = tail = new Node(NullValue);
    }
    auto empty() -> bool {
        return head == tail;
    }

   protected:
    Node* head;
    Node* tail;
};

class MutexNonBlockListQueue : public ListQueue {
   public:
    auto get() -> DataType override {
        std::unique_lock<std::mutex> l(readLock);
        if (empty()) {
            return NullValue;
        } else {
            Node* t = head;
            head = head->next;
            DataType v = head->value;
            l.unlock();
            delete t;
            return v;
        }
    }
    auto put(DataType element) -> int override {
        Node* t = new Node(element);
        std::unique_lock<std::mutex> l(writeLock);
        tail->next = t;
        tail = t;
        return 0;
    }

    auto print() -> void override {
        std::cout << "Non blocking - mutex - list based queue" << std::endl;
    }
   private:
    std::mutex readLock;
    std::mutex writeLock;
};