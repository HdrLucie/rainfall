#include <stdlib.h>
#include <string.h>

class N {
    private:
        char annotation[0x64];
        int value;

    public:
        N(int val) : value(val) { }

        virtual int operator+(N& other) {
            return this->value + other.value;
        }

        virtual int operator-(N& other) {
            return this->value - other.value;
        }

        void setAnnotation(char* str) {
            memcpy(annotation, str, strlen(str));
        }
};

int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(1);
    }

    N* first = new N(5);
    N* second = new N(6);

    first->setAnnotation(argv[1]);

    return (*second) + (*first);
}
